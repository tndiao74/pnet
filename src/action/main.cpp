
// s3demo
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "main.h"


///////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////

FILE* gLogFP = 0;



///////////////////////////////////////////////////////////////////////
// Internal Prototypes
///////////////////////////////////////////////////////////////////////

void ProcessForm(Cgicc& cgi);
void Log(const string& msg);



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

    Log("Hello Cgicc!");

    try
    {
        Cgicc cgi;

        // Send HTTP header
        cout << HTTPHTMLHeader() << endl;
        cout << html() << endl;
        cout << head() << endl;
        cout << title() << "Action CGI" << title() << endl;
        cout << head() << endl;
        cout << body() << endl;

        cout << "This is not a good base library to build off of ..." << endl;

        // Close the HTML document
        cout << body() << html();

        //ProcessForm(cgi);
    }
    catch (exception& e) {
        // Caught a standard library exception
        //Log(std::format("Exception: %s\n", e.what()));
    }

    Log("Request Finished");
    Log("");
#if false
    if (gLogFP != 0)
    {
        fclose(gLogFP);
    }
#endif
    return 0;
}

void ProcessForm(Cgicc& cgi)
{
#if false
    if (cgi.getElements().size() > 0)
    {
        cgicc::form_iterator nameItor = cgi.getElement("name");
        if (nameItor != cgi.getElements().end())
        {
            string name = cgi("name");
            Log(std::format("name = %s\n", name.c_str()));
        }
        else
        {
            Log("Field Not Found.\n");
        }
    }
    else
    {
        Log("Form Not Found.\n");
    }
#endif
}

void Log(const string& msg)
{
#if 0
    // open log file
    if (gLogFP == 0)
    {
        gLogFP = fopen("log.txt", "w");
        if (gLogFP == 0)
            return;
    }
#if false
    // timestamp
    time_t rawtime;
    time(&rawtime);
    struct tm* info;
    info = localtime(&rawtime);
    static char strTime[80]; // Allocate a buffer for the formatted string
    strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", info);

    // build message
    static const int msgBufMax = 2048;
    static char msgBuf[msgBufMax];
    sprintf_s(msgBuf, msgBufMax, "%s %s\n", strTime, msg.c_str());

    // write message
    fprintf(gLogFP, msgBuf);
#endif

    fprintf(gLogFP, "msgBuf");
#endif
}
