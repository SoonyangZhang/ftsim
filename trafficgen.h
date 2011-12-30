/*! @file Traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __TRAFFICGEN_H__
#define __TRAFFICGEN_H__

#include "des.h"
#include "node.h"
#include "packet.h"

/*! A traffic generator attached to a node. It generates
 *  a packet of fixed size intermittently. */ 
class trafficgen: public eventhandler
{
protected:
	node* n;		//< Attached node
	unsigned sa;		//< Source address
	unsigned da;		//< Destination address
	unsigned char proto;	//< Protocol
	unsigned short sp;	//< Source port
	unsigned short dp;	//< Destination port
	bool stopped;		//< A switch to turn off the generator
public:
	trafficgen(node* _n, unsigned _sa, unsigned _da, unsigned char _proto,
		unsigned short _sp, unsigned short _dp);
	virtual ~trafficgen();
	virtual void process();			//< Generic interface for the DES to call
	virtual double nextarrival() const = 0;	//< Next packet arrival time
	virtual unsigned nextsize() const;	//< Size of next packet
	virtual inline bool stop() { return stopped = true; };
};

#endif
