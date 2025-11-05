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

		this->buffer_.append(result);
	}

	void string_writer::write(const std::string& data)
	{
		this->buffer_.append(data);
	}

	std::string string_writer::get_buffer() const
	{
		return this->buffer_;
	}

	const char* string_writer::data() const
	{
		return this->buffer_.data();
	}
}
