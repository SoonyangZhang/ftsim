/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <stdlib.h>
#include <assert.h>

#include "rtable.h"
#include "packet.h"

rtable::rtable() : size(0), row(NULL)
{
};


/*! Add a route to the end of the routing table */
void
rtable::append(unsigned prefix, unsigned mask, int port)
{
	rtuple* tuple = new rtuple(prefix,mask,port);
	size++;
	row = (rtuple**)realloc(row, size*sizeof(rtuple*));
	assert(row || size==0);	// make sure realloc success
	row[size-1] = tuple;
};


/*! Add a route to the beginning of the routing table */
void
rtable::insert(unsigned prefix, unsigned mask, int port)
{
	// Make room at the beginning
	size++;
	row = (rtuple**)realloc(row, size*sizeof(rtuple*));
	assert(row || size==0);	// make sure realloc success
	for (int i=size-1; i>0; i--) {
		row[i] = row[i-1];
	};
	// Add route to row zero
	rtuple* tuple = new rtuple(prefix,mask,port);
	row[0] = tuple;
};


/*! Clean up */
rtable::~rtable()
{
	while(size) {
		delete row[--size];
	};
	if (row) {
		free(row);
	};
};


rtuple::rtuple(unsigned _prefix, unsigned _mask, int _port) :
	prefix(_prefix), mask(_mask), port(_port)
{
};


/*! Lookup route according to the destination address of a packet.
 *  Return -1 if not routable */
int
rtable::lookup(const packet*p) const
{
	// Sequential look-up until first match
	for (int i=0; i<size; i++) {
		if ((p->da & row[i]->mask) == row[i]->prefix) {
			return row[i]->port;
		};
	};
	// Declare unroutable if all failed
	return -1;
};
