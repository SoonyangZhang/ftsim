/* md5sum.h - Header file of the C++ Wrapper for the MD5 implementation by L. Peter Deutsch
 * Copyright 2006 (c) The CUPIDE Project, created by Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 * Copyright 2008 (c) Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#ifndef __MD5SUM_H__
#define __MD5SUM_H__

#include "md5.h"
#include <string>

using std::string;

class md5sum {
protected:
	md5_state_t* state;
	md5_byte_t digestCode[16];
	md5sum();
public:
	static md5sum& instance();
	~md5sum();
	void reset();
	md5_byte_t* digest(const char* s);
	md5_byte_t* digest(const string& s);
	md5_byte_t* getDigest();
	void append(const char*s);
	void append(const string& s);
	void append(unsigned i);
};

md5sum& operator<<(md5sum& m, const char*s);
md5sum& operator<<(md5sum& m, const string& s);
md5sum& operator<<(md5sum& m, unsigned int i);

#endif /*  __MD5SUM_H__ */
