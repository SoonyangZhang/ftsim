/*! @file Traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "poissonuniform.h"
#include <stdlib.h>

poissonuniform::poissonuniform(unsigned _minsize, unsigned _maxsize,
	double _rate, node* _n, unsigned _sa, unsigned _da, unsigned char _proto,
	unsigned short _sp, unsigned short _dp) :
	trafficpoisson(_rate, _n, _sa, _da, _proto, _sp, _dp), minsize(_minsize), maxsize(_maxsize)
{
};

poissonuniform::~poissonuniform()
{
};


unsigned
poissonuniform::nextsize() const
{
	return minsize+(double)(random())/RAND_MAX*(maxsize-minsize);
};
