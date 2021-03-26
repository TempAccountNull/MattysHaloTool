#pragma once
namespace utils
{
	class memory
	{
	public:
		static void Patch(char* dst, char* src, int size);
		static void Nop(char* dst, unsigned size);
	};
}