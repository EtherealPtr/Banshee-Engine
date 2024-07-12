#pragma once

#include <memory>

extern std::unique_ptr<Banshee::Application> CreateApplication();

int main()
{
	const std::unique_ptr<Banshee::Application>& app = CreateApplication();
	app->InitializeRenderer();
	app->Run();
}