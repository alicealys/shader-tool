#include <std_include.hpp>

#include "tool.hpp"
#include "shader.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace tool
{
	namespace
	{
		void read_shader(unsigned char* program, unsigned int program_size)
		{
			shader::patch_shader(program, program_size, [](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
			{
				shader::asm_::print_instruction(instruction);
				return false;
			});
		}

		void test_files()
		{
			const auto folder = "E:\\cod\\iw6\\dump\\techsets_mp_boneyard_ns1\\techsets\\ps\\";
			const auto files = utils::io::list_files(folder);

			for (const auto& file : files)
			{
				if (!file.ends_with(".cso"))
				{
					continue;
				}

				const auto name = file.substr(file.find_last_of("/\\") + 1);
				const auto dest_path = folder + "test/"s + name;

				printf("%s\n", name.data());

				std::vector<shader::asm_::instruction_t> instructions;

				{
					auto data = utils::io::read_file(file);
					const auto patched = shader::patch_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()),
						[&](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
						{
							instructions.emplace_back(instruction);
							shader::asm_::write_instruction(output_buffer, instruction);
							return true;
						}
					);

					utils::io::write_file(dest_path, patched);
				}

				{
					auto index = 0;
					auto data2 = utils::io::read_file(dest_path);

					shader::patch_shader(reinterpret_cast<unsigned char*>(data2.data()), static_cast<std::uint32_t>(data2.size()),
						[&](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
						{
							const auto& original_instruction = instructions[index++];

							if (original_instruction.opcode.type != instruction.opcode.type || 
								original_instruction.opcode.controls != instruction.opcode.controls ||
								original_instruction.opcode.length != instruction.opcode.length ||
								original_instruction.opcode.extended != instruction.opcode.extended ||
								original_instruction.opcode.extensions.size() != instruction.opcode.extensions.size() ||
								original_instruction.operands.size() != instruction.operands.size())
							{
								__debugbreak();
							}

							for (auto i = 0ull; i < original_instruction.operands.size(); i++)
							{
								if (original_instruction.operands[i].type != instruction.operands[i].type ||
									original_instruction.operands[i].dimension != instruction.operands[i].dimension ||
									original_instruction.operands[i].extended != instruction.operands[i].extended ||
									original_instruction.operands[i].extensions.size() != instruction.operands[i].extensions.size() ||
									std::memcmp(&original_instruction.operands[i].components, &instruction.operands[i].components, sizeof(instruction.operands[i].components)) ||
									std::memcmp(&original_instruction.operands[i].custom, &instruction.operands[i].custom, sizeof(instruction.operands[i].custom)) ||
									std::memcmp(&original_instruction.operands[i].indices, &instruction.operands[i].indices, sizeof(instruction.operands[i].indices))
								)
								{
									__debugbreak();
								}
							}

							return false;
						}
					);
				}
			}
		}

		void test_single_file()
		{
			const auto file = "E:\\cod\\iw6\\dump\\techsets_mp_boneyard_ns\\techsets\\hs\\world_displacement_debug_normals_world_disp_z2z3_vcl.hlsl_iw6.cso"s;

			{
				const auto file2 = file;
				auto data = utils::io::read_file(file2);
				read_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()));
			}

			{
				auto data = utils::io::read_file(file);
				const auto patched_shader = shader::patch_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()),
					[&](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
					{
						shader::asm_::write_instruction(output_buffer, instruction);
						return true;
					}
				);

				utils::io::write_file(file + ".patched", patched_shader);
			}

			printf("\n");

			{
				const auto file2 = file + ".patched";
				auto data = utils::io::read_file(file2);
				read_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()));
			}
		}

		void start_unsafe(int argc, char** argv)
		{
			test_files();
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
			printf("Action failed: %s\n", e.what());
		}
	}
}
