#pragma once

#include <string>


/**
 * Wrapper for macaron's base64 impl.
 */
namespace clib::base64
{
	// Encodes the given data with Base64
	// Throws EncodeException if the data is empty or if an exception occured when encoding. 
	std::string encode(const std::string& data);

	// Decode the given data with Base64
	// Throws DecodeException if the data is empty or if an exception occured when decoding.
	std::string decode(const std::string& data);

} // namespace clib::base64