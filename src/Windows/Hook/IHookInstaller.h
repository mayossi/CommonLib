#pragma once

#include <memory>
#include <vector>

#include "Hook.h"


namespace clib::windows::hook
{
	/**
	 * Generic hook installer interface.
	 * Keeps context of all installed hooks and can uninstall installed hooks automatically (on dtor) if configured to do so.
	 */
	class IHookInstaller
	{
	public:
		explicit IHookInstaller();

		explicit IHookInstaller(bool uninstallHooksAutomatically = true);

		IHookInstaller(const IHookInstaller&) = delete;

		IHookInstaller& operator=(const IHookInstaller&) = delete;

		IHookInstaller& operator=(IHookInstaller&& other) noexcept;

		IHookInstaller(IHookInstaller&& other) noexcept;

		void install(std::unique_ptr<Hook> pHook);

		void uninstall(std::unique_ptr<Hook> pHook);

		virtual ~IHookInstaller();

	protected:
		virtual void installImp(Hook* hook) = 0;

		virtual void uninstallImp(Hook* hook) = 0;

	private:
		void uninstallAllHooks();

		// Determines if the hooks will be cleared when the HookInstaller is destructed.
		bool m_uninstallHooksAutomatically;

		// Contains all installed hooks.
		std::vector<std::unique_ptr<Hook>> m_installedHooks;
	};

} // namespace clib::windows::hook
