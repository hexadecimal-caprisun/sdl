#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define HEIGHT 720
#define WIDTH 1280
#define WHITE 0xffffffff
#define BLACK 0x00000000


typedef struct
{

	int r, x, y;

} Circle_t;

void fill_circle(SDL_Surface * surface, Circle_t * circle)
{
	for(int x2 = circle->x - circle->r; x2 <= circle->x + circle->r; x2++)
	{
		for(int y2 = circle->y - circle->r; y2 <= circle->y + circle->r; y2++)
		{
			if((x2 - circle->x)*(x2 - circle->x) + (y2 - circle->y)*(y2 - circle->y) <= circle->r * circle->r)
			{
				SDL_Rect temp = {x2, y2, 1, 1};
				SDL_FillRect(surface, &temp, WHITE);
			}
		}
	}

}

void main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface * surface = SDL_GetWindowSurface(window);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;

	int running = 1;

	while(running)
	{
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					SDL_Log("Quitting...\n");  // Log message
					running = 0;
					break;

				case SDL_MOUSEBUTTONDOWN:
					SDL_FillRect(surface, NULL, BLACK);  // Clear Screen
					Circle_t temp = {50, e.button.x, e.button.y};  // Create temp circle
					fill_circle(surface, &temp);  // Fill temp circle
					SDL_UpdateWindowSurface(window);  // Update window
					break;
			}
		}
	}

	SDL_Quit();  // Close SDL/Subsystems
	exit(0);  // Exit
}
