#pragma once

namespace Banshee
{
	class Entity;

	class Component
	{
	public:
		Component();
		virtual ~Component() = default;

		void SetDirty(const bool _dirty) { m_Dirty = _dirty; }
		bool GetDirty() const { return m_Dirty; }
		void SetOwner(Entity* _owner) { m_Owner = _owner; }
		const Entity* GetOwner() const { return m_Owner; }

	protected:
		bool m_Dirty;
		Entity* m_Owner;
	};
} // End of Banshee namespace