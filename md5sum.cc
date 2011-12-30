/* md5sum.cc - C++ Wrapper for the MD5 implementation by L. Peter Deutsch
 * Copyright 2006 (c) The CUPIDE Project, created by Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 * Copyright 2008 (c) Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#include "md5.h"
#include "md5sum.h"
#include <string>
#include <string.h>

md5sum::md5sum()
{
	state = NULL;
	reset();
};

md5sum::~md5sum()
{
	if (state) { delete state; };
};

// Return an instance of MD5 hasher as a singleton pattern
md5sum& md5sum::instance()
{
	static md5sum* m = new md5sum();
	return *m;
};

void md5sum::reset()
{
	if (!state) { state = new md5_state_t; };
	md5_init(state);
};

md5_byte_t* md5sum::digest(const string& s)
{
	return digest(s.c_str());
};

md5_byte_t* md5sum::digest(const char* s)
{
	reset();
	md5_append(state, (const md5_byte_t*)s, strlen(s));
	md5_finish(state, digestCode);
	return digestCode;
};

md5_byte_t* md5sum::getDigest()
{
	md5_finish(state, digestCode);
	reset();
	return digestCode;
};

void md5sum::append(const string& s)
{
	append(s.c_str());
};

void md5sum::append(const char*s)
{
	md5_append(state, (const md5_byte_t*)s, strlen(s));
};

void md5sum::append(unsigned i)
{
	md5_append(state, (const md5_byte_t*)&i, sizeof(i));
};

md5sum& operator<<(md5sum& m,const string& s)
{
	return operator<<(m,s.c_str());
};

md5sum& operator<<(md5sum& m, const char*s)
{
	m.append(s);
	return m;
};

md5sum& operator<<(md5sum& m, unsigned i)
{
	m.append(i);
	return m;
};
