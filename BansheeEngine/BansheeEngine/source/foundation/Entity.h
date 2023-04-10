#pragma once

#include "Platform.h"

namespace Banshee
{
	class Entity
	{
	public:
		Entity(uint32 _id) :
			m_Id(_id)
		{}

		uint32 GetId() const { return m_Id; }

	private:
		uint32 m_Id;
	};
} // End of Banshee namespace