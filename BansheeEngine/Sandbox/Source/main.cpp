#include "ExampleModel.h"
#include "Light.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_CustomModel{},
		m_CustomModel2{ glm::vec3(20.0f, 0.0f, 0.0f) },
		m_CustomModel3{ glm::vec3(40.0f, 0.0f, 0.0f) },
		m_PrimitiveCube{ glm::vec3(0.0f, 10.0f, 0.0f) },
		m_Light{}
	{
		m_CustomModel.SetTintColor(glm::vec3(1.0f, 0.0f, 0.0f));
		m_CustomModel2.SetTintColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_CustomModel3.SetTintColor(glm::vec3(0.0f, 0.0f, 1.0f));
	}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	ExampleCustomModel m_CustomModel;
	ExampleCustomModel m_CustomModel2;
	ExampleCustomModel m_CustomModel3;
	ExamplePrimitiveModel m_PrimitiveCube;
	Light m_Light;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}