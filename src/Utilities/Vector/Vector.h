#pragma once

#include <vector>
#include <string>


namespace clib::utilities::vector
{
	template <typename T>
	bool isInVector(std::vector<T>& vector, T& item)
	{
		return std::find(vector.begin(), vector.end(), item) != vector.end();
	}

} // namespace clib::utilities::vector
