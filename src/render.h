#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <SDL3/SDL.h>

#include "vector.h"
#include "vertex.h"

extern SDL_GPUDevice *device;
extern SDL_Window *window;

bool render_init();
bool render_upload(vertex vertices[], size_t num_vertices);
bool render_draw(size_t num_vertices);
void render_quit();

#endif
