#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Platform.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class Component;
	class MeshComponent;

	class Entity
	{
	public:
		BANSHEE_ENGINE Entity(const uint32 _id);
		BANSHEE_ENGINE ~Entity();

		BANSHEE_ENGINE uint32 GetUniqueId() const { return m_Id; }

		template<typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... _args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			
			std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(_args)...);
			m_Components.emplace_back(component);

			if constexpr (std::is_same<T, MeshComponent>())
			{
				ApplyRenderableComponent(component);
			}

			return component;
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
		BANSHEE_ENGINE void ApplyRenderableComponent(std::shared_ptr<MeshComponent>& _component);

	private:
		uint32 m_Id;
		std::vector<std::shared_ptr<Component>> m_Components;
	};
} // End of Banshee namespace