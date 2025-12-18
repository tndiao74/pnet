
// Project Net
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "helpers.h"
#include "core.h"
#include "log.h"


///////////////////////////////////////////////////////////////////////
// Request
///////////////////////////////////////////////////////////////////////

uint8_t Request::buf[Buf::bufMax] = { 0, };
size_t Request::bufLen = 0;

void Request::Begin()
{
    bufLen = 0;
}

void Request::Decode(const std::string& param, _requestDecodeEvent cb)
{
    size_t paramLen = param.length();
    if (paramLen <= 0)
        return;

    // convert entire params to bytes
    bufLen = Encode::DecodeBase64((uint8_t*)param.c_str(), paramLen, buf, Buf::bufMax);
    if (bufLen <= 0)
        return;

    // decompress the bytes
    Buf::buf0Len = Zip::Inflate(buf, bufLen, Buf::buf0, Buf::bufMax);
    if (Buf::buf0Len <= 0)
        return;

    // setup target read buffer
    size_t rbufLen = Buf::buf0Len;
    uint8_t* rbuf = Buf::buf0;

    // loop over data
    size_t rbufOff = 0;
    int rstate = 0;
    uint16_t code = 0;
    uint32_t dataLen = 0;
    
    while (rbufOff < rbufLen)
    {
        // grab the action code
        if (rstate == 0)
        {
            size_t readLen = sizeof(code);
            if (rbufOff + readLen > rbufLen)
                break;
            code = *((uint16_t*)(rbuf + rbufOff));
            rbufOff += readLen;

            rstate++;
        }

        // grab the data len
        else if (rstate == 1)
        {
            size_t readLen = sizeof(dataLen);
            if (rbufOff + readLen > rbufLen)
                break;
            dataLen = *((uint32_t*)(rbuf + rbufOff));
            rbufOff += readLen;

            // abort if there is not enough data to read
            if (rbufOff + dataLen > rbufLen)
                break;

            rstate++;
        }

        // grab the data
        else if (rstate == 2)
        {
            // convert to string
            if (rbufOff + dataLen > rbufLen)
                break;

            // copy request to string
            memcpy(Buf::buf0, rbuf + rbufOff, dataLen);
            rbufOff += dataLen;
            Buf::buf0Len = dataLen;
            Buf::buf0[dataLen] = 0;
            std::string strj = (const char*)Buf::buf0;

            // convert to json
            json j = json::parse(strj);

            // invoke callback
            if (cb != nullptr)
            {
                cb(j, code);
            }

            // done
            rstate = 0;
        }

        // unknown state exit
        else
        {
            return;
        }
    }
}

void Request::Add(const json& data, uint16_t code)
{
    // convert to string
    std::string strj = data.dump().c_str();
    if (strj.length() <= 0)
        return;

    // TODO: encrypt bytes

    uint8_t* dataBytes = (uint8_t*)strj.c_str();
    uint32_t dataLen = (uint32_t)strj.length();

    // append the code
    size_t clen = sizeof(code);
    if (bufLen + clen >= Buf::bufMax)
        return;
    memcpy(buf + bufLen, &code, clen);
    bufLen += clen;

    // append the byte length size
    size_t dataLenSize = sizeof(dataLen);
    if (bufLen + dataLenSize >= Buf::bufMax)
        return;
    memcpy(buf + bufLen, &dataLen, dataLenSize);
    bufLen += dataLenSize;

    // append data bytes
    if (bufLen + dataLen >= Buf::bufMax)
        return; // size check
    memcpy(buf + bufLen, dataBytes, dataLen);
    bufLen += dataLen;
}

std::string Request::End()
{
    std::string ret = "";

    // compress bytes
    Buf::buf0Len = Zip::Deflate(buf, bufLen, Buf::buf0, Buf::bufMax);

    // encode bytes
    Buf::buf1Len = Encode::EncodeBase64(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax - 1);

    // make string
    if (Buf::buf1Len > 0)
    {
        Buf::buf1[Buf::buf1Len] = 0;
        ret = (const char*)Buf::buf1;
    }

    return ret;
}

std::string Request::EncryptPairAndEncode(const std::string& keyPub, const std::string& str)
{
    // encrypt session key with public key
    CryptPair::Init(keyPub, "");
    Buf::buf2Len = CryptPair::Encrypt((uint8_t*)str.c_str(), str.length(), Buf::buf2, Buf::bufMax);
    Buf::buf3Len = Encode::EncodeBase64(Buf::buf2, Buf::buf2Len, Buf::buf3, Buf::bufMax);
    Buf::buf3[Buf::buf3Len] = 0;
    return std::string((const char*)Buf::buf3);
}

std::string Request::DecodeAndDecryptPair(const std::string& keyPri, const std::string& str)
{
    // get session key bytes
    Buf::buf2Len = Encode::DecodeBase64((uint8_t*)str.c_str(), str.length(), Buf::buf2, Buf::bufMax);

    // unencrypt bytes with out private key
    CryptPair::Init("", keyPri);
    Buf::buf3Len = CryptPair::Decrypt((uint8_t*)Buf::buf2, Buf::buf2Len, Buf::buf3, Buf::bufMax);
    Buf::buf3[Buf::buf3Len] = 0;
    return std::string((const char*)Buf::buf3);
}

std::string Request::EncryptAndEncode(const std::string& key, const std::string& str)
{
    // encrypt with session key
    Crypt::Init(key.substr(0, 32), key.substr(32, 16));
    Buf::buf0Len = Crypt::Encrypt((uint8_t*)str.c_str(), str.length(), Buf::buf0, Buf::bufMax);
    Buf::buf1Len = Encode::EncodeBase64(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax);
    Buf::buf1[Buf::buf1Len] = 0;
    return std::string((const char*)Buf::buf1);
}

std::string Request::DecodeAndDecrypt(const std::string& key, const std::string& str)
{
    // unencrypt password with session key
    Crypt::Init(key.substr(0, 32), key.substr(32, 16));

    // grab encrypted bytes
    Buf::buf0Len = Encode::DecodeBase64((uint8_t*)str.c_str(), str.length(), Buf::buf0, Buf::bufMax);
    Buf::buf1Len = Crypt::Decrypt(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax);
    Buf::buf1[Buf::buf1Len] = 0;
    return std::string((const char*)Buf::buf1);
}


///////////////////////////////////////////////////////////////////////
// Buf
///////////////////////////////////////////////////////////////////////

uint8_t Buf::buf0[Buf::bufMax] = { 0, };
size_t Buf::buf0Len = 0;
uint8_t Buf::buf1[Buf::bufMax] = { 0, };
size_t Buf::buf1Len = 0;
uint8_t Buf::buf2[Buf::bufMax] = { 0, };
size_t Buf::buf2Len = 0;
uint8_t Buf::buf3[Buf::bufMax] = { 0, };
size_t Buf::buf3Len = 0;


///////////////////////////////////////////////////////////////////////
// Encode
///////////////////////////////////////////////////////////////////////

// Base64 character set
const int Encode::mod_table[] = { 0, 2, 1 };
const char Encode::encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64 decoding table
const int Encode::b64_decode_table[] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 0-15
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 16-31
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63, // 32-47 ('+', '/')
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1, // 48-63 ('0'-'9')
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14, // 64-79 ('A'-'O')
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1, // 80-95 ('P'-'Z')
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40, // 96-111 ('a'-'o')
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1  // 112-127 ('p'-'z')
};

size_t Encode::EncodeBase64(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    size_t outBufLen = 4 * ((bufInLen + 2) / 3); // Calculate required output length
    if (outBufLen > bufOutMax)
        return 0;

    for (size_t i = 0, j = 0; i < bufInLen;)
    {
        uint32_t octet_a = i < bufInLen ? bufIn[i++] : 0;
        uint32_t octet_b = i < bufInLen ? bufIn[i++] : 0;
        uint32_t octet_c = i < bufInLen ? bufIn[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        bufOut[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        bufOut[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        bufOut[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        bufOut[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[bufInLen % 3]; i++)
    {
        bufOut[outBufLen - 1 - i] = '='; // Add padding
    }

    return outBufLen;
}

std::string Encode::EncodeBase64(const std::string& bufIn)
{
    std::string ret;

    if (bufIn.length() > 0)
    {
        size_t len = EncodeBase64((uint8_t*)bufIn.c_str(), bufIn.length() + 1, Buf::buf0, Buf::bufMax);
        if (len > 0)
        {
            Buf::buf0[len] = 0;
            ret = (const char*)Buf::buf0;
        }
    }

    return ret;
}

std::string Encode::DecodeBase64(const std::string& bufIn)
{
    std::string ret;

    if (bufIn.length() > 0)
    {
        size_t len = DecodeBase64((uint8_t*)bufIn.c_str(), bufIn.length() + 1, Buf::buf0, Buf::bufMax);
        if (len > 0)
        {
            ret = (const char*)Buf::buf0;
        }
    }

    return ret;
}

size_t Encode::DecodeBase64(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    size_t i;
    int val;
    int bits = 0;
    int byte_idx = 0;
    size_t len = 0;

    for (i = 0; i < bufInLen; i++) {
        char c = bufIn[i];
        if (c == '=') break; // Padding character

        val = b64_decode_table[(unsigned char)c];
        if (val == -1) continue; // Invalid character

        bits = (bits << 6) | val;
        byte_idx++;

        if (byte_idx == 4) {
            if (bufOutMax < 3) return 0; // Not enough space
            bufOut[0] = (bits >> 16) & 0xFF;
            bufOut[1] = (bits >> 8) & 0xFF;
            bufOut[2] = bits & 0xFF;
            bufOut += 3;
            len += 3;
            bufOutMax -= 3;
            bits = 0;
            byte_idx = 0;
        }
    }

    // Handle remaining bits (padding)
    if (byte_idx > 0) {
        if (byte_idx == 2) { // 1 output byte
            if (bufOutMax < 1) return 0;
            bufOut[0] = (bits >> 4) & 0xFF;
            bufOutMax -= 1;
            len += 1;
        }
        else if (byte_idx == 3) { // 2 output bytes
            if (bufOutMax < 2) return 0;
            bufOut[0] = (bits >> 10) & 0xFF;
            bufOut[1] = (bits >> 2) & 0xFF;
            bufOutMax -= 2;
            len += 2;
        }
    }

    return len;
}


///////////////////////////////////////////////////////////////////////
// Crypt
///////////////////////////////////////////////////////////////////////

Crypt::cipherAES Crypt::ctx = { 0, 0 };

size_t Crypt::Init(const std::string& key, const std::string& iv)
{
    assert(key.length() == 32 && iv.length() == 16);

    // wipe out any previous ciphers
    Destroy();

    // create and initialize the cipher context
    ctx.e = EVP_CIPHER_CTX_new();
    if (ctx.e != NULL)
    {
        // initialize the encryption operation
        // Using EVP_CipherInit_ex is recommended for modern OpenSSL
        EVP_CipherInit_ex(ctx.e, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), (unsigned char*)iv.c_str(), 1 /* Encrypt */);
    }
    ctx.d = EVP_CIPHER_CTX_new();
    if (ctx.d != NULL)
    {
        // initialize the encryption operation
        // Using EVP_CipherInit_ex is recommended for modern OpenSSL
        EVP_CipherInit_ex(ctx.d, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), (unsigned char*)iv.c_str(), 0);
    }

    return 1;
}

size_t Crypt::Destroy()
{
    if (ctx.e != nullptr)
    {
        EVP_CIPHER_CTX_free(ctx.e);
    }
    if (ctx.d != nullptr)
    {
        EVP_CIPHER_CTX_free(ctx.d);
    }
    memset(&ctx, 0, sizeof(ctx));

    return 0;
}

size_t Crypt::Encrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (ctx.e == nullptr || bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    // check size
    if (bufInLen + EVP_MAX_BLOCK_LENGTH > bufOutMax)
        return 0;

    // provide the message to be encrypted
    int outLen = (int)bufOutMax;
    if (EVP_CipherUpdate(ctx.e, bufOut, &outLen, bufIn, (int)bufInLen) == 1)
    {
        int tmpLen = 0;
        // finalize the encryption (adds padding by default)
        if (EVP_CipherFinal_ex(ctx.e, bufOut + outLen, &tmpLen) == 1)
        {
            return (size_t)tmpLen;
        }
    }
    return 0;
}

size_t Crypt::Decrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (ctx.d == nullptr || bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    // provide the message to be decrypted
    int outLen = (int)bufOutMax;
    if (EVP_CipherUpdate(ctx.d, bufOut, &outLen, bufIn, (int)bufInLen) == 1)
    {
        int tmpLen = 0;
        // finalize the encryption (adds padding by default)
        if (EVP_CipherFinal_ex(ctx.d, bufOut + outLen, &tmpLen) == 1)
        {
            return (size_t)tmpLen;
        }
        else
        {
            std::string serr = ERR_error_string(ERR_get_error(), NULL);
            _log("%s", serr.c_str());
        }
    }
    return 0;
}

std::string Crypt::SHA1Hash(const std::string& str)
{
    unsigned char hash[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH is 20 bytes

    // Calculate the SHA-1 hash
    // SHA1(data, len, md)
    SHA1(reinterpret_cast<const unsigned char*>(str.c_str()),
        str.length(),
        hash); // The hash is placed in the 'hash' array

    Buf::buf0Len = Encode::EncodeBase64((uint8_t*)hash, SHA_DIGEST_LENGTH, Buf::buf0, Buf::bufMax);
    Buf::buf0[Buf::buf0Len] = 0;

    return std::string((const char*)Buf::buf0);
}


///////////////////////////////////////////////////////////////////////
// CryptPair
///////////////////////////////////////////////////////////////////////

RSA* CryptPair::rsa = nullptr;

size_t CryptPair::New(std::string& outpub, std::string& outpri)
{
    int ret = 0;
    BIGNUM* bn = nullptr;
    outpub = "";
    outpri = "";

    // 1. Generate RSA Keys (2048 bits)
    bn = BN_new();
    ret = BN_set_word(bn, RSA_F4); // Common public exponent
    if (ret != 1) return 0;
    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, 2048, bn, nullptr);
    if (ret != 1) return 0;
    BN_free(bn);

    // 1. Create a memory BIO
    // 3. Read the data from the BIO into a std::string
    // BIO_pending returns the number of bytes waiting in the BIO
    BIO* bio = BIO_new(BIO_s_mem());
    if (bio != nullptr)
    {
        PEM_write_bio_RSA_PUBKEY(bio, rsa);
        Buf::buf0Len = BIO_pending(bio);
        BIO_read(bio, Buf::buf0, static_cast<int>(Buf::buf0Len));
        Buf::buf1Len = Encode::EncodeBase64(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax - 1);
        if (Buf::buf1Len > 0)
        {
            Buf::buf1[Buf::buf1Len] = 0; // null term
            outpub = (const char*)Buf::buf1;
        }
        BIO_free_all(bio);
    }
    bio = BIO_new(BIO_s_mem());
    if (bio != nullptr)
    {
        PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);
        Buf::buf0Len = BIO_pending(bio);
        BIO_read(bio, Buf::buf0, static_cast<int>(Buf::buf0Len));
        Buf::buf1Len = Encode::EncodeBase64(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax - 1);
        if (Buf::buf1Len > 0)
        {
            Buf::buf1[Buf::buf1Len] = 0; // null term
            outpri = (const char*)Buf::buf1;
        }
        BIO_free_all(bio);
    }
    if (outpub.length() > 0 && outpri.length() > 0)
    {
        return 1;
    }

    return 0;
}

size_t CryptPair::Init(const std::string& pub, const std::string& pri)
{
    if (pub.length() <= 0 && pri.length() <= 0)
        return 0;

    Destroy();

    const std::string* sptr = nullptr;
    if (pub.length() > 0) sptr = &pub;
    else if (pri.length() > 0) sptr = &pri;
    if (sptr != nullptr)
    {
        BIO* bio = BIO_new(BIO_s_mem());
        if (bio != nullptr)
        {
            std::string dpub = Encode::DecodeBase64(*sptr);

            BIO_puts(bio, dpub.c_str());
            if(sptr == &pub)
                PEM_read_bio_RSA_PUBKEY(bio, &rsa, NULL, NULL);
            else
                PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);
            BIO_free_all(bio);

            return 1;
        }
    }
    return 0;
}

size_t CryptPair::Destroy()
{
    if (rsa != nullptr)
    {
        RSA_free(rsa);
    }
    rsa = nullptr;

    return 0;
}

size_t CryptPair::Encrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (rsa == nullptr || bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    // 2. Allocate memory for encrypted/decrypted data
    // The encrypted size will be equal to the RSA key size in bytes
    int elen = RSA_size(rsa);
    if (elen > bufOutMax)
        return 0;

    // 3. Encrypt with Public Key (Recommended padding: RSA_PKCS1_OAEP_PADDING)
    int plen = RSA_public_encrypt((int)bufInLen, bufIn, bufOut, rsa, RSA_PKCS1_OAEP_PADDING);
    if (plen == -1) {
        ERR_print_errors_fp(stderr);
        return 0;
    }

    return plen;
}

size_t CryptPair::Decrypt(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (rsa == nullptr || bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    int elen = RSA_size(rsa);
    if (elen > bufOutMax)
        return 0;

    size_t plen = RSA_private_decrypt((int)bufInLen, bufIn, bufOut, rsa, RSA_PKCS1_OAEP_PADDING);

    return plen;
}


///////////////////////////////////////////////////////////////////////
// Zip
///////////////////////////////////////////////////////////////////////

size_t Zip::Deflate(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Set up the input and output pointers and sizes
    defstream.avail_in = static_cast<uInt>(bufInLen);
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<uint8_t*>(bufIn));

    // Allocate a buffer. compressBound() can give a safe upper limit
    defstream.avail_out = static_cast<uInt>(bufOutMax);
    defstream.next_out = reinterpret_cast<Bytef*>(bufOut);

    // Initialize the compressor with default compression level
    deflateInit(&defstream, Z_DEFAULT_COMPRESSION);

    // Perform compression
    // Z_FINISH tells deflate that all input is provided
    int ret = deflate(&defstream, Z_FINISH);

    if (ret != Z_STREAM_END) {
        // Handle error here if compression fails before the stream ends
        deflateEnd(&defstream);
        return 0;
    }

    // Clean up
    deflateEnd(&defstream);

    return defstream.total_out;
}

size_t Zip::Inflate(const uint8_t* bufIn, size_t bufInLen, uint8_t* bufOut, size_t bufOutMax)
{
    if (bufIn == nullptr || bufInLen <= 0 || bufOut == nullptr || bufOutMax < bufInLen)
        return 0;

    z_stream strm = { 0 };
    // Initialize the stream structure
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Set up input buffer
    strm.avail_in = static_cast<uInt>(bufInLen);
    strm.next_in = const_cast<Bytef*>(bufIn);

    // Set up output buffer (must be pre-allocated with sufficient size)
    // In a real-world scenario, you might need a loop to dynamically grow this buffer.
    strm.avail_out = static_cast<uInt>(bufOutMax);
    strm.next_out = bufOut;

    // Initialize the inflate operation. The (15 + 32) window bits parameter 
    // tells zlib to automatically detect gzip or zlib format headers.
    int ret = inflateInit2(&strm, (15 + 32));
    if (ret == Z_OK)
    {
        // Perform the decompression in a single call (assuming buffers are large enough)
        ret = inflate(&strm, Z_FINISH);
        if (ret == Z_STREAM_END)
        {
            // Clean up the stream
            inflateEnd(&strm);

            // Resize the output data to the actual decompressed size
            return strm.total_out;
        }
    }

    // Clean up the stream
    inflateEnd(&strm);

    return 0;
}


///////////////////////////////////////////////////////////////////////
// JSON
///////////////////////////////////////////////////////////////////////

std::string JSON::CompressAndEncode(const json& j)
{
    std::string ret;

    size_t byteSize = j.dump().length();
    Buf::buf0Len = Zip::Deflate((uint8_t*)j.dump().c_str(), byteSize, Buf::buf0, Buf::bufMax);
    if (Buf::buf0Len > 0)
    {
        Buf::buf1Len = Encode::EncodeBase64(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax);
        if (Buf::buf1Len > 0 && Buf::buf1Len < Buf::bufMax - 1)
        {
            // null term base 64 string
            Buf::buf1[Buf::buf1Len] = 0;
            ret = (const char*)Buf::buf1;
        }
    }
    return ret;
}

json JSON::DecodeAndUncompress(const std::string bufIn)
{
    json j;

    Buf::buf0Len = Encode::DecodeBase64((uint8_t*)bufIn.c_str(), bufIn.length(), Buf::buf0, Buf::bufMax);
    if (Buf::buf0Len > 0)
    {
        Buf::buf1Len = Zip::Inflate(Buf::buf0, Buf::buf0Len, Buf::buf1, Buf::bufMax);
        if (Buf::buf1Len > 0 && Buf::buf1Len < Buf::bufMax - 1)
        {
            // null term base 64 string
            Buf::buf1[Buf::buf1Len] = 0;
            std::string str = (const char*)Buf::buf1;
            printf("\n%s\n", str.c_str());

            j = json::parse(str);
        }
    }
    return j;
}

///////////////////////////////////////////////////////////////////////
// Database
///////////////////////////////////////////////////////////////////////

json DB::sql_select(const std::string& strfields, const std::string& from, const std::string& where)
{
    const std::string mysqlPath = "\"C:\\Program Files\\MySQL\\MySQL Server 8.3\\bin\\mysql\"";
    auto fields = System::tokenize_whitespace(strfields);

    std::stringstream ss;
    ss << mysqlPath;
    ss << " -u root -pkkxrd pnet -s -e \"SELECT JSON_OBJECT(";
    for (size_t i = 0; i < fields.size(); i++)
    {
        const std::string& field = fields[i];
        
        ss << "'" << field << "', " << field;
        if (i < fields.size() - 1)
        {
            ss << ", ";
        }
        // mysql -u root -pkkxrd pnet -s -e "SELECT JSON_OBJECT('ID', ID, 'username', username) AS json_output FROM user;"
    }
    ss << ") AS json_output FROM " << from << " WHERE " << where << ";" << "\"";

    // run sql
    std::string cmd = ss.str();
    //std::cout << cmd;
    _log("cmd = %s", cmd.c_str());
    std::string sresultf = System::exec(cmd);

    // grab json
    size_t ji = sresultf.find_first_of('{', 0);

    json j;
    if (ji != std::string::npos)
    {
        try
        {
            std::string jsresult = sresultf.substr(ji, sresultf.length() - ji);
            printf("out1:%s\n", jsresult.c_str());
            j = json::parse(jsresult);
        }
        catch (std::exception ex)
        {
            _log("DB::sql_select: no results returned");
        }
    }

    return j;
}


///////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////
#if WINDOWS

HANDLE System::g_hChildStd_IN_Rd = NULL;
HANDLE System::g_hChildStd_IN_Wr = NULL;
HANDLE System::g_hChildStd_OUT_Rd = NULL;
HANDLE System::g_hChildStd_OUT_Wr = NULL;
HANDLE System::g_hInputFile = NULL;
CHAR System::chBuf[BUFSIZE];

void System::CreateChildProcess(const std::string& cmd)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
    static CHAR chCmd[BUFSIZE];
    _szcpy(chCmd, BUFSIZE, cmd.c_str());
    TCHAR* szCmdline = chCmd;
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    bSuccess = CreateProcess(NULL,
        szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION 

    // If an error occurs, exit the application. 
    if (!bSuccess)
        ErrorExit(TEXT("CreateProcess"));
    else
    {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example. 

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

        // Close handles to the stdin and stdout pipes no longer needed by the child process.
        // If they are not explicitly closed, there is no way to recognize that the child process has ended.

        CloseHandle(g_hChildStd_OUT_Wr);
        CloseHandle(g_hChildStd_IN_Rd);
    }
}

void System::ErrorExit(PCTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    printf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

    g_hChildStd_IN_Rd = NULL;
    g_hChildStd_IN_Wr = NULL;
    g_hChildStd_OUT_Rd = NULL;
    g_hChildStd_OUT_Wr = NULL;
    g_hInputFile = NULL;
}

void System::WriteToPipe(void)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
    DWORD dwRead, dwWritten;
    BOOL bSuccess = FALSE;
#if 0
    for (;;)
    {
        bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;

        bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
        if (!bSuccess) break;
    }
#endif
    // Close the pipe handle so the child process stops reading. 

    if (!CloseHandle(g_hChildStd_IN_Wr))
        ErrorExit(TEXT("StdInWr CloseHandle"));
}

void System::ReadFromPipe(std::string& result)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
    DWORD dwRead, dwWritten;
    BOOL bSuccess = FALSE;
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    for (;;)
    {
        bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        printf("out:\n%s", chBuf);
        if (!bSuccess || dwRead == 0) break;
        result = chBuf;
    }
}

std::string System::exec(const std::string& cmd)
{
    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited. 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT. 

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        ErrorExit(TEXT("StdoutRd CreatePipe"));

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        ErrorExit(TEXT("Stdout SetHandleInformation"));

    // Create a pipe for the child process's STDIN. 

    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
        ErrorExit(TEXT("Stdin CreatePipe"));

    // Ensure the write handle to the pipe for STDIN is not inherited. 

    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        ErrorExit(TEXT("Stdin SetHandleInformation"));

    // Create the child process. 

    CreateChildProcess(cmd);

    // Get a handle to an input file for the parent. 
    // This example assumes a plain text file and uses string output to verify data flow. 

    // Write to the pipe that is the standard input for a child process. 
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.

    WriteToPipe();
    printf("\n->Contents of %s written to child STDIN pipe.\n", cmd.c_str());

    // Read from pipe that is the standard output for child process. 

    printf("\n->Contents of child process STDOUT:\n\n");
    std::string res = "";
    ReadFromPipe(res);

    printf("\n->End of parent execution.\n");

    // The remaining open handles are cleaned up when this process terminates. 
    // To avoid resource leaks in a larger application, close handles explicitly. 


    g_hChildStd_IN_Rd = NULL;
    g_hChildStd_IN_Wr = NULL;
    g_hChildStd_OUT_Rd = NULL;
    g_hChildStd_OUT_Wr = NULL;
    g_hInputFile = NULL;

    return res;
#if 0
    // Define the appropriate popen and pclose functions based on the operating system
#ifdef _WIN32
    //FILE* pipe = _popen(cmd.c_str(), "r");
    FILE* pipe = _popen("ECHO ECHO", "r");
#else
    FILE* pipe = popen(cmd, "r");
#endif

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    char buffer[128];
    std::string result = "";

    // Read the output a line at a time and append it to the result string
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }

    // Close the pipe and get the return code
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
#endif
}
#endif

std::vector<std::string> System::tokenize_whitespace(const std::string& str)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) { // The extraction operator >> uses whitespace as a delimiter
        tokens.push_back(token);
    }

    return tokens;
}