#include "ExampleScene.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_DirectionalLight{ glm::vec3(-0.3f, -1.0f, -0.5f), glm::vec4(1.0f, 0.95f, 0.9f, 0.25f) },
		m_Ground{ glm::vec3(0.0f, 0.0f, 0.0f) },
		m_CustomModel{ glm::vec3(0.0f, 1.0f, 0.0f) },
		m_PointLightRed{ glm::vec3(-2.0f, 0.5f, -1.5f), glm::vec4(1.0f, 0.2f, 0.2f, 2.0f) },
		m_PointLightGreen{ glm::vec3(2.0f, 0.5f, 0.5f), glm::vec4(0.2f, 1.0f, 0.2f, 2.0f) },
		m_PointLightBlue{ glm::vec3(0.0f, 0.5f, 2.0f), glm::vec4(0.2f, 0.2f, 1.0f, 2.0f) }
	{
		m_CustomModel.SetTintColor(glm::vec4(1.0f, 0.8f, 0.3f, 1.0f));
	}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	DirectionalLight m_DirectionalLight;
	ExampleQuad m_Ground;
	ExampleCustomModel m_CustomModel;
	PointLight m_PointLightRed;
	PointLight m_PointLightGreen;
	PointLight m_PointLightBlue;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}
