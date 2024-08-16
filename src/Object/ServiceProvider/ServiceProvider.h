#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>


namespace clib::object
{
    /**
    * Defines a generic service provider
    */
    class ServiceProvider
    {
    public:
        template<typename T>
        static void provide(std::shared_ptr<T> service) 
        {
            services[std::type_index(typeid(T))] = service;
        }

        template<typename T>
        static std::shared_ptr<T> get() 
        {
            auto it = services.find(std::type_index(typeid(T)));
            if (it != services.end()) 
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr; // service not found
        }

        template<typename T>
        static std::shared_ptr<T> replace(std::shared_ptr<T> newService) 
        {
            auto type = std::type_index(typeid(T));
            std::shared_ptr<T> oldService = nullptr;

            if (services.find(type) != services.end()) {
                oldService = std::static_pointer_cast<T>(services[type]);
            }

            services[type] = newService;
            return oldService;
        }

    private:
        static std::unordered_map<std::type_index, std::shared_ptr<void>> services;
    };
} // namespace clib::object

