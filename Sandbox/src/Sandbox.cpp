#include <ASSInterface.h>
#include "ASSInterface/Core/EntryPoint.h"
#include "MainForm.h"


class Sandbox : public ASSInterface::Application
{
public:
	Sandbox()
	{
		PushLayer(new MainForm());
	}

	~Sandbox()
	{

	}
};

ASSInterface::Application* ASSInterface::CreateApplication()
{
	return new Sandbox();
}