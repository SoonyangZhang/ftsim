/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <stdlib.h>
#include <assert.h>

#include "ftport.h"
#include "node.h"
#include "packet.h"
#include "report.h"
#include "ftnode.h"

void
ftport::enqueue(packet* p)
{
	// Drop packet if queue is already too long
	if (maxqsize > 0 && q.size() >= maxqsize && ((ftnode*)parent)->nature != HOST) {
		report("d %f /Node/%x/Port/%x Packet %x (%x,%x,%x,%x,%x) Size=%u Queue=%lu\n",
			des::instance().gettime(), parent->tellIP(0), IP, p->id,
			p->sa, p->da, p->proto, p->sp, p->dp, p->size/8, q.size());
		delete p;
		return;
	};
	// If not, put the packet into the queue first
	port::enqueue(p);
	// Then check for congestion
#if ! defined(NOREROUTE) || ! defined(NOTHROTTLE)
	static double nextsignal = 0;
	double now = des::instance().gettime();
	if (q.size() >= threshold && p->sa != IP && p->proto < 0xFE && now >= nextsignal) {
		// This port is experiencing congestion. Send a signal.
		packet* signal = new packet();
		signal->da = p->sa;
		signal->sa = IP;
#ifdef NOREROUTE
		signal->proto = 0xFE;						// congestion signaling protocol
#elif defined(NOTHROTTLE)
		signal->proto = 0xFF;						// congestion signaling protocol
#else
		signal->proto = (neighbour->tellIP(0) == p->da)?0xFE:0xFF;	// congestion signaling protocol
#endif
		signal->sp = p->sp;
		signal->dp = p->dp;
		signal->payload = (void*)new packet(*p);
		signal->id = random();
		signal->size = 40*8;	// 40 bytes
		assert(signal->payload);
		report("# %f Congestion signal sent for flow (%x,%x,%x,%x,%x) from IP %x Node type %d\n",
			des::instance().gettime(),
			p->sa, p->da, p->proto, p->sp, p->dp, IP, ((ftnode*)parent)->nature);
		parent->receive(signal,parent);
		// Stop sending signal until the current queue cleared
		unsigned totalsize = 0;
		for (deque<packet*>::const_iterator i=q.begin(); i!=q.end(); i++) {
			totalsize += (*i)->size;
		};
		nextsignal = now + totalsize/speed;
	};
#endif
};

/*! Factory method for port */
port*
ftport::factory(node* _parent, unsigned _IP, double _speed, unsigned _maxqsize)
{
	return new ftport(_parent, _IP, _speed, defaultthreshold, _maxqsize);
};

ftport::ftport(node* _parent, unsigned _IP, double _speed, unsigned _threshold, unsigned _maxqsize) :
	port(_parent,_IP,_speed,_maxqsize), threshold(_threshold)
{
};

ftport::~ftport()
{
};
