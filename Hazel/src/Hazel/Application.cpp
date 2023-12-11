#include "Application.h"


namespace Hazel {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);

		HZ_TRACE(e);

		if (e.IsInCategory(EventCategoryInput))
		{
			HZ_TRACE(e);
		}

		while (true);
	}
}