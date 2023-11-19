#include <Banshee.h>
#include "Player.h"
#include "Enemy.h"

using namespace Banshee;

class ClientApp : public Application
{
public:
	ClientApp()
	{
	}

private:
	Player m_Player;
	Enemy m_Enemy;
};

Application* CreateApplication()
{
	return new ClientApp();
}