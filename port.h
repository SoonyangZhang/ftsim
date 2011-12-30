/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __PORT_H__
#define __PORT_H__

#include <deque>
#include "des.h"

using std::deque;

class node;
class packet;

const unsigned defaultmaxqsize=200;

/*! A port in a node. Port is dumb that it just pass on a packet to its
 *  neighbour. No packet would be generated by a port. */
class port : public eventhandler
{
	friend void connectports(port*,port*);
protected:
	double speed;		//< Line speed
	unsigned IP;		//< The IP address
	node* neighbour;	//< The node *connected* to via cable
	node* parent;		//< The node *attached* to via circuit board
	deque<packet*> q;	//< Packet queue
	unsigned maxqsize;	//< Max queue size
public:
	port(node* _parent,unsigned _IP,double _speed,unsigned _maxqsize);
	virtual ~port();
	static port* factory(node* _parent, unsigned _IP, double _speed, unsigned _maxqsize=defaultmaxqsize);
				//< Factory method to create new port
	virtual void process();		//< Generic interface for the DES to call
	virtual void enqueue(packet* p);
				//< Enqueue a packet to the buffer
	inline unsigned getIP() { return IP; };
	inline double getspeed() { return speed; };
	inline const node* getneighbour() { return neighbour; };
	inline const node* getparent() { return parent; };
};

void connectports(port*,port*);

#endif