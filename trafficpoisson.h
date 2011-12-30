/*! @file Poisson arrival traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __TRAFFICPOISSON_H__
#define __TRAFFICPOISSON_H__

#include "trafficgen.h"

/*! A traffic generator of Poisson arrival and fixed packet size */
class trafficpoisson: public trafficgen
{
protected:
	double rate;		// Mean packet arrival rate
public:
	trafficpoisson(double _rate,node* _n, unsigned _sa, unsigned _da, unsigned char _proto,
		unsigned short _sp, unsigned short _dp);
	virtual ~trafficpoisson();
	virtual double nextarrival() const;	//< Next packet arrival time
};

#endif
