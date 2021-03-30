#pragma once
namespace utils
{
	class memory
	{
	public:
		static void patch(char* dst, char* src, int size);
		static void nop(char* dst, unsigned size);
	};
}