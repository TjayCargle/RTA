#include "window.h"

#if defined(_WIN32)
#include "window_win64.hpp"
#endif

#include <cstdlib>


namespace fsgd
{
	void window::create(window* window, const properties& _props)
	{
		window->props = _props;
		window->native_handle = _create(window, _props);
		window->gfx.initialize(window);

		window->last_time = std::chrono::high_resolution_clock::now();

		window->update();
		window->show();
	}

	window::~window()
	{
		destroy();
	}

	void window::update()
	{
		curr_time = std::chrono::high_resolution_clock::now();

		double delta_time = std::chrono::duration<double>(curr_time - last_time).count();

		last_time = curr_time;

		accum_time += delta_time;

		gfx.render_frame();
	}
}