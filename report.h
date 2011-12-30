/*! @file Debug message output
 *
 *  vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap:
 *  $Id$
 */

#ifndef __REPORT_H__
#define __REPORT_H__

#include <stdio.h>

#ifdef NDEBUG
#define report(str,...)	((void)0)
#else
#define report(str,...) \
	((void)printf(str, __VA_ARGS__))
#endif

#endif
