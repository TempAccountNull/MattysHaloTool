#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>

//Globals
bool g_close_on_inject;

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

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

std::string get_current_working_dir()
{
	char result[MAX_PATH];
	std::string fullpathofexe = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	std::string strippedpath = fullpathofexe.substr(0, fullpathofexe.find_last_of("\\/"));
	return strippedpath;
}

std::string get_current_exe_name(bool include_file_extension = false)
{
	char result[MAX_PATH];
	std::string fullpathofexe = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	
	size_t index = fullpathofexe.find_last_of("\\/");
	std::string file_with_ext = fullpathofexe.substr(index + 1);

	if(!include_file_extension)
	{
		std::string file_without_ext = file_with_ext.substr(0, file_with_ext.find_last_of("."));
		return file_without_ext;
	}
	
	
	return file_with_ext;
}

void inject(std::string dllPath, DWORD procId)
{
	const char* DllFilename = dllPath.c_str();
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE)
	{
		void* loc = VirtualAllocEx(hProc, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(hProc, loc, DllFilename, strlen(DllFilename) + 1, nullptr);

		HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), loc, 0, nullptr);

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

void create_config(LPCSTR iniPath)
{
	WritePrivateProfileString("Super Cool Injector", "closeoninject", "true", iniPath);
}


void read_config()
{
	std::string iniPath = get_current_working_dir() + "\\" + get_current_exe_name(false) + ".ini";
	
	if (file_exists(iniPath))
	{
		
		char closeoninject_string[6];
		GetPrivateProfileString("Super Cool Injector", "closeoninject", "true", closeoninject_string, 6, iniPath.c_str());

		//TODO: Ugly code, there is probably a better way of doing this without using third-party libs.
		
		if (!_stricmp(closeoninject_string, "true") || !_stricmp(closeoninject_string, "false"))
		{
			//Valid Config

			if(!_stricmp(closeoninject_string, "true"))
			{
				g_close_on_inject = true;
			}
			else
			{
				g_close_on_inject = false;
			}
			
		}
		else
		{
			std::cout << "Invalid Config, Please delete it and let the program make a new one." << std::endl;
			std::cin.get();
			exit(1);
		}

	}
	else
	{
		create_config(iniPath.c_str());
	}
}

int main()
{

	//Read/Write Config
	read_config();
	
	// Init variables
	const char* procName = "MCC-Win64-Shipping.exe";
	DWORD procId = 0;

	// Get path to dll.

	const char* DllName = "\\DLL.dll";
	std::string dllPath = get_current_working_dir() + DllName;


	
	// Main code

	while(true)
	{
		
		if (file_exists(dllPath))
		{
			ClearScreen();
			std::cout << "Looking for MCC.." << std::endl;

			while (!procId)
			{
				procId = GetProcId(procName);
				Sleep(5000);
			}

			ClearScreen();
			std::cout << "Injecting DLL." << std::endl;
			inject(dllPath, procId);
		}
		else
		{
			ClearScreen();
			std::cout << dllPath.c_str() << " is missing. Cannot inject." << std::endl;
			std::cin.get();
			exit(1);
		}

		if (g_close_on_inject)
		{
			break;
		}

		// Dont do anything until program is closed.
		while (procId)
		{
			procId = GetProcId(procName);
			Sleep(5000);
		}
	}


	return 0;
}
