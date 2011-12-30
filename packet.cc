/*! @file Packet model
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#include "packet.h"

/*! Default constructor that initialises everything into zero */
packet::packet() : sa(0), da(0), proto(0), sp(0), dp(0), size(0), payload(NULL)
{
};
