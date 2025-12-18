
// Project Net
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "core.h"
#include "log.h"



///////////////////////////////////////////////////////////////////////
// log
///////////////////////////////////////////////////////////////////////

const size_t gLogBufMax = 2048;
char gLogBuf[gLogBufMax] = { 0 };
void _log(const char* msg, ...)
{
	va_list vlist;
	va_start(vlist, msg);
	//vsnprintf_s(&gLogBuf[0], gLogBufMax, gLogBufMax, msg, vlist);
	

	// one at a time
	static std::mutex mutex;
	const std::lock_guard<std::mutex> lock(mutex);

	// static bufs
	const int msgBufMax = 4096;
	const int timeBufMax = 32;
	static char msgBuf[msgBufMax];
	static char timeBuf[timeBufMax];
	static char outBuf[msgBufMax];

	// format input message
	vsnprintf(&msgBuf[0], msgBufMax, msg, vlist);

	va_end(vlist);
	
	// grab time stamp
	_szlocaltime(timeBuf, timeBufMax, "%F %I:%M:%S");

	// build final output
	_szprintf(outBuf, msgBufMax, "%s %s\n", timeBuf, msgBuf);
#if 0
	if (!(flags & DOUTF_NO_TIME_STAMP))
	{
		// grab time stamp
		_szlocaltime(timeBuf, timeBufMax, "%F %I:%M:%S");

		// format output
		if (flags & DOUTF_NO_RETURN)
		{
			_szprintf(outBuf, msgBufMax, "%s %s", timeBuf, msgBuf);
		}
		else
		{
			_szprintf(outBuf, msgBufMax, "%s %s\n", timeBuf, msgBuf);
		}
	}
	else
	{
		// format output
		if (flags & DOUTF_NO_RETURN)
		{
			_szprintf(outBuf, msgBufMax, "%s", msgBuf);
		}
		else
		{
			_szprintf(outBuf, msgBufMax, "%s\n", msgBuf);
		}
	}
#endif
	//
	// dump output
	//

	// stdio
	//printf("%s", outBuf);

	// log file
	std::ofstream myfile;
	myfile.open("log.txt");
	myfile << outBuf;
	myfile.close();

	// debug consle
	//_ConsoleDebugOutput(outBuf);
}
