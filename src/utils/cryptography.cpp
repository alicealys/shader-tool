#include <std_include.hpp>

#include "string.hpp"
#include "cryptography.hpp"

#undef max
using namespace std::string_literals;

namespace utils::cryptography
{
	namespace
	{
		[[maybe_unused]] const char* cs(const uint8_t* data)
		{
			return reinterpret_cast<const char*>(data);
		}

		[[maybe_unused]] char* cs(uint8_t* data)
		{
			return reinterpret_cast<char*>(data);
		}

		[[maybe_unused]] const uint8_t* cs(const char* data)
		{
			return reinterpret_cast<const uint8_t*>(data);
		}

		[[maybe_unused]] uint8_t* cs(char* data)
		{
			return reinterpret_cast<uint8_t*>(data);
		}

		[[maybe_unused]] unsigned long ul(const size_t value)
		{
			return static_cast<unsigned long>(value);
		}
	}

	std::string sha1::compute(const std::string& data, const bool hex)
	{
		return compute(cs(data.data()), data.size(), hex);
	}

	std::string sha1::compute(const uint8_t* data, const size_t length, const bool hex)
	{
		uint8_t buffer[20] = {0};

		hash_state state;
		sha1_init(&state);
		sha1_process(&state, data, ul(length));
		sha1_done(&state, buffer);

		std::string hash(cs(buffer), sizeof(buffer));
		if (!hex) return hash;

		return string::dump_hex(hash, "");
	}

	std::string sha256::compute(const std::string& data, const bool hex)
	{
		return compute(cs(data.data()), data.size(), hex);
	}

	std::string sha256::compute(const uint8_t* data, const size_t length, const bool hex)
	{
		uint8_t buffer[32] = {0};

		hash_state state;
		sha256_init(&state);
		sha256_process(&state, data, ul(length));
		sha256_done(&state, buffer);

		std::string hash(cs(buffer), sizeof(buffer));
		if (!hex) return hash;

		return string::dump_hex(hash, "");
	}

	std::string sha512::compute(const std::string& data, const bool hex)
	{
		return compute(cs(data.data()), data.size(), hex);
	}

	std::string sha512::compute(const uint8_t* data, const size_t length, const bool hex)
	{
		uint8_t buffer[64] = {0};

		hash_state state;
		sha512_init(&state);
		sha512_process(&state, data, ul(length));
		sha512_done(&state, buffer);

		std::string hash(cs(buffer), sizeof(buffer));
		if (!hex) return hash;

		return string::dump_hex(hash, "");
	}

	std::string base64::encode(const uint8_t* data, const size_t len)
	{
		std::string result;
		result.resize((len + 2) * 2);

		auto out_len = ul(result.size());
		if (base64_encode(data, ul(len), result.data(), &out_len) != CRYPT_OK)
		{
			return {};
		}

		result.resize(out_len);
		return result;
	}

	std::string base64::encode(const std::string& data)
	{
		return base64::encode(cs(data.data()), static_cast<unsigned>(data.size()));
	}

	std::string base64::decode(const std::string& data)
	{
		std::string result;
		result.resize((data.size() + 2) * 2);

		auto out_len = ul(result.size());
		if (base64_decode(data.data(), ul(data.size()), cs(result.data()), &out_len) != CRYPT_OK)
		{
			return {};
		}

		result.resize(out_len);
		return result;
	}

	unsigned int jenkins_one_at_a_time::compute(const std::string& data)
	{
		return compute(data.data(), data.size());
	}

	unsigned int jenkins_one_at_a_time::compute(const char* key, const size_t len)
	{
		unsigned int hash, i;
		for (hash = i = 0; i < len; ++i)
		{
			hash += key[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}
}
