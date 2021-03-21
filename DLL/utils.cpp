#include "pch.h"
#include "utils.h"


// Misc Imports
EXTERN_C IMAGE_DOS_HEADER __ImageBase;


std::string utils::locations::GetCurrentWorkingDir()
{
    char result[MAX_PATH];
    std::string fullpathofdll = std::string(result, GetModuleFileName((HINSTANCE)&__ImageBase, result, MAX_PATH));
    std::string strippedpath = fullpathofdll.substr(0, fullpathofdll.find_last_of("\\/"));
    return strippedpath;
}