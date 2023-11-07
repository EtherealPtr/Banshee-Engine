#include <Banshee.h>
#include "Player.h"

using namespace Banshee;

class ClientApp : public Application
{
public:
	ClientApp()
	{

	}

private:
	Player m_Player;
};

Application* CreateApplication()
{
	return new ClientApp();
}