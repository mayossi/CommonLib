#pragma once

#include "../Hook.h"
#include "../IHookInstaller.h"


namespace clib::windows::hook
{

	/**
	 * Import Address Table hook installer.
	 * Internal implementation uses safeStrings and resolves WinApi functions dynamically.
	 *
	 * @note The installed hooks are installed in the current process's IAT.
	 */
	class IATHookInstaller : public IHookInstaller
	{
	public:
		void install(const Hook& hook) override;

		void uninstall(const Hook& hook) override;
	};

} // namespace clib::windows::hook
