
// Project Net
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

// get build working in linux
// move to client
// static link curl in linux and run in ramfs


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

    printf("pnet server: starting now ...\n");

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


#ifdef WINDOWS
    printf("Press any key to exit.\n");
    _getch();
#endif
    return 0;
}
