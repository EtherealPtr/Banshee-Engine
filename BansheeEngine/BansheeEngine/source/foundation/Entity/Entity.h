#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Platform.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class Component;
	class MeshRendererComponent;

	class Entity
	{
	public:
		BANSHEE_ENGINE Entity(const uint32 _id);
		BANSHEE_ENGINE ~Entity();

		BANSHEE_ENGINE uint32 GetUniqueId() const { return m_Id; }

		template<typename T>
		void AddComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			
			std::shared_ptr<T> component = std::make_shared<T>();
			m_Components.emplace_back(component);

			if constexpr (std::is_same<T, MeshRendererComponent>())
			{
				ApplyRenderableComponent(component);
			}
		}

		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			for (const auto& component : m_Components)
			{
				if (auto ptr = std::dynamic_pointer_cast<T>(component))
				{
					return ptr;
				}
			}
		
			return nullptr;
		}

	private:
		BANSHEE_ENGINE void ApplyRenderableComponent(std::shared_ptr<MeshRendererComponent>& _component);

	private:
		uint32 m_Id;
		std::vector<std::shared_ptr<Component>> m_Components;
	};
} // End of Banshee namespace