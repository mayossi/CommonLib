#pragma once
#include <string>


/*
 * Provides simple to use encryption and decryption routines.
 * Encryption works by encoding the data, xoring it with the key and encoding it again.
*/
namespace clib::encryption
{
	std::string xorString(
		const std::string& data,
		size_t dataSize,
		const std::string& key,
		size_t keySize
	);

	std::string encrypt(
		const std::string& data,
		const std::string& key
	);

	std::string decrypt(
		const std::string& data,
		const std::string& key
	);

} // namespace clib::encryption
