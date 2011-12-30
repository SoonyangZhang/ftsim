/*! @file Heavy flow table
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __HFTABLE_H__
#define __HFTABLE_H__

/*! Heavy flow tuples */
class hftuple
{
public:
	unsigned int sa;	//< Source address
	unsigned int da;	//< Destination address
	unsigned char proto;	//< Protocol
	unsigned short sp;	//< Source port
	unsigned short dp;	//< Destination port
	int port;		//< Output port
public:
	hftuple(unsigned, unsigned, unsigned char, unsigned short, unsigned short, int);
};

/*! Heavy flow table */
class hftable
{
protected:
	int size;	//< Size of the routing table
	hftuple** row;	//< Rows of the routing table
	virtual hftuple* find(unsigned, unsigned, unsigned char, unsigned short, unsigned short) const;
				//< Find the entry of a flow in the heavy flow table
public:
	hftable();
	virtual ~hftable();
	virtual int lookup(unsigned, unsigned, unsigned char, unsigned short, unsigned short) const;
				//< Look-up route for a flow 
	virtual void append(unsigned, unsigned, unsigned char, unsigned short, unsigned short, int);
				//< Install a flow into the heavy flow table
	virtual void remove(unsigned, unsigned, unsigned char, unsigned short, unsigned short);
	virtual void remove(unsigned short);
				//< Remove a flow from the heavy flow table
	virtual void replace(unsigned short oldport, unsigned short newport);
};

#endif
