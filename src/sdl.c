#include "sdl.h"

SDL_Window *window;
SDL_GPUDevice *device;

const char *title = "game";
const int width = 800, height = 600;

bool sdl_init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

	window = SDL_CreateWindow(title, width, height, 0);

    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, 0, NULL);

    if (device == NULL)
    {
        SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_ClaimWindowForGPUDevice(device, window))
    {
        SDL_Log("SDL_ClaimWindowForGPUDevice failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

void sdl_quit()
{
	SDL_ReleaseWindowFromGPUDevice(device, window);
	SDL_DestroyGPUDevice(device);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
