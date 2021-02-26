#pragma once
#include <inttypes.h>

#include "MainModule.h"

class Disclaimer : public MainModule
{
public:
	Disclaimer();
	~Disclaimer();

	void Tick(const std::chrono::duration<double>& Delta);

	// Return true on success
	bool Run(const std::vector<std::string>& Arguments);

	// Command and usage info
	std::string Info(const std::string& Topic = "") const;

	// Suggest auto-complete strings for arugments
	std::string Suggest(const std::vector<std::string>& Arguments) const;
private:
};