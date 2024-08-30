#pragma once

#include "Hook.h"


namespace clib::windows::hook
{
	/**
	 * A hook installer interface.
	 */
	class IHookInstaller
	{
	public:
		virtual ~IHookInstaller() = default;

		virtual void install(const Hook& hook) = 0;

		virtual void uninstall(const Hook& hook) = 0;
	};

} // namespace clib::windows::hook
