/*! @file Poisson arrival traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __POISSONUNIFORM_H__
#define __POISSONUNIFORM_H__

#include "trafficpoisson.h"

/*! A traffic generator of Poisson arrival and fixed packet size */
class poissonuniform: public trafficpoisson 
{
protected:
	unsigned minsize;		// Min packet size
	unsigned maxsize;		// Max packet size
public:
	poissonuniform(unsigned _minsize, unsigned _maxsize, double _rate, node* _n, unsigned _sa, unsigned _da, unsigned char _proto,
		unsigned short _sp, unsigned short _dp);
	virtual ~poissonuniform();
	virtual unsigned nextsize() const;	//< Size of next packet
};

#endif
