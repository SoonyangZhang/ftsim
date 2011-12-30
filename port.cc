/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <stdlib.h>
#include "port.h"
#include "node.h"
#include "packet.h"
#include "report.h"

port::port(node* _parent, unsigned _IP, double _speed, unsigned _maxqsize) :
	speed(_speed), IP(_IP), neighbour(NULL), parent(_parent), maxqsize(_maxqsize)
{
};


/*! Process a packet by an output port */
void
port::process()
{
	// Send a packet to the receiver and remove from my queue
	packet* p = q.front();
	q.pop_front();
	report("- %f /Node/%x/Port/%x Packet %x (%x,%x,%x,%x,%x) Size=%u Queue=%lu\n",
		des::instance().gettime(), parent->tellIP(0), IP, p->id,
		p->sa, p->da, p->proto, p->sp, p->dp, p->size/8, q.size());
	neighbour->receive(p, parent);
	// Then, process the next packet (with processing delay)
	if (q.size()) {
		des::instance().schedule(q.front()->size/speed, this);
	};
};


void
port::enqueue(packet* p)
{
	q.push_back(p);
	report("+ %f /Node/%x/Port/%x Packet %x (%x,%x,%x,%x,%x) Size=%u Queue=%lu\n",
		des::instance().gettime(), parent->tellIP(0), IP, p->id,
		p->sa, p->da, p->proto, p->sp, p->dp, p->size/8, q.size());
	// If this is the only packet at the output queue, schedule
	// for its departure
	if (q.size()==1) {
		des::instance().schedule(p->size/speed, this);
	};
};

/*! connects two ports */
void connectports(port* a,port* b)
{
	a->neighbour = b->parent;
	b->neighbour = a->parent;
};

/*! Factory method for port */
port*
port::factory(node* _parent, unsigned _IP, double _speed, unsigned _maxqsize)
{
	return new port(_parent, _IP, _speed, _maxqsize);
};

port::~port()
{
};
