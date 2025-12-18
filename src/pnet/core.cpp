#include <iostream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Note: RSA_* functions are deprecated in OpenSSL 3.0+. Use EVP_* functions for new applications.

int main() {
	int ret = 0;
	RSA* rsa = nullptr;
	BIGNUM* bn = nullptr;
	// The message must be smaller than the key size minus padding overhead (e.g., < 214 bytes for a 2048-bit key with OAEP).
	unsigned char plaintext[] = "This is a short secret message!";
	unsigned char* encrypted = nullptr;
	unsigned char* decrypted = nullptr;
	int encrypted_length, decrypted_length;

	// 1. Generate RSA Keys (2048 bits)
	bn = BN_new();
	ret = BN_set_word(bn, RSA_F4); // Common public exponent
	if (ret != 1) return 1;
	rsa = RSA_new();
	ret = RSA_generate_key_ex(rsa, 2048, bn, nullptr);
	if (ret != 1) return 1;
	BN_free(bn);

	// 2. Allocate memory for encrypted/decrypted data
	// The encrypted size will be equal to the RSA key size in bytes
	encrypted_length = RSA_size(rsa);
	encrypted = (unsigned char*)malloc(encrypted_length);
	if (!encrypted) return 1;
	decrypted = (unsigned char*)malloc(encrypted_length); // Max potential size

	// 3. Encrypt with Public Key (Recommended padding: RSA_PKCS1_OAEP_PADDING)
	int public_encrypt_len = RSA_public_encrypt(sizeof(plaintext), plaintext, encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
	if (public_encrypt_len == -1) {
		ERR_print_errors_fp(stderr);
		return 1;
	}
	std::cout << "Encryption successful. Encrypted length: " << public_encrypt_len << " bytes." << std::endl;

	// 4. Decrypt with Private Key
	int private_decrypt_len = RSA_private_decrypt(public_encrypt_len, encrypted, decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
	if (private_decrypt_len == -1) {
		ERR_print_errors_fp(stderr);
		return 1;
	}
	decrypted[private_decrypt_len] = '\0'; // Null-terminate the decrypted string
	std::cout << "Decryption successful. Decrypted message: " << decrypted << std::endl;

	// 5. Cleanup
	free(encrypted);
	free(decrypted);
	RSA_free(rsa);
	EVP_cleanup(); // For older versions; might not be needed for OpenSSL 1.1.1+

	return 0;
}

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
	strncpy_s(outBuf, outBufSize, srcBuf, num);
}
	
void _szcpy(char* outBuf, size_t outBufSize, const char* srcBuf)
{
	strcpy_s(outBuf, outBufSize, srcBuf);
}

void _szcat(char* outBuf, size_t outBufSize, const char* srcBuf)
{
	strcat_s(outBuf, outBufSize, srcBuf);
}

const size_t gPrintFBufMax = 2048;
char gPrintFBuf[gPrintFBufMax] = { 0 };
char* _szprintf(const char* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
	vsnprintf_s(&gPrintFBuf[0], gPrintFBufMax, gPrintFBufMax, format, vlist);
	va_end(vlist);
	return gPrintFBuf;
}

void _szprintf(char* outBuf, size_t outBufSize, const char* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
	vsnprintf_s(&outBuf[0], outBufSize, outBufSize, format, vlist);
	va_end(vlist);
}

void _szlocaltime(char* outBuf, size_t outBufSize, const char* format)
{
	static string timeBuf;
	static struct tm tmTime;

	auto timeNow = chrono::system_clock().now();
	time_t _ctime = chrono::system_clock().to_time_t(timeNow);

	const int timeBufMax = 32;

	localtime_s(&tmTime, &_ctime);
	strftime(&timeBuf[0], timeBufMax, format, &tmTime);

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
	return _stricmp(str0, str1);
}

size_t _wszlen(const wchar_t* buf)
{
	return wcslen(buf);
}

void _wszncpy(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf, size_t num)
{
	wcsncpy_s(outBuf, outBufSize, srcBuf, num);
}

void _wszcpy(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf)
{
	wcscpy_s(outBuf, outBufSize, srcBuf);
}

void _wszcat(wchar_t* outBuf, size_t outBufSize, const wchar_t* srcBuf)
{
	wcscat_s(outBuf, outBufSize, srcBuf);
}

const size_t gWPrintFBufMax = 2048;
wchar_t gWPrintFBuf[gWPrintFBufMax] = { 0 };
wchar_t* _wszprintf(const wchar_t* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
	_vsnwprintf_s(&gWPrintFBuf[0], gWPrintFBufMax, gWPrintFBufMax, format, vlist);
	va_end(vlist);
	return gWPrintFBuf;
}

void _wszprintf(wchar_t* outBuf, size_t outBufSize, const wchar_t* format, ...)
{
	va_list vlist;
	va_start(vlist, format);
	_vsnwprintf_s(&outBuf[0], outBufSize, outBufSize, format, vlist);
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
	fopen_s(&fp, file, mode);
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
	vsnprintf_s(&gPrintFBuf[0], gPrintFBufMax, gPrintFBufMax, format, vlist);
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
	outBuf[0] = 0;
#endif
	return 0;
}
