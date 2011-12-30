/*! @file Simulator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <deque>
#include <math.h>

#include "fattree.h"
#include "packet.h"
#include "des.h"
#include "md5sum.h"
#include "poissonuniform.h"
#include "trafficstop.h"

//#include <mcheck.h>

using std::cout;
using std::endl;
using std::deque;

double speed = 1.0e6;	// Link speed = 1 Mbps
unsigned minpacket = 50*8;
unsigned maxpacket = 1500*8;
int degree = 3;

/** Exponentially distributed random numbers in the range of [0,infty) with mean 1/rate */
inline double expo(double rate)
{
	return -log( 1-((double)(random())/RAND_MAX) ) / rate;
	/* p(t) = rate * exp( - rate*t)
	   P(t) = 1 - exp(-rate*t)
	   t = -log(1-P(t))/rate   */
};


inline unsigned addr(int pod, int edge, int host)
{
	// Get the addresss of the host at: host i, edge j, pod k
	const int n=ceil(log2(degree));
	return (((((((pod<<1)+0)<<n)+edge)<<2)+0)<<n)+host;
};

int main(int argc, char** argv)
{
	// Memory leak trace
	// mtrace();

	int opt;
	double util = 0.01;
	double duration = 60*60*10;
	double flowsize = 60;	// A flow last for 60 sec in avg
	deque<poissonuniform*> gen;
	deque<trafficstop*> stopper;

	srandom(time(NULL));

	// Parse command line
	while ((opt=getopt(argc,argv,"s:u:t:f:")) != -1) {
		switch (opt) {
		case 's':	// Random seed
			srandom(atoi(optarg));
			break;
		case 'u':	// Utilization in percentage
			util = atof(optarg)/100;
			break;
		case 't':	// Simulation duration
			duration = atof(optarg);
			break;
		case 'l':	// Link speed
			speed = atof(optarg);
			break;
		case 'f':
			flowsize = atof(optarg);
			break;
		};
	};

	// Create a fat-tree network
	fattree* net = new fattree(degree,speed);
	cout << "Topology created." << endl;

	// Prepare the traffic generator and stoppers
	double time=0;
	while (time < duration) {
		// Generator of a flow
		int spod  = 2.0*degree*random()/RAND_MAX;
		int dpod  = 2.0*degree*random()/RAND_MAX;
		int sedge = 1.0*degree*random()/RAND_MAX;
		int dedge = 1.0*degree*random()/RAND_MAX;
		int shost = 1.0*degree*random()/RAND_MAX;
		int dhost = 1.0*degree*random()/RAND_MAX;
		double rate = speed/((minpacket+maxpacket)/2.0)*(0.98*random()/RAND_MAX + 0.01);
		poissonuniform* g =
			new poissonuniform(minpacket, maxpacket, rate,
				net->host(spod, sedge, shost),
				addr(spod, sedge, shost),
				addr(dpod, dedge, dhost), 0,
				65536.0*random()/RAND_MAX, 65536.0*random()/RAND_MAX);
		des::instance().schedule(time,g);
		gen.push_back(g);
		// Stopper of a flow
		trafficstop* s = new trafficstop(g);
		des::instance().schedule(time+expo(1/flowsize),s);
		stopper.push_back(s);
		// Prepare for next flow
		time += expo(util*degree*degree*degree*4/flowsize);
	};

	// Start the discrete event simulator
	des::instance().run();

	// Clean-up after the simulation finished
	cout << "Completed on " << des::instance().gettime() << endl;

	while (! gen.empty()) {
		delete gen.front();
		gen.pop_front();
	};
	while (! stopper.empty()) {
		delete stopper.front();
		stopper.pop_front();
	};
	delete net;
	delete &des::instance();
	delete &md5sum::instance();
};
