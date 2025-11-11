#pragma once

namespace alys::utils
{
	class string_writer
	{
	public:
		string_writer() = default;

		void write(const char* fmt, ...);

		std::string get_buffer() const;
		const char* data() const;
		void set_to_console(bool to_console);

	private:
		std::string buffer_;
		bool to_console_ = false;

	};
}
