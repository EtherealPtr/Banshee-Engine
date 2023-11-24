#pragma once

#include <vector>
#include <memory>

namespace Banshee
{
	class Entity;
	class System;

	class Component
	{
	public:
		Component() noexcept :
			m_Dirty(false),
			m_Owner(nullptr)
		{}

		virtual ~Component() noexcept = default;

		void SetDirty(const bool _dirty);
		bool GetDirty() const noexcept { return m_Dirty; }
		void SetOwner(const Entity* _owner) noexcept { m_Owner = _owner; }
		const Entity* GetOwner() const noexcept { return m_Owner; }
		void RegisterSystem(System* _system);

	private:
		void NotifyObservers() const;

	protected:
		bool m_Dirty;
		const Entity* m_Owner;
		std::vector<System*> m_Systems;
	};
} // End of Banshee namespace