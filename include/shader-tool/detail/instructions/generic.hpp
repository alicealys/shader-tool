#pragma once

#include "base.hpp"

namespace alys::shader::detail
{
	class generic_instruction : public base_instruction
	{
	public:
		instruction_t read(utils::bit_buffer_le&);
		void write(utils::bit_buffer_le&, const instruction_t&);
		void dump(utils::string_writer& buffer, const instruction_t&);
	};

	class general_instruction final : public generic_instruction
	{
	};

	class conditional_instruction final : public generic_instruction
	{
	public:
		void dump(utils::string_writer& buffer, const instruction_t&) override;
	};

	class arithmetic_instruction final : public generic_instruction
	{
	public:
		void dump(utils::string_writer& buffer, const instruction_t&) override;
	};

	class declaration_instruction_nametoken : public base_instruction
	{
	public:
		instruction_t read(utils::bit_buffer_le&);
		void write(utils::bit_buffer_le&, const instruction_t&);
		void dump(utils::string_writer& buffer, const instruction_t&);
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
