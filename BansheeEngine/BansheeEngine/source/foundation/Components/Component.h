#pragma once

namespace Banshee
{
	class Entity;

	class Component
	{
	public:
		Component() noexcept :
			m_Dirty(true),
			m_Owner(nullptr)
		{}

		virtual ~Component() = default;

		void SetDirty(const bool _dirty) noexcept { m_Dirty = _dirty; }
		bool GetDirty() const noexcept { return m_Dirty; }
		void SetOwner(Entity* _owner) noexcept { m_Owner = _owner; }
		const Entity* GetOwner() const noexcept { return m_Owner; }

	protected:
		bool m_Dirty;
		Entity* m_Owner;
	};
} // End of Banshee namespace