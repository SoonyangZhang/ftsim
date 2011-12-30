/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <assert.h>
#include <stdlib.h>

#include "md5sum.h"
#include "ftnode.h"
#include "ftport.h"
#include "packet.h"
#include "rtable.h"
#include "report.h"
#include "rlport.h"
#include "hftable.h"

ftnode::ftnode(nodetype n) : nature(n), rlimiter(NULL), rlcount(0)
{
	hft = new hftable();
	portfactory = ftport::factory;	// The default port factory
};

ftnode::~ftnode()
{
	delete hft;
};

/*! Determine the output port number for an incoming packet. As an
 *  template pattern, this incarnation only lookup the routing table. */
int
ftnode::route(packet* _p, const node* _n)
{
	// Check if it is an downward routing in the fat-tree network
	if (nature == CORE || ((const ftnode*)_n)->nature > nature) {
		// If so, route according to the routing table
		return node::route(_p,_n);
	};
	// If not, check the heavy flow table
	int hftoutput = hft->lookup(_p->sa, _p->da, _p->proto, _p->sp, _p->dp);
	if (hftoutput != -1) {
		return hftoutput;
	};
	
	// If no match, move on to random routing:
	return randomroute(_p->sa, _p->da, _p->proto, _p->sp, _p->dp);
};

/*! Random routing logic: convert a 5-tuple into the output port number */
int
ftnode::randomroute(unsigned sa, unsigned da, unsigned char proto, unsigned short sp, unsigned short dp)
{
	// If N==1, no random at all
	if (N==1) {
		return 0;
	};
	// Get a hash value based on 5-tuples
	md5sum& m = md5sum::instance();
	m << sa << da << (unsigned int)proto << sp << dp;
	unsigned char* c = m.getDigest();
	m.reset();
	// Reduce the 128-bit hash value into 32-bit unsigned integer
	unsigned int hash = 0;
	for (int i=0; i<16; i+=4) {
		hash <<= 8;
		hash += c[i+0] ^ c[i+1] ^ c[i+2] ^ c[i+4];
	};
	// Get the port sequence number by modulus (N/2)
	int portseq = (hash % (N/2))+1;
	assert(portseq*2 <= N);
	// Convert port sequence number to port number
	for (int i=0; i<N; i++) {
		if (nature < ((ftnode*)(p[i]->getneighbour()))->nature) {
			// Count the number of upward-going ports
			portseq--;
		};
		if (portseq==0) return i;
	};
	// Sanity check: If we can reach here, there are topology problem
	assert(portseq==0);
	return -1;
};

void
ftnode::handle(packet*_p)
{
	int oldport;
	int newport;
	unsigned oldIP;
	packet* flow = (packet*)(_p->payload);

	// Find its original output port
	if (flow) {
		oldport = hft->lookup(flow->sa, flow->da, flow->proto, flow->sp, flow->dp);
		if (oldport == -1) {
			oldport = randomroute(flow->sa, flow->da, flow->proto, flow->sp, flow->dp);
		};
	};

	switch (_p->proto) {
	case 0xFE:
		/* Congestion signal from edge:
		 *   Rate-limit this flow */

		if (oldport >= N) {
			// If the output port > N, this flow is already rate limited,
			// we cut the rate by half again
			rlimiter[oldport-N]->speed /= 2;
			report("# %f Throttled flow (%x,%x,%x,%x,%x) again to speed %f\n",
				des::instance().gettime(),
				flow->sa, flow->da, flow->proto, flow->sp, flow->dp,
				rlimiter[oldport-N]->speed);
		} else {
			// create a new rate limited port that cloning the current one
			rlcount++;
			rlimiter = (rlport**) realloc(rlimiter, rlcount*sizeof(rlport*));
			assert(rlimiter || rlcount==0);
			rlimiter[rlcount-1] = new rlport((ftport*)(p[oldport]));
			// set the rate limited port in a rate that of half of the original
			rlimiter[rlcount-1]->speed /= 2;
			report("# %f Throttle flow (%x,%x,%x,%x,%x) to speed %f\n",
				des::instance().gettime(),
				flow->sa, flow->da, flow->proto, flow->sp, flow->dp,
				rlimiter[rlcount-1]->speed);
			// create a flow-based route to this port
			hft->append(flow->sa, flow->da, flow->proto, flow->sp, flow->dp, rlcount+N-1);
		}
		delete flow;
		delete _p;
		break;
	case 0xFF:
		/* Congestion signal from non-edge:
		 *   Move this congested flow to next upward-going port */

		// We can do nothing if there is only one upward port
		if (N==2) {
			delete flow;
			delete _p;
			break;
		};
		// Find next upward routing route port
		newport = int((double(random())/RAND_MAX)*N);
		for(;;) {
			if (newport != oldport &&
			    nature < ((ftnode*)(p[newport]->getneighbour()))->nature) {
				break;
			} else {
				// Rewind if searched to the end
				if (++newport >= N) {
					newport = 0;
				};
			};
		};
		// Set this flow into the heavy flow table
		assert(newport < N);
		oldIP = ((oldport<N)?p[oldport]:rlimiter[oldport-N]) ->getIP();
		report("# %f Deflected flow (%x,%x,%x,%x,%x) from port %d (IP %x) to %d (IP %x)\n",
			des::instance().gettime(),
			flow->sa, flow->da, flow->proto, flow->sp, flow->dp,
			oldport, oldIP, newport, p[newport]->getIP());
		hft->append(flow->sa, flow->da, flow->proto, flow->sp, flow->dp, newport);
		delete flow;
		delete _p;
		break;
	default:
		// All others: Delete the packet
		report("r %f /Node/%x Packet %x (%x,%x,%x,%x,%x) Size=%u\n",
			des::instance().gettime(), tellIP(0), _p->id,
			_p->sa, _p->da, _p->proto, _p->sp, _p->dp, _p->size/8);
		delete _p;
	};
};

/*! Intercept congestion signal at edge switch */
void
ftnode::receive(packet*_p, const node*_n)
{
	if (_n==0) _n=this;
	if (nature==EDGE && ((const ftnode*)_n)->nature==AGG && _p->proto>=0xFE) {
		// The congestion signal is for me. Check if it is redundant
		packet* flow = (packet*)(_p->payload);
		int oldport = route(flow,this);
		const node* oldneighbour = ((oldport<N)?p[oldport]:rlimiter[oldport-N]) ->getneighbour();
		if ( _n == oldneighbour) {
			report("r %f /Node/%x CongestSignal %x (at %x of flow (%x,%x,%x,%x,%x))\n",
				des::instance().gettime(), tellIP(0), _p->id, _p->sa,
				flow->sa, flow->da, flow->proto, flow->sp, flow->dp);
			return handle(_p);
		} else {
			report("r %f /Node/%x CongestSignal %x (at %x of flow (%x,%x,%x,%x,%x)) ignored (already rerouted from %x to %x)\n",
				des::instance().gettime(), tellIP(0), _p->id, _p->sa,
				flow->sa, flow->da, flow->proto, flow->sp, flow->dp,
				_n->tellIP(0), oldneighbour->tellIP(0));
			delete flow;
			delete _p;
			return;
		};
	} else {
		return node::receive(_p,_n);
	};
};


port*
ftnode::getport(int portnum) const
{
	return (portnum < N)? node::getport(portnum) : rlimiter[portnum-N];
};

/*! Remove a rate limiting port */
void
ftnode::removerlport(rlport* p)
{
	for (unsigned i=0; i<rlcount; i++) {
		// Find the rate limiting port
		if (rlimiter[i] != p) {
			continue;
		};
		// Remove from the routing table
		hft->remove(N+i);
		delete rlimiter[i];
		// Re-order the array
		rlcount--;
		for (unsigned j=i; j<rlcount; j++) {
			rlimiter[j] = rlimiter[j+1];
			hft->replace(j+N,j+1+N);
		};
		rlimiter = (rlport**) realloc(rlimiter, rlcount*sizeof(rlport*));
		assert(rlimiter || rlcount==0);
	};
};
