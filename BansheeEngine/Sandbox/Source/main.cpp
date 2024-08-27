#include "ExampleModel.h"
#include "Light.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_CustomModel{},
		m_SimpleModel{},
		m_Light{}
	{}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	ExampleCustomModel m_CustomModel;
	ExampleSimpleModel m_SimpleModel;
	Light m_Light;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}