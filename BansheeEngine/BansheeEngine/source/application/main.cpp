#include <Banshee.h>

using namespace Banshee;

class ClothSimulatorApp : public Application
{
public:
	ClothSimulatorApp()
	{

	}

	~ClothSimulatorApp()
	{

	}
};

Application* CreateApplication()
{
	return new ClothSimulatorApp();
}