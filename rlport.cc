/*! @file Rate-limiter port
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "rlport.h"
#include "ftnode.h"

rlport::rlport(node* _parent, unsigned _IP, double _speed, unsigned _threshold, unsigned _maxqsize) :
	ftport(_parent,_IP,_speed,_threshold,_maxqsize)
{
	maxspeed = speed;
};

rlport::rlport(const ftport* p) :
	ftport(*p)
{
	maxspeed = speed;
};

rlport::~rlport()
{
};

void
rlport::process()
{
	// Send the packet
	port::process();
	// Additive increase: Double when the queue emptied
	speed += speed/q.size();
	if (speed >= maxspeed) {
		speed = maxspeed;
		// If the rlport already reached the original speed and its
		// packet queue is emptied, remove this port
		if (q.empty()) {
			((ftnode*)parent)->removerlport(this);
		};
	};
	// TODO: The correct formula for Add. Inc.
};
