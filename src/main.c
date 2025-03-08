#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sdl.h"
#include "render.h"
#include "vertex.h"

bool init()
{
	if (!sdl_init()) return false;
	if (!render_init()) return false;
	
	return true;
}

void quit()
{
	render_quit();
	sdl_quit();
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (!init()) return 1;

	vertex vertices[] = 
	{
		{{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
	};

	size_t num_vertices = sizeof(vertices) / sizeof(vertex);
	
	if (!render_upload(vertices, num_vertices)) return 1;

	SDL_Event event;

	while (true)
	{
		while (SDL_PollEvent(&event))
		{
			if(event.type == SDL_EVENT_QUIT)
			{
				quit();
				return 0;
			}
		}

		if (!render_draw(num_vertices)) return 1;
	}

	quit();
	
	return 0;
}
