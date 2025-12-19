
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
#if 1
std::string requestDecodeEvent(const json& j, uint16_t code)
{
    _log("\ncode: %d\njson\n%s\n\n", code, j.dump().c_str());

    std::string response = "";

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

                // hash password
                string passhash = Crypt::SHA1Hash(password);
                
                // check for login
                if (passhash.length() > 0 && passhash == dbpasshash)
                {
                    response = "Access Granted.";

                }
                else
                {
                    response = "Access Denied.";
                }
            }
            else
            {
                response = _szprintf("username %s not found.", username.c_str());
            }
        }
    }

    return response;
}

void ListBuckets(const Aws::S3::S3Client& s3Client);
void UploadFile(const Aws::S3::S3Client& s3Client, const std::string uploadFile, const std::string bucketName, const std::string objectKey);
void DownloadFile(const Aws::S3::S3Client& s3Client, const std::string saveFile, const std::string bucketName, const std::string objectKey);
const char* MakeGUID(char* outGuidStr, int outGuidStrMax);
#endif

int main(int argc, char* argv[])
{
    // debug memory usage
#ifdef WINDOWS
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 1959565;
#endif

    _log("pnet server invoke instance");

    //The Aws::SDKOptions struct contains SDK configuration options.
    //An instance of Aws::SDKOptions is passed to the Aws::InitAPI and 
    //Aws::ShutdownAPI methods. The same instance should be sent to both methods.
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    Aws::InitAPI(options);

    // access token
    _putenv("AWS_ACCESS_KEY_ID=AKIASPGS5NSIFAYZGKGT");
    _putenv("AWS_SECRET_ACCESS_KEY=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");

    // You don't normally have to test that you are authenticated. But the S3 service permits anonymous requests
    // thus the s3Client will return "success" and 0 buckets even if you are unauthenticated, which can be confusing to a new user.
    std::shared_ptr<Aws::Auth::DefaultAWSCredentialsProviderChain> provider = Aws::MakeShared<Aws::Auth::DefaultAWSCredentialsProviderChain>("alloc-tag");
    Aws::Auth::AWSCredentials creds = provider->GetAWSCredentials();
    if (creds.IsEmpty())
    {
        printf("Authentication failed.\n");
        Aws::ShutdownAPI(options);
        _getch();
        return -1;
    }

    printf("\nAWSAccessKeyId = %s.\n", creds.GetAWSAccessKeyId().c_str());

    // init client
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = "us-west-1"; // region bucket is in
    Aws::S3::S3Client s3Client(clientConfig);

    // list buckets
    ListBuckets(s3Client);

    printf("Content-type: text/html\n\n"); // Required HTTP header

    const char* queryString = std::getenv("QUERY_STRING");

    if (queryString != nullptr)
    {
        std::string input(queryString);
        
        std::string response = Request::Decode(input, requestDecodeEvent);

        printf(gServerResponse, response.c_str());
        
        // Note: The input string needs URL decoding and parsing into key-value pairs
    }
    else
    {
        printf("<p>No QUERY_STRING environment variable found.</p>");
        _log("No QUERY_STRING environment variable found.");
    }

    // destroy
    Aws::ShutdownAPI(options);

    return 0;
}

#if 1
void ListBuckets(const Aws::S3::S3Client& s3Client)
{
    // list buckets
    Aws::S3::Model::ListBucketsOutcome outcome = s3Client.ListBuckets();
    if (!outcome.IsSuccess())
    {
        printf("Failed with error: %s\n", outcome.GetError().GetMessage().c_str());
    }
    else
    {
        printf("Found %d buckets.\n", (int)outcome.GetResult().GetBuckets().size());
        for (auto& bucket : outcome.GetResult().GetBuckets())
        {
            printf("Name: %s\n", bucket.GetName().c_str());
        }
    }
}

void UploadFile(const Aws::S3::S3Client& s3Client, const std::string uploadFile, const std::string bucketName, const std::string objectKey)
{
    // try and open the upload file
    std::shared_ptr<Aws::FStream> fileStream = Aws::MakeShared<Aws::FStream>("MyAllocationTag", uploadFile, std::ios_base::in | std::ios_base::binary);
    if (fileStream->is_open())
    {
        // build a put request
        Aws::S3::Model::PutObjectRequest reqPutObj;
        reqPutObj.SetBucket(bucketName);
        reqPutObj.SetKey(objectKey);
        reqPutObj.SetBody(fileStream);

        // execute request
        Aws::S3::Model::PutObjectOutcome outcome = s3Client.PutObject(reqPutObj);
        if (!outcome.IsSuccess())
        {
            printf("Error: PutObject: %s\n", outcome.GetError().GetMessage().c_str());
        }
        else
        {
            printf("Added object with key %s to bucket %s.\n", objectKey.c_str(), bucketName.c_str());
        }
    }
    else
    {
        // failed to open upload file
        printf("Error: unable to open file, '%s'.\n", uploadFile.c_str());
    }
}

void DownloadFile(const Aws::S3::S3Client& s3Client, const std::string saveFile, const std::string bucketName, const std::string objectKey)
{
    // build a get request
    Aws::S3::Model::GetObjectRequest reqGetObj;
    reqGetObj.SetBucket(bucketName);
    reqGetObj.SetKey(objectKey);

    // execute request
    Aws::S3::Model::GetObjectOutcome outcome = s3Client.GetObject(reqGetObj);
    if (!outcome.IsSuccess())
    {
        printf("Error: GetObject: %s\n", outcome.GetError().GetMessage().c_str());
    }
    else
    {
        Aws::IOStream& ioStream = outcome.GetResultWithOwnership().GetBody();
        Aws::OFStream outStream(saveFile, std::ios_base::out | std::ios_base::binary);
        if (!outStream.is_open())
        {
            printf("Error: unable to open file, '%s'.\n", saveFile.c_str());
        }
        else {
            outStream << ioStream.rdbuf();
            printf("Downloaded object with key %s from bucket %s to file %s.\n", objectKey.c_str(), bucketName.c_str(), saveFile.c_str());
        }
    }
}

const char* MakeGUID(char* outGuidStr, int outGuidStrMax)
{
    GUID guid;
    HRESULT hr = CoCreateGuid(&guid);
    if (outGuidStrMax >= 39) // 36 characters for GUID + 2 for hyphens + 1 for null terminator
    {
        sprintf_s(outGuidStr, outGuidStrMax,
            "%08lX-%04hX-%04hX-%02X%02X-%02X%02X%02X%02X%02X%02X",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
    }
    return outGuidStr;
}
#endif