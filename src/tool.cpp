#include <std_include.hpp>

#include "tool.hpp"
#include "shader.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace tool
{
	namespace
	{
		constexpr const auto ldr_const = 1550.f;
		constexpr const auto temp_register_inc = 10;

		shader::asm_::operand_t write_cb_div_instruction(utils::bit_buffer_le& output_buffer, 
			const shader::asm_::operand_t& cb_operand_orig, const std::uint32_t index)
		{
			auto div = shader::asm_::tokens::create_opcode(D3D10_SB_OPCODE_DIV, 0);

			const auto temp_register = shader::asm_::tokens::create_register_operand(D3D10_SB_OPERAND_TYPE_TEMP, "xyzw", index);
			const auto cb_operand = shader::asm_::tokens::create_register_operand(D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER, "xyzw",
				cb_operand_orig.indices[0].values[0].u32, cb_operand_orig.indices[1].values[0].u32);

			const auto literal = shader::asm_::tokens::create_literal_operand(ldr_const, ldr_const, ldr_const, ldr_const);

			div.length += shader::asm_::writer::get_operand_length(temp_register);
			div.length += shader::asm_::writer::get_operand_length(cb_operand);
			div.length += shader::asm_::writer::get_operand_length(literal);

			shader::asm_::writer::write_opcode(output_buffer, div);
			shader::asm_::writer::write_operand(output_buffer, temp_register);
			shader::asm_::writer::write_operand(output_buffer, cb_operand);
			shader::asm_::writer::write_operand(output_buffer, literal);

			return temp_register;
		}

		bool patch_instruction_dcl_temps(utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction, std::uint32_t& base_temps)
		{
			base_temps = instruction.operands[0].custom.types.dcl_temps.size;
			instruction.operands[0].custom.types.dcl_temps.size += temp_register_inc;

			shader::asm_::writer::write_opcode(output_buffer, instruction.opcode);
			shader::asm_::writer::write_operand(output_buffer, instruction.operands[0]);

			return true;
		}

		bool patch_instruction(utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction, 
			const std::unordered_set<std::uint32_t>& cb_indices, std::uint32_t& base_temps)
		{
			const auto cb_operand_indices = shader::asm_::tokens::find_operands(instruction, 1, [&](const shader::asm_::operand_t& operand)
			{
				return operand.type == D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER && cb_indices.contains(operand.indices[1].values[0].u32);
			});

			if (cb_operand_indices.empty())
			{
				return false;
			}

			for (auto i = 0u; i < cb_operand_indices.size(); i++)
			{
				const auto index = cb_operand_indices[i];
				const auto& cb_operand = instruction.operands[index];

				auto pseudo_cb_operand = write_cb_div_instruction(output_buffer, cb_operand, base_temps + i);

				pseudo_cb_operand.components = instruction.operands[index].components;
				pseudo_cb_operand.extended = instruction.operands[index].extended;
				pseudo_cb_operand.extensions = instruction.operands[index].extensions;
				pseudo_cb_operand.extra_operand = instruction.operands[index].extra_operand;
				instruction.operands[index] = pseudo_cb_operand;
			}

			instruction.opcode.length = 1;

			for (const auto& operand : instruction.operands)
			{
				instruction.opcode.length += shader::asm_::writer::get_operand_length(operand);
			}

			shader::asm_::writer::write_opcode(output_buffer, instruction.opcode);

			for (const auto& operand : instruction.operands)
			{
				shader::asm_::writer::write_operand(output_buffer, operand);
			}

			return true;
		}

		bool patch_shader_const(utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction, 
			const std::unordered_set<std::uint32_t>& cb_indices, std::uint32_t& base_temps)
		{
			switch (instruction.opcode.type)
			{
			case D3D10_SB_OPCODE_MOV:
			case D3D10_SB_OPCODE_MUL:
			case D3D10_SB_OPCODE_ADD:
			case D3D10_SB_OPCODE_DP4:
				return patch_instruction(output_buffer, instruction, cb_indices, base_temps);
			case D3D10_SB_OPCODE_DCL_TEMPS:
				return patch_instruction_dcl_temps(output_buffer, instruction, base_temps);
			}

			return false;
		}

		std::uint32_t convert_dest(std::uint32_t dest)
		{
			if (dest >= 0x7F)
			{
				return dest;
			}

			if (dest >= 65)
			{
				return dest - 3;
			}

			if (dest >= 15)
			{
				return dest - 1;
			}

			return dest;
		}

		bool patch_cb_index(utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
		{
			if (instruction.opcode.type == D3D10_SB_OPCODE_DCL_CONSTANT_BUFFER)
			{
				return false;
			}

			shader::asm_::writer::write_opcode(output_buffer, instruction.opcode);
			for (auto& operand : instruction.operands)
			{
				if (operand.type == D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER && operand.extra_operand == nullptr)
				{
					operand.indices[1].values[0].u32 = convert_dest(operand.indices[1].values[0].u32);
				}

				shader::asm_::writer::write_operand(output_buffer, operand);
			}

			return true;
		}

		void read_shader(unsigned char* program, unsigned int program_size)
		{
			shader::patch_shader(program, program_size, [](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
			{
				shader::asm_::disassembler::print_instruction(instruction);
				return false;
			});
		}

		void test_files()
		{
			const auto folder = "E:\\cod\\iw6\\dump\\techsets_mp_boneyard_ns\\techsets\\ps\\";
			const auto files = utils::io::list_files(folder);

			for (const auto& file : files)
			{
				if (!file.ends_with(".cso"))
				{
					continue;
				}

				const auto name = file.substr(file.find_last_of("/\\") + 1);
				printf("%s\n", name.data());

				auto data = utils::io::read_file(file);

				std::uint32_t base_temps{};
				const auto patched = shader::patch_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()),
					[&](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
					{
						std::unordered_set<std::uint32_t> cb_indices;
						cb_indices.insert(4);
						cb_indices.insert(5);
						cb_indices.insert(6);


						return patch_shader_const(output_buffer, instruction, cb_indices, base_temps);
					}
				);

				utils::io::write_file(folder + "patched_cb/"s + name, patched);
			}
		}

		void test_files_h2_h1(const std::string& sub_folder)
		{
			const auto folder = "E:\\cod\\h2\\zonetool\\techsets_estate\\techsets\\" + sub_folder;
			const auto files = utils::io::list_files(folder);

			for (const auto& file : files)
			{
				if (!file.ends_with(".cso"))
				{
					continue;
				}

				const auto name = file.substr(file.find_last_of("/\\") + 1);
				printf("%s\n", name.data());

				auto data = utils::io::read_file(file);

				const auto patched = shader::patch_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()),
					patch_cb_index);

				//utils::io::write_file(folder + "/patched/" + name, patched);
			}
		}


		void test_single_file()
		{
			const auto file = "E:\\cod\\iw6\\dump\\techsets_mp_boneyard_ns\\techsets\\ps\\lmo_sm_l0d_l0sd_r0c0l0b1c1v1nl1l1b2c2l2_nofog_vcl_blm_sm5.hlsl_iw6.cso"s;

			{
				auto data = utils::io::read_file(file);
				std::uint32_t base_temps{};
				const auto patched_shader = shader::patch_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()),
					[&](utils::bit_buffer_le& output_buffer, shader::asm_::instruction_t instruction)
					{
						std::unordered_set<std::uint32_t> cb_indices;
						cb_indices.insert(4);
						cb_indices.insert(5);
						cb_indices.insert(6);


						return patch_shader_const(output_buffer, instruction, cb_indices, base_temps);
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

		void test_single_file_h2_h1()
		{
			const auto file = "E:\\cod\\h2\\zonetool\\techsets_estate\\techsets\\ps - Copy\\blood_pool_l_spot_feo_ndw_lmpb_l0s2_frt_sm_b0c0n0s0.hlsl.cso"s;

			{
				auto data = utils::io::read_file(file);
				//read_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()));
			}

			{
				auto data = utils::io::read_file(file);
				const auto patched_shader = shader::patch_shader(reinterpret_cast<unsigned char*>(data.data()), static_cast<std::uint32_t>(data.size()),
					patch_cb_index);

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
