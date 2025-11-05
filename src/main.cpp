#include <std_include.hpp>

#include "tool.hpp"
#include "tests.hpp"

int main(int argc, char** argv)
{
	tool::main(argc, argv);
	tests::main(argc, argv);
	return 0;
}
