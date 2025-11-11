#include "../std_include.hpp"

#include "string_writer.hpp"

namespace alys::utils
{
	namespace
	{
		std::string format(va_list* ap, const char* message)
		{
			static thread_local char buffer[0x1000];

			const auto count = vsnprintf_s(buffer, _TRUNCATE, message, *ap);
			if (count < 0)
			{
				return {};
			}

			return {buffer, static_cast<size_t>(count)};
		}
	}

	void string_writer::write(const char* fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);
		const auto result = format(&ap, fmt);
		va_end(ap);

		if (this->to_console_)
		{
			printf("%s", result.data());
		}
		else
		{
			this->buffer_.append(result);
		}
	}

	std::string string_writer::get_buffer() const
	{
		return this->buffer_;
	}

	const char* string_writer::data() const
	{
		return this->buffer_.data();
	}

	void string_writer::set_to_console(bool to_console)
	{
		this->to_console_ = to_console;
	}
}
