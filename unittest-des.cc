/*! @file Unit test for Discrete Event Simulator
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include <iostream>
#include "des.h"

using std::cout;
using std::endl;

class dummyhndr : public eventhandler
{
private:
	des& DES;
public:
	dummyhndr();
	void process();
};

dummyhndr::dummyhndr() : 
	DES(des::instance())
{
};

void
dummyhndr::process()
{
	cout << DES.gettime() << endl;
};

int main(int argc, char** argv)
{
	des& DES = des::instance();
	dummyhndr* h = new dummyhndr();
	DES.schedule(0.1, h);
	DES.schedule(1.3, h);
	DES.schedule(1.1, h);
	DES.schedule(0.5, h);
	DES.schedule(0.3, h);
	DES.schedule(0.7, h);
	DES.run();	// Numbers printed should in ascending order
	delete h;
};
