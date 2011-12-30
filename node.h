/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __NODE_H__
#define __NODE_H__

#include "port.h"

class packet;
class rtable;

/*! A router component in a fat-tree network */
class node 
{
protected:
	int N;		//< Number of ports
	port** p;	//< Array of ports
	rtable* r;	//< Routing table
public:
	port* (*portfactory)(node*, unsigned, double, unsigned);
			//< Pointer to the factory method to make new ports
public:
	node();
	virtual ~node();
	virtual void receive(packet*_p,const node*_n=0);
				//< Receive a packet from somebody
	virtual void handle(packet*_p);
				//< Handle a packet if this node is the destination
	virtual int route(packet*_p, const node* _n);
				//< Determine an output port for an incoming packet
	virtual port* addport(unsigned _IP, double _speed, unsigned _maxqsize=defaultmaxqsize);
	virtual bool addroute(unsigned _IP, unsigned _mask, const node* nexthop);
	virtual port* getport(int portnum) const;
	inline unsigned tellIP(int n) const { return p[n]->getIP(); };
};

#endif
