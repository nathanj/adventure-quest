#include "game.h"

struct room world[10][10][10];

void print_map()
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("[");
			if (x == i && y == j)
				printf(B_RED "X" NORMAL);
			else if (world[l][i][j].creatures)
				printf(B_BLUE "x" NORMAL);
			else if (world[l][i][j].stairs_down)
				printf(">");
			else if (world[l][i][j].stairs_up)
				printf("<");
			else
				printf(" ");
			printf("]");
		}
		printf("\n");
	}
}

void create_stairs()
{
	int l, i, j;

	for (l = 0; l < 9; l++) {
retry:
		i = rand() % 10;
		j = rand() % 10;

		/* Cannot have both sets of stairs on same room. */
		if (world[l][i][j].stairs_up)
			goto retry;

		world[l][i][j].stairs_down = 1;
		world[l+1][i][j].stairs_up = 1;
	}
}

void create_monsters()
{
	int num = rand() % 10;
	int n, i, j;

	for (n = 0; n < num; n++) {
		i = rand() % 10;
		j = rand() % 10;

		world[0][i][j].creatures = create_bat();
	}
}

void init_world()
{
	memset(world, 0, sizeof(world));

	create_stairs();
	create_monsters();
}

struct room *current_room()
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;

	return &world[l][x][y];
}

