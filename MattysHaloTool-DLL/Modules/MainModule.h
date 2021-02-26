#pragma once
#include <chrono>
#include <vector>
#include <string>

#include "../console.h"

class MainModule : public Console::Console::Command
{
public:
	virtual void Tick(const std::chrono::duration<double>& DeltaTime) = 0;
	virtual ~MainModule() = default;
private:
};