
// Project Net
// Copyright 2021 Tony McCall


#pragma once	// define header


///////////////////////////////////////////////////////////////////////
// Header Includes
///////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////
// Core Functions
///////////////////////////////////////////////////////////////////////

size_t _szlen(const char* buf);
void _szncpy(char* outBuf, size_t outBufSize, const char* srcBuf, size_t num);
void _szcpy(char* outBuf, size_t outBufSize, const char* srcBuf);
void _szcat(char* outBuf, size_t outBufSize, const char* srcBuf);
void _szprintf(char* outBuf, size_t outBufSize, const char* format,...);
char* _szprintf(const char* format, ...);
void _szlocaltime(char* outBuf, size_t outBufSize, const char* format);
char* _sztolower(char* str);
char* _sztolower(const char* strSrc, char* buf, size_t bufSize);
size_t _szicmp(const char* str0, const char* str1);

size_t _wszlen(const wchar_t* buf);
void _wszncpy(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf, size_t num);
void _wszcpy(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf);
void _wszcat(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf);
void _wszprintf(wchar_t* outBuf, size_t outBufSize, const wchar_t* format, ...);
wchar_t* _wszprintf(const wchar_t* format, ...);
wchar_t* _sztowsz(const char* source);
char* _wsztosz(const wchar_t* source);

FILE* __fsopen(const char* file, const char* mode);
bool __fstimestamp(const char* file, char* buf, size_t bufSize);
void __fsprintf(FILE* fp, const char* format, ...);
int __fscopy(const char* scrFile, const char* destFile);
int __fscreatedir(const char* dir);
