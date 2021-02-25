#pragma once
#include <map>

#include "console.h"

Console::Console Terminal;
std::map<std::string, std::shared_ptr<ElModule>> Commands;