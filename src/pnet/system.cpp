
// ProjectG
// Copyright 2021 Tony McCall


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "system.h"


///////////////////////////////////////////////////////////////////////
// System Functions
///////////////////////////////////////////////////////////////////////
/*
void _GetCurrentDateAndTime(char* outBuf, int outBufSize)
{
	time_t sysTime;
	struct tm localTime;

	sysTime = time(NULL);
#ifdef WINDOWS	
	localtime_s(&localTime, &sysTime);
#else
	//localtime(&localTime, &sysTime);
	*outBuf=0;
#endif
	strftime(outBuf, (size_t)outBufSize, "%m/%d/%Y %H:%M%S", &localTime);
}

void _GetCurrentDateAndTimeFile(char* outBuf, int outBufSize)
{
	time_t sysTime;
	struct tm localTime;

	sysTime = time(NULL);
#ifdef WINDOWS	
	localtime_s(&localTime, &sysTime);
#else
	//localtime(&localTime, &sysTime);
	* outBuf = 0;
#endif
	strftime(outBuf, (size_t)outBufSize, "%Y_%m_%d-%H_%M_%S", &localTime);	
}

void _ConsoleDebugOutput(const char* outBuf)
{
#if WINDOWS
	OutputDebugStringA(outBuf);
#endif
}

int _ConsoleWaitKeyPressed()
{
#ifdef WINDOWS	
	return (int)_getch();
#else
	return 0;
#endif
}

int _ConsolePollKeyPressed()
{
#if WINDOWS
	return _kbhit();
#else
	return 0;
#endif
}
*/
