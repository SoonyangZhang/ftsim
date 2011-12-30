/*! @file Discrete Event Simulator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "des.h"
#include <algorithm>

using std::push_heap;
using std::pop_heap;

des::des() : now(0), last(0)
{
	// The vector v is a heap, but as it is initially empty, we
	// do not need to call make_heap(v.begin(), v.end())
};

// Return a discrete event simulator
des&
des::instance()
{
	static des* singleton = new des();
	return *singleton;
};

void
des::schedule(double t, eventhandler* p)
{
	// Prepare an event object
	event* e;
	if (pool.size()) {
		e = pool.top();
		pool.pop();
	} else {
		e = new event;
	};
	e->time = now + t;
	e->handler = p;
	// Put it into the heap
	v.push_back(e);
	push_heap(v.begin(), v.end(), compevent);
};

// Execute the simulator
void
des::run()
{
	while (v.size()) {
		// Get the greatest event
		pop_heap(v.begin(), v.end(), compevent);
		event* e = v.back();
		v.pop_back();
		pool.push(e);

		// Shift the clock
		last = now;
		now = e->time;

		// Call the handler
		e->handler->process();
	};
};

// Clean up the events upon destruction
des::~des()
{
	while(pool.size()) {
		delete pool.top();
		pool.pop();
	};
	while (v.size()) {
		delete v.back();
		v.pop_back();
	};
};

// Get the current time from the DES
double
des::gettime() const
{
	return now;
};

eventhandler::~eventhandler()
{
};
