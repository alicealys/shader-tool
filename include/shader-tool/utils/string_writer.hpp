#pragma once

namespace alys::utils
{
	class string_writer
	{
	public:
		string_writer() = default;

		void write(const char* fmt, ...);
		void write(const std::string& data);

		std::string get_buffer() const;
		const char* data() const;

	private:
		std::string buffer_;

	};
}
