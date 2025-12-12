// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// nlohmann json
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

// lib curl
#include <curl/curl.h> // Include the libcurl header

// windows only includes
#ifdef WINDOWS
#include <crtdbg.h>
#include <conio.h>
#endif
