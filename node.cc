/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <stdlib.h>
#include <assert.h>

#include "node.h"
#include "packet.h"
#include "port.h"
#include "rtable.h"
#include "report.h"

/*! A node receiving a packet from its neighbour: This is all the data plane
 *  logic of a router. */
void
node::receive(packet*_p, const node*_n)
{
	if (_n==0) _n=this;
	int portnum = route(_p,_n);
	port* outport = getport(portnum);

	// If I am the destination, handle the packet by function handle()
	if (_p->da == outport->getIP()) {
		return handle(_p);
	};

	// Otherwise, forward to the output port
	outport->enqueue(_p);
};


port*
node::getport(int portnum) const
{
	return p[portnum];
};


/*! Determine the output port number for an incoming packet. As an
 *  template pattern, this incarnation only lookup the routing table. */
int
node::route(packet* _p, const node* _n)
{
	return r->lookup(_p);
};

void
node::handle(packet*_p)
{
	report("r %f /Node/%x/Port/%x Packet %x (%x,%x,%x,%x,%x) Size=%u\n",
		des::instance().gettime(), tellIP(0), _p->id, _p->da,
		_p->sa, _p->da, _p->proto, _p->sp, _p->dp, _p->size/8);
	delete _p;
};

node::~node()
{
	// Delete all ports
	while(N--) {
		delete p[N];
	};
	if (p) {
		free(p);
	};
	delete r;
};

node::node() : N(0), p(NULL)
{
	r = new rtable;
	portfactory = port::factory;	// The default port factory
};

bool
node::addroute(unsigned _IP, unsigned _mask, const node* nexthop)
{
	// Resolve nexthop into port number
	for (int i=0; i<N; i++) {
		if (p[i]->getneighbour() == nexthop) {
			// Found port number, then install route
			r->append(_IP, _mask, i);
			return true;
		};
	};
	// Next-hop cannot resolve into port number
	return false;
};

port*
node::addport(unsigned _IP, double _speed, unsigned _maxqsize)
{
	// Increment port count and create new port
	N++;
	p = (port**) realloc(p,N*sizeof(port*));
	assert(p || N==0);	// make sure realloc success
	p[N-1] = (*portfactory)(this, _IP, _speed, _maxqsize);
	// Install local route
	r->insert(_IP,0xFFFFFFFF,N-1);
	return p[N-1];
};
