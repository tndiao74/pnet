
// Project Net
// Copyright 2025 Raw Thrills Inc.


#pragma once	// define header


///////////////////////////////////////////////////////////////////////
// Header Includes
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Buf
///////////////////////////////////////////////////////////////////////

class Buf
{
    // data
public:

    // string buffers
    static const size_t bufMax = 4096;
    static uint8_t buf0[bufMax];
    static size_t buf0Len;
    static uint8_t buf1[bufMax];
    static size_t buf1Len;
};


///////////////////////////////////////////////////////////////////////
// Encode
///////////////////////////////////////////////////////////////////////

class Encode
{
    // data
private:

    // Base64 character set
    static const int mod_table[];
    static const char encoding_table[];

    // Base64 decoding table
    static const int b64_decode_table[];

public:
    static size_t EncodeBase64(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
    static size_t DecodeBase64(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
    static std::string EncodeBase64(const std::string& bufIn);
    static std::string DecodeBase64(const std::string& bufIn);
};


///////////////////////////////////////////////////////////////////////
// Crypt
///////////////////////////////////////////////////////////////////////

class Crypt
{
    // data
private:

    struct cipherAES
    {
        EVP_CIPHER_CTX* e;
        EVP_CIPHER_CTX* d;
    };
    static cipherAES ctx;

public:

    static size_t Init(const unsigned char* key, size_t keyMax, const unsigned char* iv, size_t ivMax);
    static size_t Destroy();
    static size_t Encrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
    static size_t Decrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
};


///////////////////////////////////////////////////////////////////////
// CryptPair
///////////////////////////////////////////////////////////////////////

class CryptPair
{
    // data
private:

    static RSA* rsa;

public:

    static size_t New(std::string& outpub, std::string& outpri);
    static size_t Init(const std::string& pub, const std::string& pri);
    static size_t Destroy();
    static size_t Encrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
    static size_t Decrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
};


///////////////////////////////////////////////////////////////////////
// Zip
///////////////////////////////////////////////////////////////////////

class Zip
{
    // data
private:


public:
    static size_t Deflate(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
    static size_t Inflate(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax);
};


///////////////////////////////////////////////////////////////////////
// JSON
///////////////////////////////////////////////////////////////////////

class JSON
{
    // data
private:


public:
    static std::string CompressAndEncode(const json& j);
    static json DecodeAndUncompress(const std::string bufIn);
};


///////////////////////////////////////////////////////////////////////
// Request
///////////////////////////////////////////////////////////////////////

typedef void(*_requestDecodeEvent)(const json& j, uint16_t code);

class Request
{
    // data
private:
    static uint8_t buf[Buf::bufMax];
    static size_t bufLen;

public:
    static void Begin();
    static void Add(const json& data, uint16_t code);
    static std::string End();
    static void Decode(const std::string& param, _requestDecodeEvent cb = nullptr);
};
