#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Platform.h"
#include "Foundation/Components/Component.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class Component;
	class TransformComponent;

	class Entity
	{
	public:
		BANSHEE_ENGINE Entity(const uint32 _id) noexcept;
		BANSHEE_ENGINE ~Entity() noexcept = default;

		BANSHEE_ENGINE const uint32 GetUniqueId() const noexcept { return m_Id; }

		template<typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... _args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

			const std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(_args)...);
			m_Components.emplace_back(component);
			component->SetOwner(this);

			if (std::is_same<T, TransformComponent>::value)
			{
				if (!m_TransformComponent)
				{
					m_TransformComponent = std::dynamic_pointer_cast<TransformComponent>(component);
				}
			}

			return component;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent() const noexcept
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

		std::shared_ptr<TransformComponent> GetTransform() const noexcept { return m_TransformComponent; }

	private:
		uint32 m_Id;
		std::vector<std::shared_ptr<Component>> m_Components;
		std::shared_ptr<TransformComponent> m_TransformComponent;
	};
} // End of Banshee namespace