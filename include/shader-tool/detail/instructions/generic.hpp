#pragma once

#include "base.hpp"

namespace alys::shader::detail
{
	enum instruction_flags
	{
		flag_none = 0,
		flag_conditional = (1 << 0),
		flag_saturate = (1 << 1),
		flag_precise = (1 << 2),
		flag_integer = (1 << 3),
	};

	template <std::uint32_t Flags = flag_none>
	class general_instruction final : public base_instruction
	{
		instruction_t read(utils::bit_buffer_le& input_buffer)
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

		void write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
		{
			const auto length = get_opcode_length(instruction);
			write_opcode(output_buffer, instruction.opcode, length);
			for (const auto& operand : instruction.operands)
			{
				write_operand(output_buffer, operand);
			}
		}

		void dump(utils::string_writer& buffer, const instruction_t& instruction)
		{
			dump_opcode_name(buffer, instruction.opcode);

			if constexpr ((Flags & flag_conditional))
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

			for (const auto& ext : instruction.opcode.extensions)
			{
				dump_opcode_extended(buffer, ext);
			}

			if (!instruction.operands.empty())
			{
				buffer.write(" ");
				dump_operands(buffer, instruction.operands, (Flags & flag_integer) == 0);
			}
		}
	};

	template <size_t OperandCount, size_t ValueCount>
	class declaration_instruction : public base_instruction
	{
	public:
		instruction_t read(utils::bit_buffer_le& input_buffer)
		{
			instruction_t instruction{};

			std::uint32_t length{};
			instruction.opcode = read_opcode(input_buffer, length);

			for (auto i = 0u; i < OperandCount; i++)
			{
				instruction.operands.emplace_back(read_operand(input_buffer));
			}

			for (auto i = 0u; i < ValueCount; i++)
			{
				operand_t op{};
				op.custom.is_custom = true;
				op.custom.u.value = input_buffer.read_bytes(4);
				instruction.operands.emplace_back(op);
			}

			return instruction;
		}

		void write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
		{
			const auto length = get_opcode_length(instruction);
			write_opcode(output_buffer, instruction.opcode, length);

			for (auto i = 0u; i < OperandCount; i++)
			{
				write_operand(output_buffer, instruction.operands[i]);
			}

			for (auto i = 0u; i < ValueCount; i++)
			{
				output_buffer.write_bytes(4, instruction.operands[i + OperandCount].custom.u.value);
			}
		}

		void dump(utils::string_writer& buffer, const instruction_t& instruction)
		{
			dump_opcode(buffer, instruction.opcode);

			buffer.write(" ");

			for (auto i = 0u; i < OperandCount; i++)
			{
				dump_operand(buffer, instruction.operands[i]);
				if (i < (OperandCount + ValueCount) - 1)
				{
					buffer.write(", ");
				}
			}

			for (auto i = 0u; i < ValueCount; i++)
			{
				buffer.write("%i", instruction.operands[i + OperandCount].custom.u.value);
				if (i < ValueCount - 1)
				{
					buffer.write(", ");
				}
			}
		}
	};
}
