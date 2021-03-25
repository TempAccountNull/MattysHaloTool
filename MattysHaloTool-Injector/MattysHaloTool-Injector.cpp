// ReSharper disable CppUseAuto
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>

//Globals
bool g_close_on_inject;

void clear_screen()
{
	HANDLE                     h_std_out;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cell_count;
	COORD                      home_coords = { 0, 0 };

	h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	if (h_std_out == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(h_std_out, &csbi)) return;
	cell_count = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		h_std_out,
		static_cast<TCHAR>(' '),
		cell_count,
		home_coords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		h_std_out,
		csbi.wAttributes,
		cell_count,
		home_coords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(h_std_out, home_coords);
}

DWORD get_proc_id(const char* proc_name)
{
	DWORD proc_id = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (h_snap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof(proc_entry);

		if (Process32First(h_snap, &proc_entry))
		{
			do
			{
				if (!_stricmp(reinterpret_cast<char const*>(proc_entry.szExeFile), proc_name))
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(h_snap, &proc_entry));
		}
	}
	CloseHandle(h_snap);
	return proc_id;
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
	std::string full_path_of_exe = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	std::string stripped_path = full_path_of_exe.substr(0, full_path_of_exe.find_last_of("\\/"));
	return stripped_path;
}

std::string get_current_exe_name(bool include_file_extension = false)
{
	char result[MAX_PATH];
	std::string full_path_of_exe = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	
	size_t index = full_path_of_exe.find_last_of("\\/");
	std::string file_with_ext = full_path_of_exe.substr(index + 1);

	if(!include_file_extension)
	{
		std::string file_without_ext = file_with_ext.substr(0, file_with_ext.find_last_of('.'));
		return file_without_ext;
	}
	
	
	return file_with_ext;
}

void inject(std::string dll_path, DWORD proc_id)
{
	const char* dll_filename = dll_path.c_str();
	HANDLE h_proc = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);

	if (h_proc && h_proc != INVALID_HANDLE_VALUE)
	{
		void* loc = VirtualAllocEx(h_proc, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(h_proc, loc, dll_filename, strlen(dll_filename) + 1, nullptr);

		HANDLE h_thread = CreateRemoteThread(h_proc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), loc, 0, nullptr);

		if (h_thread)
		{
			CloseHandle(h_thread);
		}
	}

	if (h_proc)
	{
		CloseHandle(h_proc);
	}
}

void create_config(LPCSTR ini_path)
{
	WritePrivateProfileString("Super Cool Injector", "closeoninject", "true", ini_path);
}


void read_config()
{
	std::string ini_path = get_current_working_dir() + "\\" + get_current_exe_name(false) + ".ini";
	
	if (file_exists(ini_path))
	{
		
		char closeoninject_string[6];
		GetPrivateProfileString("Super Cool Injector", "closeoninject", "true", closeoninject_string, 6, ini_path.c_str());

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
		create_config(ini_path.c_str());
	}
}

int main()
{

	//Read/Write Config
	read_config();
	
	// Init variables
	const char* proc_name = "MCC-Win64-Shipping.exe";
	DWORD proc_id = 0;

	// Get path to dll.

	const char* dll_name = "\\DLL.dll";
	std::string dll_path = get_current_working_dir() + dll_name;


	
	// Main code

	while(true)
	{
		
		if (file_exists(dll_path))
		{
			clear_screen();
			std::cout << "Looking for MCC.." << std::endl;

			while (!proc_id)
			{
				proc_id = get_proc_id(proc_name);
				Sleep(5000);
			}

			clear_screen();
			std::cout << "Injecting DLL." << std::endl;
			inject(dll_path, proc_id);
		}
		else
		{
			clear_screen();
			std::cout << dll_path.c_str() << " is missing. Cannot inject." << std::endl;
			std::cin.get();
			exit(1);
		}

		if (g_close_on_inject)
		{
			break;
		}

		// Dont do anything until program is closed.
		while (proc_id)
		{
			proc_id = get_proc_id(proc_name);
			Sleep(5000);
		}
	}


	return 0;
}
