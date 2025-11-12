#include <std_include.hpp>

#include "tool.hpp"
#include "tests.hpp"

#include <shader-tool/shader.hpp>

#include <utils/io.hpp>

namespace tool
{
	namespace
	{
		using command_cb = std::function<void(const std::vector<std::string>&)>;
		std::unordered_map<std::string, command_cb> commands;

		void add_command(const std::string& name, const command_cb& cb)
		{
			commands.insert(std::make_pair(name, cb));
		}

		void disassemble_shader(const std::vector<std::string>& args)
		{
			if (args.size() < 2)
			{
				printf("usage: disassemble <file> [dest]\n");
				return;
			}

			std::string data;

			const auto& path = args[1];
			std::string dest;
			if (args.size() >= 3)
			{
				dest = args[2];
			}
			else
			{
				const auto pos = path.find_last_of('.');
				dest = path.substr(0, pos) + ".asm";
			}

			if (!utils::io::read_file(path, &data))
			{
				throw std::runtime_error("file not found");
			}

			printf("disassembling %s...\n", path.data());
			const auto shader = alys::shader::shader_object::parse(data);
			utils::io::write_file(dest, shader.dump());
			printf("\t -> %s\n", dest.data());
		}

		void print_shader(const std::vector<std::string>& args)
		{
			if (args.size() < 2)
			{
				printf("usage: print <file> [dest]\n");
				return;
			}

			std::string data;

			const auto& path = args[1];
			if (!utils::io::read_file(path, &data))
			{
				throw std::runtime_error("file not found");
			}

			const auto shader = alys::shader::shader_object::parse(data);
			shader.print();
		}

		void run_tests(const std::vector<std::string>& args)
		{
			tests::run_tests();
		}

		void print_usage()
		{
			printf("usage: shader-tool [options]\n\n");
			printf("\tdisassemble <path> [dest]\n");
			printf("\t\tdisassemble a shader\n\n");
			printf("\tprint <path>\n");
			printf("\t\tdisassembles shader to output\n\n");
		}

		void start_unsafe(int argc, char** argv)
		{
			if (argc < 2)
			{
				print_usage();
				return;
			}

			add_command("disassemble", disassemble_shader);
			add_command("print", print_shader);
			add_command("run_tests", run_tests);

			std::vector<std::string> args;
			for (auto i = 1; i < argc; i++)
			{
				args.emplace_back(argv[i]);
			}

			const auto iter = commands.find(args[0]);
			if (iter != commands.end())
			{
				iter->second(args);
			}
			else
			{
				print_usage();
			}
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
