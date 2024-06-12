#include <Banshee.h>
#include "Player.h"
#include "Ground.h"

using namespace Banshee;

class ClientApp : public Application
{
public:
	ClientApp()
	{
		
	}

private:
	Ground m_Ground;
	Ground2 m_Ground2;
	Player m_Player;
};

Application* CreateApplication()
{
	return new ClientApp();
}