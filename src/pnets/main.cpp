
// Project Net
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "../pnet/log.h"
#include "../pnet/helpers.h"
#include "../pnet/core.h"

#include "main.h"

using namespace std;


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

const size_t bufMax = 8192;
uint8_t buf0[bufMax];
size_t buf0Len = 0;
uint8_t buf1[bufMax];
size_t buf1Len = 0;

void requestDecodeEvent(const json& j, uint16_t code)
{
    _log("\ncode: %d\njson\n%s\n\n", code, j.dump().c_str());

    // auth event
    if (code == 1001 && j.value("action", "") == "auth")
    {
        string username = j.value("username", "");
        if (username.length() > 0)
        {
            // look up the user in the db
            json jdb = DB::sql_select("ID username passhash privatekey", "user", _szprintf("username='%s'", username.c_str()));
            if (jdb.size() > 0 && jdb.value("username", "").length() > 0)
            {
                string keyPri = jdb.value("privatekey", "");
                string dbpasshash = jdb.value("passhash", "");
                string sessionKeyEnc = j.value("sessionKeyEnc", "");
                string passwordEnc = j["passwordEnc"];

                // unencrypt the session key
                string sessionKey = Request::DecodeAndDecryptPair(keyPri, sessionKeyEnc);

                // unencrypt password with session key
                string password = Request::DecodeAndDecrypt(sessionKey, passwordEnc);

                _log("\n%s\n\n", password.c_str());

                // hash password
                string passhash = Crypt::SHA1Hash(password);
                _log("passhash\n%s\n\n", passhash.c_str());

                if (passhash.length() > 0 && passhash == dbpasshash)
                {
                    _log("Access Granted.");
                }
                else
                {
                    _log("Access Denied.");
                }
#if 0
                // get session key bytes
                buf0Len = Encode::DecodeBase64((uint8_t*)sessionKeyEnc.c_str(), sessionKeyEnc.length(), buf0, bufMax);

                // unencrypt key bytes with out private key
                CryptPair::Init("", keyPri);
                memset(buf1, 0, sizeof(buf1));
                buf1Len = CryptPair::Decrypt((uint8_t*)buf0, buf0Len, buf1, bufMax);

                // unencrypt password with session key
                Crypt::Init((unsigned char*)&buf1[0], 32, (unsigned char*)&buf1[32], 16);

                // grab encrypted bytes
                buf0Len = Encode::DecodeBase64((uint8_t*)passwordEnc.c_str(), passwordEnc.length(), buf0, bufMax);
                buf1Len = Crypt::Decrypt(buf0, buf0Len, buf1, bufMax);
                buf1[buf1Len] = 0;
                string passwordClear = (const char*)buf1;
#endif
                

                
            }
            else
            {
                _log("username %s not found.", username.c_str());
            }
        }
    }
}

int main(int argc, char* argv[])
{
    // debug memory usage
#ifdef WINDOWS
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 1959565;
#endif
#if 0
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

    const std::string input_string = "kkxrd";
    unsigned char hash[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH is 20 bytes

    // Calculate the hash
    SHA1(reinterpret_cast<const unsigned char*>(input_string.c_str()),
        input_string.length(),
        hash); // The hash is placed into the 'hash' array

    // Convert the hash to a hexadecimal string for output
    std::cout << "SHA1 hash: ";
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    std::cout << std::endl;

    json jdb = DB::sql_select("ID username passhash", "user", "ID IS NOT NULL");
    std::string username = jdb.value("username", "");
#endif
    //std::string System::exec(const char* cmd)
#if 0
    if (driver != nullptr)
    {
        try
        {
            sql::Connection* con(driver->connect("tcp://127.0.0.1:3306", "root", "kkxrd"));

            if (con != nullptr)
            {
                // Connect to the specific database schema
                con->setSchema(database);
                cout << "Connection established." << endl;

                // Create a statement object
                std::unique_ptr<sql::Statement> stmt(con->createStatement());

                // Execute a query and store the results in a ResultSet object
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));

                // Iterate over the result set and print the result
                while (res->next()) {
                    // Access data by column name
                    cout << "Result message: " << res->getString("_message") << endl;
                }
            }
        }
        catch (exception e)
        {
            printf("%s", e.what());
        }
        
        return 0;
    }
#endif

#if 0
        // Use a unique_ptr to manage the connection object lifecycle
        std::unique_ptr<sql::Connection> con(driver->connect(url, user, pass));

        // Connect to the specific database schema
        con->setSchema(database);
        cout << "Connection established." << endl;

        // Create a statement object
        std::unique_ptr<sql::Statement> stmt(con->createStatement());

        // Execute a query and store the results in a ResultSet object
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));

        // Iterate over the result set and print the result
        while (res->next()) {
            // Access data by column name
            cout << "Result message: " << res->getString("_message") << endl;
        }
#endif

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

        Request::Decode(input, requestDecodeEvent);
        
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
