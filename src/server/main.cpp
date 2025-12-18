
// Project Net
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "../pnet/base64.h"
#include "../pnet/log.h"
#include "../pnet/helpers.h"
#include "../pnet/core.h"

#include "main.h"


///////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////

const char* gErrResponse =
    "<!DOCTYPE html>" \
    "<html>" \
    "<head>" \
    "  <title>Invalid request</title>" \
    "</head>" \
    "<body>" \
    "  <p>Invalid request.</p>" \
    "</body>" \
    "</html>";

const char* gServerResponse =
    "<!DOCTYPE html>" \
    "<html>" \
    "<head>" \
    "  <title>Server Response</title>" \
    "</head>" \
    "<body>" \
    "  <p>%s</p>" \
    "</body>" \
    "</html>";

///////////////////////////////////////////////////////////////////////
// Internal Prototypes
///////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////
// application entry point
///////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // debug memory usage
#ifdef WINDOWS
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 1959565;
#endif

    static const size_t bufMax = 1024;
    static uint8_t buf0[bufMax];
    static size_t buf0Len = 0;
    static uint8_t buf1[bufMax];
    static size_t buf1Len = 0;
    memset(buf0, 0, sizeof(buf0));
    memset(buf1, 0, sizeof(buf1));

    int z = 0;

    _szprintf((char*)buf0, bufMax, "%s",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    buf0Len = strlen((char*)buf0) + 1;
    buf0[buf0Len - 1] = '9';
    z = 0;

    buf1Len = Zip::Deflate(buf0, buf0Len, buf1, bufMax);
    memset(buf0, 0, sizeof(buf0));
    buf0Len = Zip::Inflate(buf1, buf1Len, buf0, bufMax);
    char cz = buf0[buf0Len - 1];
    z = 0;
#if 0
    // Dummy key and IV for demonstration (use secure methods in production)
    const unsigned char key[] = "0123456789abcdef0123456789abcdef"; // 32 bytes for AES-256
    const unsigned char iv[] = "0123456789abcdef";                   // 16 bytes for AES block size
    Crypt::Init(key, 32, iv, 16);
    buf1Len = Crypt::Encrypt(buf0, buf0Len, buf1, bufMax);
    memset(buf0, 0, sizeof(buf0));
    buf0Len = Crypt::Decrypt(buf1, buf1Len, buf0, bufMax);
    z = 0;
#endif

    //Encode::EncodeBase64(buf0, )

    _log("pnet server invoke instance");

    printf("Content-type: text/html\n\n"); // Required HTTP header

    const char* queryString = std::getenv("QUERY_STRING");

    if (queryString != nullptr)
    {
        std::string input(queryString);
        _log("input\n%s\n", input.c_str());
        printf(gServerResponse, input.c_str());
        
        // Note: The input string needs URL decoding and parsing into key-value pairs
    }
    else
    {
        printf("<p>No QUERY_STRING environment variable found.</p>");
        _log("No QUERY_STRING environment variable found.");
    }
#if 0
    const char* bgMsg =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    cryptoCipher ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx = crypto_init(key, strlen((const char*)key), iv, strlen((const char*)iv));

    const int cbufMax = 1024;
    char cbufClear[cbufMax];
    char cbufEnc[cbufMax];

    memset(cbufClear, 0, sizeof(cbufClear));
    strcpy(cbufClear, bgMsg);

    memset(cbufEnc, 0, sizeof(cbufEnc));
    zlib_deflate((const uint8_t*)cbufClear, strlen(cbufClear), (uint8_t*)cbufEnc, cbufMax);

    crypto_encrypt(ctx, (const uint8_t*)cbufClear, strlen(cbufClear), (uint8_t*)cbufEnc, cbufMax);
    int x = 0;
    memset(cbufClear, 0, sizeof(cbufClear));
    crypto_decrypt(ctx, (const uint8_t*)cbufEnc, cbufMax, (uint8_t*)cbufClear, cbufMax);

    crypto_destory(ctx);
#endif

    //printf("pnet server: starting now ...\n");
#if 0
    json jsonData;
    json params;
    json actions;
    params["mNextTick"] = 30.0f;
    int actionCnt = 3;
    params["actionCnt"] = actionCnt;
    for (int i = 0; i < actionCnt; i++)
    {
        json action;
        action["name"] = "action_name";
        action["param1"] = "action_param";
        actions += action;
    }

    jsonData += params;
    jsonData += actions;

    const size_t bufMax = 2048;
    char buf[bufMax];
    buf[0] = 0;
    base64_encode(jsonData.dump().c_str(), bufMax, buf);
    printf("%s -> %s\n", jsonData.dump().c_str(), buf);
#endif

    return 0;
}
