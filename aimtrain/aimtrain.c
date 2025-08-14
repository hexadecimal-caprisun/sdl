#include <SDL2/SDL_log.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

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

int check_hitbox(Circle_t * target, int x_pos, int y_pos)
{ 
	if((x_pos - target->x)*(x_pos - target->x) + (y_pos - target->y)*(y_pos - target->y) <= target->r * target->r) return 1;
	return 0;
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface * surface = SDL_GetWindowSurface(window);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;

	int running = 1;

	srand(time(NULL));

	double total_clicks = 0;
	double hit_shots = 0;

	Circle_t target = {50, (rand() % WIDTH) + 1, (rand() % HEIGHT) + 1};
	fill_circle(surface, &target);
	SDL_UpdateWindowSurface(window);

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
					total_clicks++;
					if(check_hitbox(&target, e.button.x, e.button.y))
					{
						hit_shots++;
						SDL_FillRect(surface, NULL, BLACK);
						target.x = (rand() % WIDTH) + 1;
						target.y = (rand() % HEIGHT) + 1;
						fill_circle(surface, &target);
						SDL_UpdateWindowSurface(window);
					}
					break;
			}
		}
	}

	SDL_Log("You fired %d shots, landing %d of them, leaving you with an accuracy of %%%.2lf!", (int)total_clicks, (int)hit_shots, (hit_shots/total_clicks) * 100);

	SDL_Quit();  // Close SDL/Subsystems
	return 0;
}
