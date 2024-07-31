#include "Entity.h"
#include "Foundation/Components/Component.h"

namespace Banshee
{
	Entity::Entity(const uint32 _id) noexcept :
		m_Id{ _id },
		m_Components{},
		m_TransformComponent{}
	{}
} // End of Banshee namespace