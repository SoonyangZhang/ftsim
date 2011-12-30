/*! @file Router model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <stdlib.h>
#include <assert.h>

#include "hftable.h"


hftuple::hftuple(unsigned _sa, unsigned _da, unsigned char _proto,
	unsigned short _sp, unsigned short _dp, int _port) :
	sa(_sa), da(_da), proto(_proto), sp(_sp), dp(_dp), port(_port)
{
};


hftable::hftable() : size(0), row(NULL)
{
};


/*! Install a route into the routing table */
void
hftable::append(unsigned sa, unsigned da, unsigned char proto,
	unsigned short sp, unsigned short dp, int port)
{
	// Look for an existing entry
	hftuple* t = find(sa,da,proto,sp,dp);
	if (t) {
		t->port = port;
		return;
	};
	// If no existing entry, create a new entry
	hftuple* tuple = new hftuple(sa,da,proto,sp,dp,port);
	size++;
	row = (hftuple**)realloc(row, size*sizeof(hftuple*));
	assert(row || size==0);	// make sure realloc success
	row[size-1] = tuple;
};


/*! Remove a route from the routing table */
void
hftable::remove(unsigned sa, unsigned da, unsigned char proto,
	unsigned short sp, unsigned short dp)
{
	// Sequential look-up until first match
	for (int i=0; i<size; i++) {
		if ((sa    == row[i]->sa) &&
		    (da    == row[i]->da) &&
		    (proto == row[i]->proto) &&
		    (sp    == row[i]->sp) &&
		    (dp    == row[i]->dp)
		) {
			delete row[i];
			size--;
			for (;i<size;i++) {
				row[i] = row[i+1];
			};
			row = (hftuple**)realloc(row, size*sizeof(hftuple*));
			assert(row || size==0);
			return;
		};
	};
};


/*! Remove a route from the routing table */
void
hftable::remove(unsigned short port)
{
	// Sequential look-up until first match
	for (int i=0; i<size; i++) {
		if (port == row[i]->port) {
			delete row[i];
			size--;
			for (;i<size;i++) {
				row[i] = row[i+1];
			};
			row = (hftuple**)realloc(row, size*sizeof(hftuple*));
			assert(row || size==0);
			return;
		};
	};
};


/*! Replace an output port with another */
void
hftable::replace(unsigned short oldport, unsigned short newport)
{
	for (int i=0; i<size; i++) {
		if (row[i]->port == oldport) {
			row[i]->port = newport;
		};
	};
};


/*! Clean up */
hftable::~hftable()
{
	while(size) {
		delete row[--size];
	};
	if (row) {
		free(row);
	};
};


/*! Lookup route according to the destination address of a packet.
 *  Return -1 if not routable */
int
hftable::lookup(unsigned sa, unsigned da, unsigned char proto, unsigned short sp,
	unsigned short dp) const
{
	// Sequential look-up for the exact match
	hftuple* t = find(sa,da,proto,sp,dp);
	// Tell the output port or declare unroutable if failed
	return (t)?(t->port):-1;
};

hftuple*
hftable::find(unsigned sa, unsigned da, unsigned char proto, unsigned short sp,
	unsigned short dp) const
{
	// Sequential look-up until first match
	for (int i=0; i<size; i++) {
		if ((sa    == row[i]->sa) &&
		    (da    == row[i]->da) &&
		    (proto == row[i]->proto) &&
		    (sp    == row[i]->sp) &&
		    (dp    == row[i]->dp)
		) {
			return row[i];
		};
	};
	// Failed: return NULL
	return NULL;
};
