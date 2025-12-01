#pragma once

#include "base.hpp"

namespace alys::shader::detail
{
	template <std::uint32_t Flags = flag_none>
	class general_instruction final : public base_instruction
	{
		instruction_t read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
		{
			instruction_t instruction{};
			std::uint32_t opcode_length{};
			instruction.opcode = read_opcode(input_buffer, opcode_length);

			const auto opcode_op_len = 1 + instruction.opcode.extensions.size();
			const auto end = input_buffer.total() + (opcode_length - opcode_op_len) * 8 * 4;
			while (input_buffer.total() < end)
			{
				const auto operand = read_operand(input_buffer);
				instruction.operands.emplace_back(operand);
			}

			return instruction;
		}

		void write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
		{
			const auto length = get_opcode_length(instruction);
			write_opcode(output_buffer, instruction.opcode, length);
			for (const auto& operand : instruction.operands)
			{
				write_operand(output_buffer, operand);
			}
		}

		void dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
		{
			dump_opcode_name(buffer, instruction.opcode);

			if constexpr ((Flags & flag_conditional) != 0)
			{
				buffer.write((instruction.opcode.controls & 128) ? "_nz" : "_z");
			}

			if constexpr ((Flags & flag_saturate) != 0)
			{
				if ((instruction.opcode.controls & 0x4) != 0)
				{
					buffer.write("_sat");
				}
			}

			if constexpr ((Flags & flag_precise) != 0)
			{
				const auto precise_mask = instruction.opcode.controls >> 8;
				if (precise_mask != 0)
				{
					buffer.write(" [precise");

					if (precise_mask != 0xF)
					{
						buffer.write("(");
						if ((precise_mask & 0x1) != 0)
						{
							buffer.write("x");
						}

						if ((precise_mask & 0x2) != 0)
						{
							buffer.write("y");
						}

						if ((precise_mask & 0x4) != 0)
						{
							buffer.write("z");
						}

						if ((precise_mask & 0x8) != 0)
						{
							buffer.write("w");
						}
						buffer.write(")");
					}

					buffer.write("]");
				}
			}

			if (instruction.opcode.type == D3D10_SB_OPCODE_RESINFO)
			{
				switch (instruction.opcode.controls)
				{
				case 1:
					buffer.write("_rcpFloat");
					break;
				case 2:
					buffer.write("_uint");
					break;
				}
			}

			if (instruction.opcode.type == D3D10_1_SB_OPCODE_SAMPLE_INFO)
			{
				switch (instruction.opcode.controls)
				{
				case 1:
					buffer.write("_uint");
					break;
				}
			}

			for (const auto& ext : instruction.opcode.extensions)
			{
				if (ext.type == D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS)
				{
					buffer.write("_aoffimmi");
				}
			}

			if constexpr ((Flags & flag_indexable) != 0)
			{
				buffer.write("_indexable");
			}

			const auto dump_extension = [&](const std::uint32_t type)
			{
				for (const auto& ext : instruction.opcode.extensions)
				{
					if (ext.type != type)
					{
						continue;
					}

					dump_opcode_extended(buffer, ext);
					break;
				}
			};

			dump_extension(D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS);
			dump_extension(D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM);
			dump_extension(D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE);

			if (!instruction.operands.empty())
			{
				buffer.write(" ");
				dump_operands(buffer, instruction.operands);
			}
		}

		std::uint32_t get_flags()
		{
			return Flags;
		}
	};
}
