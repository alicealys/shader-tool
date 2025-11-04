#pragma once

#include <string>
#include <tomcrypt.h>

namespace utils::cryptography
{
	namespace sha1
	{
		std::string compute(const std::string& data, bool hex = false);
		std::string compute(const uint8_t* data, size_t length, bool hex = false);
	}

	namespace sha256
	{
		std::string compute(const std::string& data, bool hex = false);
		std::string compute(const uint8_t* data, size_t length, bool hex = false);
	}

	namespace sha512
	{
		std::string compute(const std::string& data, bool hex = false);
		std::string compute(const uint8_t* data, size_t length, bool hex = false);
	}

	namespace base64
	{
		std::string encode(const uint8_t* data, size_t len);
		std::string encode(const std::string& data);
		std::string decode(const std::string& data);
	}

	namespace jenkins_one_at_a_time
	{
		unsigned int compute(const std::string& data);
		unsigned int compute(const char* key, size_t len);
	};
}
