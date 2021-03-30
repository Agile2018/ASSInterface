#pragma once


#ifdef ASS_PLATFORM_WINDOWS

extern ASSInterface::Application* ASSInterface::CreateApplication();

int main(int argc, char** argv)
{
	
	ASSInterface::Log::Init();
	ASS_ERROR_PROFILE_BEGIN_SESSION();
	ASS_PROFILE_BEGIN_SESSION("Startup", "ASSInterfaceProfile-Startup.json");
	auto app = ASSInterface::CreateApplication();
	ASS_PROFILE_END_SESSION();

	ASS_PROFILE_BEGIN_SESSION("Runtime", "ASSInterfaceProfile-Runtime.json");
	app->Run();
	ASS_PROFILE_END_SESSION();

	ASS_PROFILE_BEGIN_SESSION("Startup", "ASSInterfaceProfile-Shutdown.json");
	delete app;
	ASS_PROFILE_END_SESSION();
	ASS_ERROR_PROFILE_END_SESSION();
}

#endif