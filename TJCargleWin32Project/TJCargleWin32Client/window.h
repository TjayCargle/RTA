#pragma once
#include <cstdint>
#include <cassert>
#include <utility>
#include <chrono>
#include "graphics.h"

namespace fsgd
{
	class window
	{
	public:
		struct point { int x, y; };

		struct properties
		{
			uint32_t width;
			uint32_t height;
			bool locked_aspect_ratio;
		};

		bool key_down[4]{ false, false, false, false };

	private:
		graphics gfx;

		using time_point_t = decltype(std::chrono::high_resolution_clock::now());

		time_point_t last_time;
		time_point_t curr_time;

		void set_cursor_pos(point p);

		size_t native_handle = { 0 };

		properties props{};

		window(size_t _native_handle, const properties& _props) : native_handle{ _native_handle }, props(_props) {}

		window(const window&) = default;
		window& operator=(const window&) = default;

		double accum_time{ 0.0 };

		static size_t _create(window* window, const properties& _props);

	public:
		struct
		{
			bool moved = false;
			bool lbutton_up = false;
			bool lbutton_down = false;
			bool rbutton_up = false;
			bool rbutton_down = false;
			point pos;
			point last_pos;
		}cursor_props;

		window() = default;

		~window();

		static void create(window* window, const properties& _props);

		const properties& get_properties()const { return props; }

		bool is_valid()const { return native_handle != 0; }

		size_t get_native_handle()const { return native_handle; }

		void show();

		void destroy();

		bool pump_events();

		void update();

		bool is_focus()const;

		bool move_window(int x, int y, int w, int h);

		void get_window_pos(int& x, int& y, int& w, int& h);
	};
}