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
	int v_x, v_y;

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

void check_collision(Circle_t * circle)
{
	/*
		Checks if the perimeter of circle is touching a wall,
		reverse direction if so
	*/

	if(circle->x + circle->r == WIDTH || circle->x - circle->r == 0)
	{
		circle->v_x *= -1;
	}

	if(circle->y + circle->r == HEIGHT || circle->y - circle->r == 0)
	{
		circle->v_y *= -1;
	}
}

void move_circle(SDL_Surface * surface, Circle_t * circle)
{
	SDL_FillRect(surface, NULL, BLACK);
	circle->x += circle->v_x;
	circle->y += circle->v_y;
	check_collision(circle);
	fill_circle(surface, circle);
}

void main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface * surface = SDL_GetWindowSurface(window);

	srand(time(NULL));

	Circle_t circle = {100, WIDTH/2, HEIGHT/2, 1, 1};

	fill_circle(surface, &circle);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;

	int running = 1;

	while(running)  // Run loop
	{

		while(SDL_PollEvent(&e))  // Just quit handling
		{
			switch(e.type)
			{
				case SDL_QUIT:
					SDL_Log("Quitting...\n");  // Log message
					running = 0;
					break;
			}
		}

		move_circle(surface, &circle);
		SDL_UpdateWindowSurface(window);

	}

	SDL_Quit();  // Close SDL/Subsystems
	exit(0);  // Exit
}
