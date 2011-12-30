/*! @file Unit test for fat tree topology generator with Poisson traffic generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "fattree.h"
#include "packet.h"
#include "des.h"
#include "md5sum.h"
#include "poissonuniform.h"
#include "trafficstop.h"

//#include <mcheck.h>

using std::cout;
using std::endl;

const double speed = 1.2e4;	// Link speed = 12 kbps
const unsigned minpacket = 1500*8;
const unsigned maxpacket = 1500*8;
const int degree = 3;

int main(int argc, char** argv)
{
	// Memory leak trace
	// mtrace();

	// Initialize pseudo-random generator
	//srandom(time(NULL));
	srandom((argc>1)?atoi(argv[1]):0);

	// Create a fat-tree network
	fattree* net = new fattree(degree,speed);
	cout << "Topology created." << endl;

	// Pick two sources and one destination: host i, edge j, pod k
	//                  (((((((k<<1)+0)<<n)+j)<<2)+0)<<n)+i;
	unsigned src1addr = (((((((0<<1)+0)<<2)+0)<<2)+0)<<2)+0;
	unsigned src2addr = (((((((1<<1)+0)<<2)+0)<<2)+0)<<2)+0;
	unsigned dst1addr = (((((((5<<1)+0)<<2)+2)<<2)+0)<<2)+2;
	unsigned dst2addr = (((((((5<<1)+0)<<2)+2)<<2)+0)<<2)+1;
	ftnode* src1 = net->host(0,0,0);
	ftnode* src2 = net->host(1,0,0);

	// Traffic generator for src1 and src2
	double rate = speed/((minpacket+maxpacket)/2.0)*0.80;	// 80% utilization
	poissonuniform* tg1 = new poissonuniform(minpacket, maxpacket, rate, src1, src1addr, dst1addr, 0, 0, 0);
	poissonuniform* tg2 = new poissonuniform(minpacket, maxpacket, rate, src2, src2addr, dst2addr, 0, 0, 0);
	des::instance().schedule(0,tg1);
	des::instance().schedule(0,tg2);

	// Stop traffic gen at 100sec
	trafficstop* ts1 = new trafficstop(tg1);
	trafficstop* ts2 = new trafficstop(tg2);
	des::instance().schedule(100,ts1);
	des::instance().schedule(100,ts2);

	// Start the discrete event simulator
	des::instance().run();

	// Clean-up after the simulation finished
	cout << des::instance().gettime() << ": Completed." << endl;

	delete ts1;
	delete ts2;
	delete tg1;
	delete tg2;
	delete net;
	delete &des::instance();
	delete &md5sum::instance();
};
