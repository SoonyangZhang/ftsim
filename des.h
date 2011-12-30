/*! @file Discrete Event Simulator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __DES_H__
#define __DES_H__

#include <vector>
#include <stack>

using std::vector;
using std::stack;

class eventhandler
{
public:
	virtual void process()=0;
	virtual ~eventhandler();
};

class event
{
public:
	double time;
	eventhandler* handler;
};

class des
{
private:
	des();
protected:
	double now;
	double last;
	vector<event*> v;	//< Vector of events
	stack<event*> pool;	//< Pool of unused event objects
public:
	static des& instance();	//< Get instance
	void schedule(double t, eventhandler* p);
				//< Schedule new event
	void run();		//< Run the simulator
	double gettime() const;	//< Get current time
	~des();
};

/*! Compare two events. A `larger' event is the one with smaller time value. */
inline bool compevent(event* e1, event* e2) {
	// Say e1 < e2 iff e1.time > e2.time
	return (e1->time > e2->time);
};

#endif
