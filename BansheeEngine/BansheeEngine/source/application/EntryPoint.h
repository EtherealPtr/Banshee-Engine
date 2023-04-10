#pragma once

extern Banshee::Application* CreateApplication();

int main()
{
	auto app = CreateApplication();
	app->Run();

	delete app;
	app = nullptr;
}