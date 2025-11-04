#include <std_include.hpp>

#include "tool.hpp"

#include <shader-tool/shader.hpp>

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace tool
{
	namespace
	{
		void start_unsafe(int argc, char** argv)
		{
		}
	}

	void main(int argc, char** argv)
	{
		try
		{
			start_unsafe(argc, argv);
		}
		catch (const std::exception& e)
		{
			printf("error: %s\n", e.what());
		}
	}
}
