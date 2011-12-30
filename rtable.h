/*! @file Routing table
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __RTABLE_H__
#define __RTABLE_H__

class packet;

/*! Routing tuples */
class rtuple
{
public:
	unsigned prefix;	//< Network address
	unsigned mask;		//< Mask of network address
	int port;		//< Output port
public:
	rtuple(unsigned, unsigned, int);
};

/*! Routing table */
class rtable
{
protected:
	int size;	//< Size of the routing table
	rtuple** row;	//< Rows of the routing table
public:
	rtable();
	virtual ~rtable();
	virtual int lookup(const packet*p) const;
				//< Look-up route for a packet
	virtual void insert(unsigned prefix, unsigned mask, int port);
				//< Add a route to the beginning of the routing table
	virtual void append(unsigned prefix, unsigned mask, int port);
				//< Add a route to the end of the routing table
};

#endif
