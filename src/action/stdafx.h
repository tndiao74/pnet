// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <io.h>
//#include <format>
#include <string>
#include <exception>

using namespace std;

#include <stdio.h>

// AWS
//#include <aws/core/Aws.h>
//#include <aws/s3/S3Client.h>
//#include <aws/core/auth/AWSCredentialsProviderChain.h>
//#include <aws/s3/model/Bucket.h>

//using namespace Aws;
//using namespace Aws::Auth;

// 


// cgicc
// ignore header warnings in vc
#if WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4083)
#endif

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#if WINDOWS
#pragma warning(pop)
#endif

using namespace cgicc;


// windows only includes
#ifdef WINDOWS
#include <crtdbg.h>
#include <conio.h>
#endif
