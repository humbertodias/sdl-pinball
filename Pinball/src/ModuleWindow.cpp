#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
    LOG("Init SDL window & surface");
    bool ret = true;

    // Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
        ret = false;
    }
    else
    {
        // Check if screen dimensions are valid
        if (SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0 || SCREEN_SIZE <= 0)
        {
            LOG("Invalid screen dimensions or size. Check SCREEN_WIDTH, SCREEN_HEIGHT, and SCREEN_SIZE.\n");
            ret = false;
        }
        else
        {
            // Create the window
            int width = SCREEN_WIDTH * SCREEN_SIZE;
            int height = SCREEN_HEIGHT * SCREEN_SIZE;
            Uint32 flags = SDL_WINDOW_SHOWN;

            // Adjust flags based on settings
            if (WIN_FULLSCREEN)
            {
                flags |= SDL_WINDOW_FULLSCREEN;
            }

            if (WIN_RESIZABLE)
            {
                flags |= SDL_WINDOW_RESIZABLE;
            }

            if (WIN_BORDERLESS)
            {
                flags |= SDL_WINDOW_BORDERLESS;
            }

            if (WIN_FULLSCREEN_DESKTOP)
            {
                flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            }

            // Try to create the window
            window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

				if(window == NULL)
				{
					LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
					ret = false;
				}

			}
    }

    return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}