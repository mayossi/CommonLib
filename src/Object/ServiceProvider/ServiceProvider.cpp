#include "ServiceProvider.h"


namespace clib::object
{
	std::unordered_map<std::type_index, std::shared_ptr<void>> ServiceProvider::services;

} // namespace clib::object
