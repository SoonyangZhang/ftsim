/*! @file Traffic generator stopper
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __TRAFFICSTOP_H__
#define __TRAFFICSTOP_H__

#include "des.h"
#include "trafficgen.h"

/*! A traffic generator stopper */
class trafficstop: public eventhandler
{
protected:
	trafficgen* t;		//< Attached node
public:
	trafficstop(trafficgen* t);
	virtual ~trafficstop();
	virtual void process();			//< Generic interface for the DES to call
};

#endif
