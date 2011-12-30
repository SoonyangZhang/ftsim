/*! @file Fat tree topology generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "fattree.h"
#include "port.h"

/// Create the topology with N=degree
fattree::fattree(int N, double speed) : degree(N)
{
	// We need totally:
	//    N^2 core switches
	//  + 2N pods * (N edge + N aggregation + N^2 hosts)
	//  = 5N^2 + 2N^3 nodes
	nodes = new ftnode*[(5+2*N)*N*N];
	for (int i=0; i<(5+2*N)*N*N; i++) {
		nodetype t;
		if (i < N*N) t = CORE;
		else if (i < 3*N*N) t = AGG;
		else if (i < 5*N*N) t = EDGE;
		else t = HOST;

		nodes[i] = new ftnode(t);
	};

	// Find n = ceil(log_2(N))
	int n=0;
	while ((N-1) >> (++n));

	// For each of the 2N pods
	for (int i=0; i<2*N; i++) { // i = pod ID
		// Connect the edge switch to hosts
		for (int j=0; j<N; j++) { // j = edge ID
			for (int k=0; k<N; k++) { // k = Host ID
				// connect nodes[3N^2+N*i+j] to nodes[5N^2+i*N^2+j*N+k]
				// edge addr = [Pod ID][0][Edge ID][10][Host ID]
				// host addr = [Pod ID][0][Edge ID][00][Host ID]
				unsigned edgeaddr = (((((i<<(n+1))+j)<<2)+2)<<n)+k;
				unsigned hostaddr = (((((i<<(n+1))+j)<<2)+0)<<n)+k;
				int edgeid = (3*N+i)*N+j;
				int hostid = ((5+i)*N+j)*N+k;
				connectports(
					nodes[edgeid]->addport(edgeaddr, speed),
					nodes[hostid]->addport(hostaddr, speed, 0)
				);
				// Set the default route to the hosts
				nodes[hostid]->addroute(0,0,nodes[edgeid]);
				// Set a route at the edge switch towards hosts
				nodes[edgeid]->addroute(hostaddr,0xFFFFFFFF,nodes[hostid]);
			};
		};
		// Connect the edge switch to the aggregation switch
		for (int j=0; j<N; j++) { // j = edge ID
			for (int k=0; k<N; k++) { // k = agg ID
				// connect nodes[3N^2+N*i+j] to nodes[N^2+N*i+k]
				// edge addr = [Pod ID][0][Edge ID][11][Agg ID]
				// agg. addr = [Pod ID][0][Edge ID][01][Agg ID]
				unsigned edgeaddr = (((((i<<(n+1))+j)<<2)+3)<<n)+k;
				unsigned aggaddr  = (((((i<<(n+1))+j)<<2)+1)<<n)+k;
				int edgeid = (3*N+i)*N+j;
				int aggid = (N+i)*N+k;
				connectports(
					nodes[edgeid]->addport(edgeaddr, speed),
					nodes[aggid]->addport(aggaddr, speed)
				);
				// Set a route at the aggregation switch towards edge switches
				unsigned mask = 0xFFFFFFFF << (n+2);
				nodes[aggid]->addroute((edgeaddr&mask), mask, nodes[edgeid]);
			};
			// Set a default route for an edge switch
			nodes[(3*N+i)*N+j]->addroute(0,0,nodes[(N+i)*N]);
		};
		// Connect the aggregation switch to core switches
		for (int j=0; j<N; j++) { // j = agg ID
			for (int k=0; k<N; k++) { // k = core ID
				// connect nodes[N^2+N*i+j] to nodes[N*j+k]
				// agg. addr = [Pod ID][1][00][Agg ID][Core ID]
				// core addr = [Pod ID][1][01][Agg ID][Core ID]
				unsigned aggaddr  = (((((i<<3)+4)<<n)+j)<<n)+k;
				unsigned coreaddr = (((((i<<3)+5)<<n)+j)<<n)+k;
				int aggid = (N+i)*N+j;
				int coreid = N*j+k;
				connectports(
					nodes[aggid]->addport(aggaddr, speed),
					nodes[coreid]->addport(coreaddr, speed)
				);
				// Set a route at the core switch towards the aggregation switches
				unsigned mask = 0xFFFFFFFF << (2*n+3);
				nodes[coreid]->addroute((aggaddr&mask), mask, nodes[aggid]);
			};
			// Set a default route to the aggregation switch
			nodes[(N+i)*N+j]->addroute(0,0,nodes[N*j]);
		};
	};
};

fattree::~fattree()
{
	for (int i=0; i<(5+2*degree)*degree*degree; i++) {
		delete nodes[i];
	};
	delete[] nodes;
};
