
// s3demo
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "../pnet/base64.h"

#include "main.h"


///////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////


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

    printf("pnet client: starting now ...\n");
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a curl easy handle
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, "https://anmccall.net");
#if WINDOWS
        curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
#endif

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up the easy handle
        curl_easy_cleanup(curl);
    }
    
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
    
    curl_global_cleanup();

#ifdef WINDOWS
    printf("Press any key to exit.\n");
    _getch();
#endif
    return 0;
}
