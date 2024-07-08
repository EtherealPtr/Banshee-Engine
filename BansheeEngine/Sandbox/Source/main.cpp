#include <Banshee.h>
#include "Player.h"
#include "Light.h"

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_Player(),
		m_Light()
	{}

private:
	Player m_Player;
	Light m_Light;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}