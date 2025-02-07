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

void check_collision(Circle_t * circle, SDL_Rect * player, int * alive, int * score)
{
	/*
		Checks if the perimeter of circle is touching outer wall,
		reverse direction if so
	*/

	if(circle->x + circle->r >= WIDTH)  // Bot side
	{
		circle->v_x = rand() % 3;
		circle->v_x *= -1;
		circle->v_y = rand() % 3;
		circle->v_y *= -1;
	}

	if(circle->y + circle->r >= HEIGHT || circle->y - circle->r <= 0)  // Roof & Floor
	{
		circle->v_y *= -1;
	}

	/*
		Checks if perimeter of circle is touching player paddle, reverse
		direction if so
	*/

	if(circle->x - circle->r <= 0)  // Player side
	{
		if(circle->y >= player->y && circle->y <= player->y + player->h)  // Middle of ball in paddle
		{
			circle->v_x *= -1;
			*score += 1;
		}
		else
		{
			SDL_Log("You Lost!\n");
			SDL_Log("Score: %d\n", *score);
			*alive = 0;
		}
	}
}

void move_circle(SDL_Surface * surface, Circle_t * circle, SDL_Rect * player, SDL_Rect * bot, int * score, int * alive)
{
	// Clear old circle, draw paddle back
	SDL_FillRect(surface, NULL, BLACK);
	SDL_FillRect(surface, player, WHITE);

	//update circle and bot paddle pos
	circle->x += circle->v_x;
	circle->y += circle->v_y;
	bot->y = circle->y - circle->r;

	//Check for collisions
	check_collision(circle, player, alive, score);
	fill_circle(surface, circle);
	SDL_FillRect(surface, bot, WHITE);
	SDL_Delay(3);  // Too fast without delay
}

void main()
{

	srand(time(NULL));

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface * surface = SDL_GetWindowSurface(window);

	// Create necessary objects
	Circle_t circle = {30, WIDTH/2, HEIGHT/2, (rand()%3)+1, (rand()%3)+1};
	SDL_Rect player = {0, HEIGHT/2, 10, 75};
	SDL_Rect bot = {WIDTH - 10, HEIGHT/2, 10, 75};
//	SDL_Rect test = {0, 0, 100, 100};

	//Draw objects to screen
//	SDL_FillRect(surface, &test, WHITE);
	SDL_FillRect(surface, &player, WHITE);
	SDL_FillRect(surface, &bot, WHITE);
	fill_circle(surface, &circle);

	//Update Screen
	SDL_UpdateWindowSurface(window);

//	SDL_Delay(3000);

	SDL_Event e;
	int running = 1;
	int score = 0;

	while(running)  // Run loop
	{

		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					SDL_Log("Quitting...\n");
					running = 0;
					break;
			}
		}

		move_circle(surface, &circle, &player, &bot, &score, &running);
		SDL_UpdateWindowSurface(window);

	}

	SDL_Quit();  // Close SDL/Subsystems
	exit(0);  // Exit
}
