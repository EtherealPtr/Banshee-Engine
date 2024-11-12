#include "ExampleModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_CustomModel{},
		//m_DirectionalLight{},
		m_PointLight{ glm::vec3(5.0f, 1.0f, -5.0f), glm::vec4(0.1f, 0.1f, 1.0f, 5.0f), 5.0f },
		m_PointLight2{ glm::vec3(-5.0f, 1.0f, -5.0f), glm::vec4(1.0f, 0.1f, 0.1f, 5.0f), 5.0f },
		m_SpotLight{ glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(0.1f, 1.0f, 0.1f, 10.0f), 10.0f, 45.0f }
	{}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	ExampleCustomModel m_CustomModel;
	//DirectionalLight m_DirectionalLight;
	PointLight m_PointLight;
	PointLight m_PointLight2;
	SpotLight m_SpotLight;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}