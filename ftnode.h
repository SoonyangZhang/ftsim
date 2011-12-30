/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __FTNODE_H__
#define __FTNODE_H__

#include "node.h"

class hftable;
class rlport;

/*! An enum of different node types */
typedef enum {
	HOST,
	EDGE,
	AGG,
	CORE
} nodetype;

/*! A router component in a fat-tree network */
class ftnode : public node
{
	friend class ftport;
protected:
	nodetype nature;	//< Nature of this node
	hftable* hft;		//< The heavy flow table
	rlport** rlimiter;	//< Rate limiter ports
	unsigned rlcount;	//< Rate limiter counts
public:
	ftnode(nodetype n);
	virtual ~ftnode();
	virtual void receive(packet*_p,const node*_n=0);
	virtual void handle(packet*_p);
				//< Handles the congestion signal
	virtual int route(packet*_p, const node* _n);
				//< Random routing and heavy flow reroute 
	int randomroute(unsigned sa, unsigned da, unsigned char proto, unsigned short sp, unsigned short dp);
				//< Helper for random routing: 5-tuple to port
	virtual void removerlport(rlport* p);
				//< Remove a rate limiting port
	virtual port* getport(int portnum) const;
};

#endif
