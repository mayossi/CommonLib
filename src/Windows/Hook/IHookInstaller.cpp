#include "IHookInstaller.h"


namespace clib::windows::hook
{
	IHookInstaller::IHookInstaller()
		: m_uninstallHooksAutomatically(true)
		, m_installedHooks()
	{
	}

	IHookInstaller::IHookInstaller(const bool uninstallHooksAutomatically /* = true */)
		: m_uninstallHooksAutomatically(uninstallHooksAutomatically)
		, m_installedHooks()
	{
	}

	IHookInstaller::IHookInstaller(IHookInstaller&& other) noexcept
		: m_uninstallHooksAutomatically(other.m_uninstallHooksAutomatically)
		, m_installedHooks(std::move(other.m_installedHooks))
	{
		// Disable uninstallation in moved-from object
		other.m_uninstallHooksAutomatically = false;
	}

	IHookInstaller& IHookInstaller::operator=(IHookInstaller&& other) noexcept
	{
		if (this != &other)
		{
			// Transfer ownership from the other object
			m_uninstallHooksAutomatically = other.m_uninstallHooksAutomatically;
			m_installedHooks = std::move(other.m_installedHooks);

			// Disable uninstallation in the moved-from object
			other.m_uninstallHooksAutomatically = false;
		}
		return *this;
	}

	void IHookInstaller::uninstallAllHooks()
	{
		for (auto& installedHook : m_installedHooks)
		{
			uninstall(std::move(installedHook));
		}

		m_installedHooks.clear();
	}

	void IHookInstaller::install(std::unique_ptr<Hook> pHook)
	{
		// Adding the new hook to the context vector.
		m_installedHooks.push_back(std::move(pHook));

		// Calling the actual hook implementation.
		installImp(m_installedHooks.back().get());
	}

	void IHookInstaller::uninstall(std::unique_ptr<Hook> pHook)
	{
		const auto it = std::find_if(
			m_installedHooks.begin(),
			m_installedHooks.end(),
			[&pHook](const std::unique_ptr<Hook>& h)
			{
				return h.get() == pHook.get();
			}
		);

		if (it != m_installedHooks.end())
		{
			uninstallImp(it->get());
			m_installedHooks.erase(it);
		}
	}

	IHookInstaller::~IHookInstaller()
	{
		// If the HookInstaller is configured to clean up after itself,
		// uninstalling all installed hooks automatically.
		if (m_uninstallHooksAutomatically)
		{
			uninstallAllHooks();
		}
	}

} // namespace clib::windows::hook
