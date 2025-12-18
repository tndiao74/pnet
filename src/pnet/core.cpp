
// Project Net
// Copyright 2021 Tony McCall


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "core.h"
#include "log.h"


using namespace std;


///////////////////////////////////////////////////////////////////////
// Core Functions
///////////////////////////////////////////////////////////////////////

size_t _szlen(const char* buf)
{
	return strlen(buf);
}

void _szncpy(char* outBuf, size_t outBufSize, const char* srcBuf, size_t num)
{
#if WINDOWS
	strncpy_s(outBuf, outBufSize, srcBuf, num);
#endif
#if LINUX
	strncpy(outBuf, srcBuf, num);
#endif
}
	
void _szcpy(char* outBuf, size_t outBufSize, const char* srcBuf)
{
#if WINDOWS
	strcpy_s(outBuf, outBufSize, srcBuf);
#endif
#if LINUX
	strcpy(outBuf, srcBuf);
#endif
}

void _szcat(char* outBuf, size_t outBufSize, const char* srcBuf)
{
#if WINDOWS
	strcat_s(outBuf, outBufSize, srcBuf);
#endif
#if LINUX
	strcat(outBuf, srcBuf);
#endif
	
}

const size_t gPrintFBufMax = 8192;
char gPrintFBuf[gPrintFBufMax] = { 0 };
char* _szprintf(const char* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
#if WINDOWS
	vsnprintf_s(&gPrintFBuf[0], gPrintFBufMax, gPrintFBufMax, format, vlist);
#endif
#if LINUX
	vsnprintf(&gPrintFBuf[0], gPrintFBufMax, format, vlist);
#endif
	va_end(vlist);
	return gPrintFBuf;
}

void _szprintf(char* outBuf, size_t outBufSize, const char* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
#if WINDOWS
	vsnprintf_s(&outBuf[0], outBufSize, outBufSize, format, vlist);
#endif
#if LINUX
	vsnprintf(&outBuf[0], outBufSize, format, vlist);
#endif

	va_end(vlist);
}

void _szlocaltime(char* outBuf, size_t outBufSize, const char* format)
{
	static string timeBuf;

	auto timeNow = chrono::system_clock().now();
	time_t _ctime = chrono::system_clock().to_time_t(timeNow);

	const int timeBufMax = 32;
#if WINDOWS
	static struct tm tmTime;
	localtime_s(&tmTime, &_ctime);
	strftime(&timeBuf[0], timeBufMax, format, &tmTime);
#endif
#if LINUX
	struct tm* tmTime;
	tmTime = localtime(&_ctime); // Convert to local time
	strftime(&timeBuf[0], timeBufMax, format, tmTime);
#endif

	_szcpy(outBuf, outBufSize, timeBuf.c_str());
}

char* _sztolower(char* str)
{
	size_t strLen = strlen(str);
	for (size_t i = 0; i < strLen; i++)
		str[i] = tolower(str[i]);
	return str;
}

char* _sztolower(const char* strSrc, char* buf, size_t bufSize)
{
	size_t strLen = strlen(strSrc);
	if (strLen + 1 > bufSize)
	{
		assert(0);
		return buf;
	}
	for (size_t i = 0; i < strLen; i++)
		buf[i] = tolower(strSrc[i]);
	buf[strLen] = 0;
	return buf;
}

size_t _szicmp(const char* str0, const char* str1)
{
#if WINDOWS
	return _stricmp(str0, str1);
#endif
#if LINUX
	return strcasecmp(str0, str1);
#endif
}

size_t _wszlen(const wchar_t* buf)
{
	return wcslen(buf);
}

void _wszncpy(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf, size_t num)
{
#if WINDOWS
	wcsncpy_s(outBuf, outBufSize, srcBuf, num);
#endif
#if LINUX
	wcsncpy(outBuf, srcBuf, num);
#endif
}

void _wszcpy(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf)
{
#if WINDOWS
	wcscpy_s(outBuf, outBufSize, srcBuf);
#endif
#if LINUX
	wcscpy(outBuf, srcBuf);
#endif
}

void _wszcat(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf)
{
#if WINDOWS
	wcscat_s(outBuf, outBufSize, srcBuf);
#endif
#if LINUX
	wcscat(outBuf, srcBuf);
#endif
}

const size_t gWPrintFBufMax = 2048;
wchar_t gWPrintFBuf[gWPrintFBufMax] = { 0 };
wchar_t* _wszprintf(const wchar_t* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
	
#if WINDOWS
	_vsnwprintf_s(&gWPrintFBuf[0], gWPrintFBufMax, gWPrintFBufMax, format, vlist);
#endif
#if LINUX
	vswprintf(&gWPrintFBuf[0], gWPrintFBufMax, format, vlist);
#endif
	va_end(vlist);
	return gWPrintFBuf;
}

void _wszprintf(wchar_t* outBuf, size_t outBufSize, const wchar_t* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
#if WINDOWS
	_vsnwprintf_s(&outBuf[0], outBufSize, outBufSize, format, vlist);
#endif
#if LINUX
	vswprintf(&outBuf[0], outBufSize, format, vlist);
#endif
	va_end(vlist);
}

wchar_t* _sztowsz(const char* source)
{
	if (source != nullptr)
	{
		size_t len = _szlen(source);
		if (len > 0)
		{
			if (len > gWPrintFBufMax-1)
				len = gWPrintFBufMax-1;
			for (int i = 0; i < len; i++)
			{
				gWPrintFBuf[i] = (wchar_t)source[i];
			}
			gWPrintFBuf[len] = 0;
		}
	}
	return gWPrintFBuf;
}

char* _wsztosz(const wchar_t* source)
{
	if (source != nullptr)
	{
		size_t len = _wszlen(source);
		if (len > 0)
		{
			if (len > gPrintFBufMax - 1)
				len = gPrintFBufMax - 1;
			for (int i = 0; i < len; i++)
			{
				gPrintFBuf[i] = (char)source[i];
			}
			gPrintFBuf[len] = 0;
		}
	}
	return gPrintFBuf;
}

FILE* __fsopen(const char* file, const char* mode)
{
	FILE* fp = nullptr;
#if WINDOWS
	fopen_s(&fp, file, mode);
#endif
#if LINUX
	fp = fopen(file, mode);
#endif

	return fp;
}

bool __fstimestamp(const char* file, char* buf, size_t bufSize)
{
	if (!file || !buf || bufSize <= 0)
		return false;
#if WINDOWS
	HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FILETIME lastWriteTime;
	if (!GetFileTime(hFile, NULL, NULL, &lastWriteTime))
	{
		CloseHandle(hFile);
		return false;
	}

	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&lastWriteTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	_szprintf(buf, bufSize, "%02d/%02d/%d  %02d:%02d:%04d", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute, stLocal.wMilliseconds);

	CloseHandle(hFile);
#endif
	return true;
}

void __fsprintf(FILE* fp, const char* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
	
	#if WINDOWS
	vsnprintf_s(&gPrintFBuf[0], gPrintFBufMax, gPrintFBufMax, format, vlist);
#endif
#if LINUX
	vsnprintf(&gPrintFBuf[0], gPrintFBufMax, format, vlist);
#endif

	va_end(vlist);
	fputs(gPrintFBuf, fp);
}

int __fscopy(const char* scrFile, const char* destFile)
{
	int ret = 0;
#if WINDOWS	
	if (!CopyFile(scrFile, destFile, FALSE))
		ret = 1;
#else
	return 1;
#endif
	return ret;
}

int __fscreatedir(const char* dir)
{
#if WINDOWS	
	CreateDirectory(dir, NULL);
#else

#endif
	return 0;
}
