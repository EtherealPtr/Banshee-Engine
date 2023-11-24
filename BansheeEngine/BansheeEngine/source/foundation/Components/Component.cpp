#include "Component.h"
#include "Foundation/Systems/System.h"

namespace Banshee
{
	void Component::SetDirty(const bool _dirty)
	{
		m_Dirty = _dirty;

		if (m_Dirty)
		{
			NotifyObservers();
			m_Dirty = false;
		}
	}

	void Component::RegisterSystem(System* _system)
	{
		m_Systems.emplace_back(_system);
	}

	void Component::NotifyObservers() const
	{
		for (System* const system : m_Systems)
		{
			if (system)
			{
				system->Update();
			}
		}
	}
} // End of Banshee namespace