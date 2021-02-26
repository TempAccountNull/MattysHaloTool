#pragma once
#include <map>

#include "console.h"
#include "Modules/MainModule.h"
#include "Utils/singleton.h"

class Main : public Utils::Singleton< Main >
{
public:

	std::string GetDirectory();

	void Tick(const std::chrono::duration<double>& DeltaTile);

	void SetSessionMessage(const std::string& Message);

	static void SetMainThreadID(size_t ThreadID)
	{
		MainThreadID = ThreadID;
	}
	static size_t GetMainThreadID()
	{
		return MainThreadID;
	}

	template<class T,
		class = typename std::enable_if<std::is_base_of<MainModule, T>::value>::type>
		void PushModule(const std::string& Name)
	{
		std::shared_ptr<T> Module = std::make_shared<T>();
		Terminal.PushCommand(Name, Module);
		Commands[Name] = Module;
	}

private:
	// Thread
	static size_t MainThreadID;

	// Console
	Console::Console Terminal;
	std::map<std::string, std::shared_ptr<MainModule>> Commands;
};
