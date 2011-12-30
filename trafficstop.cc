/*! @file Traffic generator stopper
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "trafficstop.h"

trafficstop::trafficstop(trafficgen* _t) : t(_t)
{
};

trafficstop::~trafficstop()
{
};

void
trafficstop::process()
{
	t->stop();
};
