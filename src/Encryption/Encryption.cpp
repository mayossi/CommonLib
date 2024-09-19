#include "Encryption.h"
#include "../Base64/base64.h"


namespace clib::encryption
{
	std::string xorString(
		const std::string& data,
		const size_t dataSize,
		const std::string& key,
		const size_t keySize)
	{
		std::string encryptedData;
		for (int i = 0; i < static_cast<int>(dataSize); i++)
		{
			encryptedData += data[i] ^ key[i % keySize];
		}

		return encryptedData;
	}

	std::string encrypt(
		const std::string& data,
		const std::string& key)
	{
		if (data.empty() || key.empty())
		{
			return "";
		}

		const std::string encodedData = base64::encode(data);
		const std::string encryptedEncodedData = xorString(
			encodedData,
			encodedData.size(),
			key,
			key.size()
		);
		return base64::encode(encryptedEncodedData);
	}

	std::string decrypt(
		const std::string& data,
		const std::string& key)
	{
		if (data.empty() || key.empty())
		{
			return "";
		}

		const std::string decodedEncryptedData = base64::decode(data);
		const std::string decryptedEncodedData = xorString(
			decodedEncryptedData,
			decodedEncryptedData.size(),
			key,
			key.size()
		);
		return base64::decode(decryptedEncodedData);
	}

} // namespace clib::encryption
