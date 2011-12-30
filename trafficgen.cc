/*! @file Traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <stdlib.h>

#include "trafficgen.h"

trafficgen::trafficgen(node* _n, unsigned _sa, unsigned _da,
	unsigned char _proto, unsigned short _sp, unsigned short _dp) :
	n(_n), sa(_sa), da(_da), proto(_proto), sp(_sp), dp(_dp), stopped(false)
{
};

trafficgen::~trafficgen()
{
};

void
trafficgen::process()
{
	if (stopped) return;

	// Create a packet
	packet* p = new packet();
	p->sa = sa;
	p->da = da;
	p->proto = proto;
	p->sp = sp;
	p->dp = dp;
	p->size = nextsize();
	p->id = random();
	p->payload = NULL;

	// Send to the node attached
	n->receive(p);

	// Schedule for next packet arrival
	des::instance().schedule(nextarrival(), this);
};

unsigned
trafficgen::nextsize() const
{
	return 1500*8;
};
