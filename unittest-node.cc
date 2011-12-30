/*! @file Unit test for nodes
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <iostream>
#include "node.h"
#include "port.h"
#include "rtable.h"
#include "packet.h"
#include "des.h"

using std::cout;
using std::endl;

class host : public node
{
public:
	virtual void receive(packet*_p,const node*_n);
	virtual void handle(packet*_p);
};

void
host::receive(packet*_p, const node*_n)
{
	int portnum = route(_p,_n);
	cout << "Received packet of size " << std::dec << _p->size << " from host "
		<< std::hex << _p->sa << " dest to " << _p->da
		<< ". Delivered to port " << p[portnum]->getIP() << endl;

	node::receive(_p,_n);
};

void
host::handle(packet*_p)
{
	cout << "Discarded packet." << endl;
	node::handle(_p);
};

int main(int argc, char** argv)
{
	// Create two nodes and two routers to route a packet from one host to another host
	host* dest = new host;
	host* src = new host;
	host* r1 = new host;
	host* r2 = new host;

	connectports( dest->addport(0x0A000001,1e6), r2->addport(0x0A000002,1e6) );
	connectports( src->addport(0x0A000101,1e6), r1->addport(0x0A000102,1e6) );
	connectports( r1->addport(0x0A000201,1e6), r2->addport(0x0A000202,1e6) );

	r1->addroute(0x0A000100,0xFFFFFF00,src);
	r2->addroute(0x0A000000,0xFFFFFF00,dest);
	src->addroute(0,0,r1);
	dest->addroute(0,0,r2);
	r1->addroute(0,0,r2);
	r2->addroute(0,0,r1);

	packet* p = new packet();
	p->sa = 0x0A000101;
	p->da = 0x0A000001;
	p->size = 1500*8;

	src->receive(p,src);
	des::instance().run();

	delete dest;
	delete src;
	delete r1;
	delete r2;
};
