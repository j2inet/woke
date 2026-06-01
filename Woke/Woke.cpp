// Woke.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <string>

const int INDEFINITE_WAKE_TIME = -1;
struct Options
{
    int wakeTime = -1;
    bool verbose = false;

};

void ParseArguments(int argc, wchar_t* argv[], Options& options)
{
    
    for (int i = 0; i < argc; ++i)
    {
        std::wstring arg = argv[i];
        auto nextArg = [&]() -> std::wstring {
            if (i + 1 < argc)
            {
                return argv[i + 1];
            }
            return L"";
		};
        
		if ((arg == L"-w" || arg == L"--wake-time") && !nextArg().empty())
        {
            options.wakeTime = std::stoi(nextArg());
            ++i; // Skip the next argument since it's already processed
        }
		else if ((arg == L"-v") || (arg == L"--verbose"))
        {
            options.verbose = true;
        }
		else if ((arg == L"-s") || (arg == L"--silent"))
        {
            options.verbose = false;
        }
		else if ((arg == L"-i") || (arg == L"--indefinite"))
        {
            options.wakeTime = INDEFINITE_WAKE_TIME;
        }
        else if ((arg == L"-a")|| (arg == L"--about") || (arg == L"-h") || (arg == L"--help"))
        {
            std::wcout << L"Woke - A utility to prevent the system from sleeping.\n"
                << L"Usage:\n"
                << L"  -w, --wake-time <seconds>   Set a specific wake time in seconds.\n"
                << L"  -i, --indefinite             Keep the system awake indefinitely.\n"
                << L"  -a, --about, -h, --help      Display this information.\n";
            exit(0);
		}

    }
}
static std::atomic<bool> keepRunning{ true };
HANDLE waitEvent = NULL;


BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        keepRunning = false;
		SetEvent(waitEvent); // Signal the main thread to exit
        return TRUE;
    default:
        return FALSE;
    }
}


int wmain(int argc, wchar_t* argv[])
{
    Options runOptions;
    ParseArguments(argc, argv, runOptions);
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);

    waitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD waitTime = (runOptions.wakeTime == INDEFINITE_WAKE_TIME) ? INFINITE : runOptions.wakeTime * 1000;
    if(runOptions.verbose)
    {
        std::wcout << L"System will be kept awake for " << (waitTime == INFINITE ? L"indefinitely" : std::to_wstring(waitTime / 1000) + L" seconds") << L". Press Ctrl+C to exit." << std::endl;
	}
	WaitForSingleObject(waitEvent, waitTime);
	CloseHandle(waitEvent);
    SetThreadExecutionState(ES_CONTINUOUS);    
	if (runOptions.verbose)
    {
        std::wcout << L"Woke has exited. System can now sleep." << std::endl;
    }
}

