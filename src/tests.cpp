#include <std_include.hpp>

#include "tests.hpp"

#include <shader-tool/shader.hpp>

#include <utils/io.hpp>

using namespace alys::shader::literals;

namespace tests
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
