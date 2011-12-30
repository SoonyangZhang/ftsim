/*! @file Rate-limiter port
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __RLPORT_H__
#define __RLPORT_H__

#include "ftport.h"

class rlport : public ftport
{
	friend class ftnode;
protected:
	double maxspeed;
public:
	rlport(node* _parent,unsigned _IP,double _speed,
		unsigned _threshold=defaultthreshold, unsigned _maxqsize=defaultmaxqsize);
	rlport(const ftport* p);
	virtual ~rlport();
	virtual void process();	// Send a packet and adjust speed
};

#endif
