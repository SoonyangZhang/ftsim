/*! @file Poisson arrival traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "trafficpoisson.h"
#include <stdlib.h>
#include <math.h>

/** Exponentially distributed random numbers in the range of [0,infty) */
inline double expo(double rate)
{
	return -log( 1-((double)(random())/RAND_MAX) ) / rate;
	/* p(t) = rate * exp( - rate*t)
	   P(t) = 1 - exp(-rate*t)
	   t = -log(1-P(t))/rate   */
};

trafficpoisson::trafficpoisson(double _rate,node* _n,
	unsigned _sa, unsigned _da, unsigned char _proto,
	unsigned short _sp, unsigned short _dp) :
	trafficgen(_n, _sa, _da, _proto, _sp, _dp), rate(_rate)
{
};

trafficpoisson::~trafficpoisson()
{
};

double
trafficpoisson::nextarrival() const
{
	return expo(rate);
};
