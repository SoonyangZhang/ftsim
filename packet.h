/*! @file Packet model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdio.h>

class packet
{
public:
	unsigned sa;		//< Source address
	unsigned da;		//< Destination address
	unsigned char proto;	//< Protocol
	unsigned short sp;	//< Source port
	unsigned short dp;	//< Destination port
	unsigned size;		//< Packet size
	unsigned id;		//< Packet identification
	void* payload;		//< Payload (optional)
public:
	packet();
};

#endif
