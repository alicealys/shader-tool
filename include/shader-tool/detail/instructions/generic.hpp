#pragma once

#include "base.hpp"

namespace alys::shader::detail
{
	class general_instruction final : public base_instruction
	{
	public:
		instruction_t read(utils::bit_buffer_le&);
		void write(utils::bit_buffer_le&, const instruction_t&);
		void print(const instruction_t&);
	};

	class declaration_instruction_nametoken final : public base_instruction
	{
	public:
		instruction_t read(utils::bit_buffer_le&);
		void write(utils::bit_buffer_le&, const instruction_t&);
		void print(const instruction_t&);
	};

	template <size_t OperandCount, size_t ValueCount>
	class declaration_instruction final : public base_instruction
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

		void print(const instruction_t& instruction)
		{
			print_opcode(instruction.opcode);

			printf(" ");

			for (auto i = 0u; i < OperandCount; i++)
			{
				print_operand(instruction.operands[i]);
				if (i < (OperandCount + ValueCount) - 1)
				{
					printf(", ");
				}
			}

			for (auto i = 0u; i < ValueCount; i++)
			{
				printf("%i", instruction.operands[i + OperandCount].custom.u.value);
				if (i < ValueCount - 1)
				{
					printf(", ");
				}
			}

			printf("\n");
		}
	};
}
