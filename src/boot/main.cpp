
// Project Net
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "../pnet/helpers.h"

#include "main.h"

using namespace std;


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

    string username = "master";
    string sessionKey = string("0123456789abcdef0123456789abcdef") + string("0123456789abcdef");

    // start new request
    Request::Begin();

    // auth action
    json jactionAuth;
    jactionAuth["action"] = "auth";
    jactionAuth["username"] = username;
    jactionAuth["userEnc"] = Encode::EncodeBase64(username);
    jactionAuth["passwordEnc"] = Encode::EncodeBase64("kkxrd");
    jactionAuth["sessionKey"] = Encode::EncodeBase64(sessionKey);
    Request::Add(jactionAuth, 1001);

    // tick action
    unsigned long long sessionToken = 55555;
    json jactionTick;
    jactionTick["action"] = "tick";
    jactionTick["sessionToken"] = sessionToken;
    jactionTick["payload"] = Encode::EncodeBase64("ALL");
    Request::Add(jactionTick, 1002);

    // finish request string
    string param = Request::End();
    Request::Decode(param);

#if 0
    static const size_t bufMax = 1024;
    static uint8_t buf0[bufMax];
    static size_t buf0Len = 0;
    static uint8_t buf1[bufMax];
    static size_t buf1Len = 0;
    memset(buf0, 0, sizeof(buf0));
    memset(buf1, 0, sizeof(buf1));



    // make action list

    const unsigned char key[] = "0123456789abcdef0123456789abcdef"; // 32 bytes for AES-256
    const unsigned char iv[] = "0123456789abcdef";                   // 16 bytes for AES block size

    string keyPub, keyPri;
    keyPub = "LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0KTUlJQklqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FROEFNSUlCQ2dLQ0FRRUF4UGYvck0zbFowc0dTdEttaXJqUApuRjVCSzk0ZVVHU3pBU2tYNE83YzdBb1diUjBYamhJYi9ielN5WGUxVlo4aFg1V0dGMGZlejNObk1ZOG1JTzZqCi9hdzdZNmo3clZMd25tc0N5T1ptZTNrSU5SeHZIOHp1cjRlL1VEUGFBR0pud2UyUklmWjBnTmtnYU5Jb09nSkUKVmltVG8yMmJEMkJjS0FxcGprKzJPMGFQVEw3NzZySGlhZHBZcEdCUTIrWGsvcEhkeG9aTEZkczhTdFcrbnRYSApDdHUza3JNaldSTHRLUDlKVnRmVFZFQTdKRlpYL2NzMHFKbEVlZHBhOVljWm4xc0Y0UGd2OEw1SVZuYSt5TWFmCmdBUmU5SEJYaTlzVDNzVFBveDlub2NUYzVtV0tMYlBlaWVVa3QyNDZuYzNzdVQwbXNWVGlQcHovYzdDWGlsd0QKOVFJREFRQUIKLS0tLS1FTkQgUFVCTElDIEtFWS0tLS0tCg==";
    keyPri = "LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFb3dJQkFBS0NBUUVBeFBmL3JNM2xaMHNHU3RLbWlyalBuRjVCSzk0ZVVHU3pBU2tYNE83YzdBb1diUjBYCmpoSWIvYnpTeVhlMVZaOGhYNVdHRjBmZXozTm5NWThtSU82ai9hdzdZNmo3clZMd25tc0N5T1ptZTNrSU5SeHYKSDh6dXI0ZS9VRFBhQUdKbndlMlJJZlowZ05rZ2FOSW9PZ0pFVmltVG8yMmJEMkJjS0FxcGprKzJPMGFQVEw3Nwo2ckhpYWRwWXBHQlEyK1hrL3BIZHhvWkxGZHM4U3RXK250WEhDdHUza3JNaldSTHRLUDlKVnRmVFZFQTdKRlpYCi9jczBxSmxFZWRwYTlZY1puMXNGNFBndjhMNUlWbmEreU1hZmdBUmU5SEJYaTlzVDNzVFBveDlub2NUYzVtV0sKTGJQZWllVWt0MjQ2bmMzc3VUMG1zVlRpUHB6L2M3Q1hpbHdEOVFJREFRQUJBb0lCQUFmbWNsVGlGSmdNYU00SQplaWJNbElKSUJxTUxFdzlwcEZpWjJSRklZWlpoMklmZkVsaURZZjFBeDhFamVSVnFlQWxnUVZNMERpYTFab2VVCjJEZlQwYVh1TzlQTWJabVNsOGNWaHUxWVAvYlJUT0t5UE1QbUNtZXM4UisrT3VSRWYrL2lReTJ2RFJZMDBRZXYKdDRyL05DdEJOdUN1ME00dElxTlpud1VNTGYxNXplQ2gxMnhtSVByWndGSkhCQU1kbzNyNy9MUmlUZHgyVElGcwp2U21MbmcvSnV1MjdLemFkNDNRZWROdDdmUnNFbGhndHdlbHczK2RYK0l3aEdWMmJROXNGd1djSGVEWHJkbHZ1ClZycXZvSXpmY21uMEZRSjdmYXBjRDlCZ082VDBnSEk3S05objUwbXovQkdHTC92cUJDWDI1OUloQ21Eek5HYjYKOVNQaGNhTUNnWUVBOTZPeHJFVHFlSkYrTGxiOUlVQ21kc1B3QXI3eElNRHRkYXpxbG1uWXI5N1BDMXBFcGxPcQpUM3hqWHdXN1did2ZIMTNzL0JxaFVXS2paWExLZGdvMGhwZ2VqZkpNeG83T1FEbS9sSlprV2s4c2EySDhTdXZoCnNLWm1IYy9aellxL3RROXRFV205RTFRY2pXWlU0UXVoTFNtbDcyYzBsYzB4ZmRjejJ1aFYwck1DZ1lFQXk1NWQKMGU2SGpHQ1VtaVYxRGt3UTQvamhpOTZoSURjVVdHb3JnY3l4MjNXc3dyRzNPazJNWWpObncrMzNsby9LQ3JlQwpWS0pXeWUraTliUkp3VTJmOXc2NEVFRTI2bHZ2eklaLzI5MnNrbXBmeWFxUmlsYU1MdFZudFRxckV5SHd6TWpjClE3Z3k3SWk2VFd5U0RDeVBMb0x1SC9ZWS83L2RYL05TN2J1dkFyY0NnWUFoL0NNTUEwRXlYclUwWXRmR1Vuc0YKM1luNHRuK2RBUjBWZ3krR2lPd2w1cXYvWU5uL0FZaGo5Q3duQkVCYjVrZFhKS3E3Um1SUktQcEhsK05WSTcxbApvMGErTUR4OUcyZ094RjRZWkROTmVZUWRFSXR4Tk1INlhqdXhaNjc2V0VwSzdOMEpBTHZrQlMwTjhyYy9VenRCCldXL2VWUW1RYldLWFpJdEhSWTlScFFLQmdRQ1F3NTBzWGd2Y3orV1cyajZvMVlmRXJQT2VQOEFLYVY1N3BiQWMKN20wYy9FTEJnUndseFhDamVLeXpiZ0p5OXZQTVByL1ljRHFJQTJtMmltRk5uMlMxZjY5NVR3UStCdk1zVkF1ZgpRN0VudjUwdWdhaEwxRkNDRTVOU0U4NEJJa1pUTWhMM3lvUGFDek41VEZGaGVtU3B5c2lGeksvbFZJR05jQzlyCmVkNXBiUUtCZ0hOUmN0SzZFNUY1d0RaZkhkVDdLMjZNRjJ5RDdnMzEyYkxoVjBxRGtadVZVNzdwd2ROS1Blb2MKNDBld05yU1R2c3A0T2ZEWkF2b1ZTTU9jZUNRWGNWQ1d6d0E2cUtwWXBvSGlXVWZpZGp0ZkNSSXRCcjBJaXB4VQpVVVE4UEdGNU5ET1kwNkdVek9IK3gybU8wMERwMzBXVEtpRSt6UHhTNmU2SkFFVFNHbUVuCi0tLS0tRU5EIFJTQSBQUklWQVRFIEtFWS0tLS0tCg==";
    //CryptPair::New(keyPub, keyPri);
    //printf("\n%s\n", keyPub.c_str());
    //printf("\n%s\n", keyPri.c_str());
    strcpy((char*)buf0, "This is a test message.");
    buf0Len = strlen((char*)buf0) + 1;
    CryptPair::Init(keyPub, "");
    buf1Len = CryptPair::Encrypt(buf0, buf0Len, buf1, bufMax);
    int x = 0;
    memset(buf0, 0, sizeof(buf0));
    CryptPair::Init("", keyPri);
    buf0Len = CryptPair::Decrypt(buf1, buf1Len, buf0, bufMax);


    // auth action
    string username = "master";
    string sessionKey = string("0123456789abcdef0123456789abcdef") + string("0123456789abcdef");
    json jactionAuth;
    jactionAuth["action"] = "auth";
    jactionAuth["username"] = username;
    jactionAuth["userEnc"] = Encode::EncodeBase64(username);
    jactionAuth["passwordEnc"] = Encode::EncodeBase64("kkxrd");
    jactionAuth["sessionKey"] = Encode::EncodeBase64(sessionKey);

    // tick action
    unsigned long long sessionToken = 1001;
    json jactionTick;
    jactionTick["action"] = "tick";
    jactionTick["sessionToken"] = sessionToken;
    jactionTick["payload"] = Encode::EncodeBase64("ALL");

    // build action list
    json jactions;
    jactions += jactionAuth;
    jactions += jactionTick;

    printf("action list:\n%s", jactions.dump().c_str());

    // compress and encode json
    std::string jsonStr = JSON::CompressAndEncode(jactions);
    json jtest = JSON::DecodeAndUncompress(jsonStr);    
    jtest = json::parse(jactions.dump());
    if (!jtest.is_null() && jtest.is_array() && jtest.size() > 0)
    {
        for (auto it = jtest.cbegin(); it != jtest.cend(); it++)
        {
            json jobj = *it;
            string actionName = jobj.value("action", "");
            printf("Action [%s]: %s\n", actionName.c_str(), jobj.dump().c_str());
        }
    }
#endif
#if 0
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
#endif

    Crypt::Destroy();
    CryptPair::Destroy();

#ifdef WINDOWS
    printf("Press any key to exit.\n");
    _getch();
#endif
    return 0;
}
