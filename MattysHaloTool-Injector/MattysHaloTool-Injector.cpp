#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>

DWORD GetProcId(const char* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_stricmp(reinterpret_cast<char const*>(procEntry.szExeFile), procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

bool file_exists(std::string filename)
{
    FILE* file = nullptr;
    if (fopen_s(&file, filename.c_str(), "r") == EINVAL, file)
    {
        fclose(file);
        return true;
    }

    return false;
}

std::string GetCurrentWorkingDir()
{
    char result[MAX_PATH];
    std::string fullpathofexe = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
    std::string strippedpath = fullpathofexe.substr(0, fullpathofexe.find_last_of("\\/"));
    return strippedpath;
}

void inject(std::string dllPath, DWORD procId)
{
    const char* DllFilename = dllPath.c_str();
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE)
	{
		void* loc = VirtualAllocEx(hProc, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(hProc, loc, DllFilename, strlen(DllFilename) + 1, nullptr);

		HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	if (hProc)
	{
		CloseHandle(hProc);
	}
}

int main()
{
	// Init variables
	const char* procName = "MCC-Win64-Shipping.exe";
    DWORD procId = 0;

	// Get path to dll.

	const char* DllName = "\\DLL.dll";
    std::string dllPath = GetCurrentWorkingDir() + DllName;

	// Main code
	
    std::cout << "Looking for MCC.. \n";
	
    while (!procId)
    {
        procId = GetProcId(procName);
        Sleep(30);
    }

	if( file_exists(dllPath))
	{
        std::cout << "Injecting DLL. \n";
        inject(dllPath, procId);
	}
    else
    {
        std::string errormsg(dllPath);
        errormsg.append(" is missing. Cannot inject. \n");
	    std::cout << errormsg;
        system("pause");
    }

    return 0;
}
