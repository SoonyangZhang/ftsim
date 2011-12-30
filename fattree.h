/*! @file Fat tree topology generator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __FATTREE_H__
#define __FATTREE_H__

#include "ftnode.h"

/*! A router component in a fat-tree network */
class fattree
{
protected:
	int degree;		//< Degree of each switching node
	ftnode** nodes;		//< Nodes pool
public:
	fattree(int _degree, double _speed);
	~fattree();

	// Return the host at coordinate (pod,edge,host)
	inline ftnode* host(int _pod, int _edge, int _host) {
		return nodes[((5+_pod)*degree+_edge)*degree+_host];
	};

	// Return the edge switch at coordinate (pod,edge)
	inline ftnode* edge(int _pod, int _edge) {
		return nodes[(3*degree+_pod)*degree+_edge];
	};

	// Return the aggregation switch at coordinate (pod, agg)
	inline ftnode* agg(int _pod, int _agg) {
		return nodes[(degree+_pod)*degree+_agg];
	};

	// Return the core switch at coordinate (agg, port)
	inline ftnode* core(int _agg, int _core) {
		return nodes[degree*_agg+_core];
	};
};

#endif
