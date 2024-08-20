#include <Banshee.h>
#include "Player.h"
#include "Light.h"

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_Player(),
		m_Player2()
	{
		m_Player2.SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
	}

	ClientApp(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	Player m_Player;
	Player m_Player2;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}