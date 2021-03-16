#include <SDL.h>
#include <stdio.h>
#include <string>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* surface = NULL;
SDL_Texture* texture = NULL;

int Init()
{
	int result = 1;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init failed: %s\n", SDL_GetError());
		result = 0;
	}

	window = SDL_CreateWindow("Idea Board", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created: %s\n", SDL_GetError());
		result = 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf("Renderer could not be created: %s\n", SDL_GetError());
	}

	surface = SDL_LoadBMP("butt.bmp");
	if (surface == NULL)
	{
		printf("Could not load BMP %s: %s\n", "butt.bmp", SDL_GetError());
		result = 0;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
	{
		printf("Could not create texture from surface: %s\n", SDL_GetError());
	}
	SDL_FreeSurface(surface);

	return result;
}

void Quit()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	int init = Init();

	// Temporary for testing
	SDL_Rect rect;
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(window, &w, &h);
	rect.x = 50;
	rect.y = 0;
	rect.w = w / 2;
	rect.h = h;

	bool quit = false;
	SDL_Event event;

	// Mouse drag testing
	bool mouseDown = false;
	int mouseX = 0;
	int mouseY = 0;
	int mouseOffsetX = 0;
	int mouseOffsetY = 0;
	
	if (init != 0)
	{
		while (!quit)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					quit = true;
				}

				if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				{
					mouseDown = false;
				}
				// Testing and figuring out how mouse events work
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				{
					// if the user is clicking the testing rect
					if (event.button.x >= rect.x && event.button.x <= rect.x + rect.w && event.button.y >= rect.y && event.button.y <= rect.y + rect.h)
					{
						mouseDown = true;
						mouseOffsetX = event.button.x - rect.x;
						mouseOffsetY = event.button.y - rect.y;
					}
				}
			}

			if (mouseDown)
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				rect.x = mouseX - mouseOffsetX;
				rect.y = mouseY - mouseOffsetY;
			}

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			SDL_RenderPresent(renderer);

			// So the program doesn't hog CPU
			SDL_Delay(8);
		}
	}

	Quit();

	return 0;
}

//Could be useful
/*SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	rect.x = 0;
	rect.y = 0;
	rect.w = mode.w / 2;
	rect.h = mode.h;*/