/*! @file Unit test for fat tree topology generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <iostream>

#include "fattree.h"
#include "packet.h"
#include "des.h"
#include "md5sum.h"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	// Create two nodes and two routers to route a packet from one host to another host
	fattree* net = new fattree(3,1e4);	// 10 kbps
	cout << "Topology created." << endl;

	// Pick two sources and one destination
	unsigned src1addr = (((((0<<(2+1))+0)<<2)+0)<<2)+0;
	unsigned src2addr = (((((1<<(2+1))+0)<<2)+0)<<2)+0;
	unsigned dst1addr = (((((5<<(2+1))+2)<<2)+0)<<2)+2;
	unsigned dst2addr = (((((5<<(2+1))+2)<<2)+0)<<2)+1;
	ftnode* src1 = net->host(0,0,0);
	ftnode* src2 = net->host(1,0,0);

	// Inject packets to hosts: Try to make it least for 2 sec
	packet *p;
	for (int i=0; i<20; i++) {
		p = new packet();
		p->sa = src1addr;
		p->da = dst1addr;
		p->size = 1500*8;
		src1->receive(p,src1);

		p = new packet();
		p->sa = src2addr;
		p->da = dst2addr;
		p->size = 1500*8;
		src2->receive(p,src2);
	};
	des::instance().run();

	cout << des::instance().gettime() << ": Completed." << endl;

	delete net;
	delete &des::instance();
	delete &md5sum::instance();
};
