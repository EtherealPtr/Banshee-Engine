#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Platform.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class Component;
	class TransformComponent;

	class Entity
	{
	public:
		BANSHEE_ENGINE Entity(const uint32 _id);
		BANSHEE_ENGINE ~Entity() noexcept = default;

		BANSHEE_ENGINE const uint32 GetUniqueId() const noexcept { return m_Id; }
		BANSHEE_ENGINE const std::shared_ptr<TransformComponent>& GetTransform() const noexcept { return m_Transform; }

		template<typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... _args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
			
			const std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(_args)...);
			m_Components.emplace_back(component);
			RegisterComponent(component);

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
		BANSHEE_ENGINE void RegisterComponent(const std::shared_ptr<Component>& _component) const;

	private:
		uint32 m_Id;
		std::shared_ptr<TransformComponent> m_Transform;
		std::vector<std::shared_ptr<Component>> m_Components;
	};
} // End of Banshee namespace