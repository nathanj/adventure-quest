#include "game.h"

struct room world[10][10][10];

void print_room(int i, int j)
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;

	struct room *room = &world[l][i][j];

	if (x == i && y == j)
		printf(B_RED "X");
	else if (room->creatures)
		printf("%s%c",
		       room->creatures->color,
		       room->creatures->symbol);
	else if (room->gold)
		printf(B_YELLOW "$");
	else if (room->stairs_down)
		printf(">");
	else if (room->stairs_up)
		printf("<");
	else
		printf(" ");

	printf(NORMAL);
}

void print_map()
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("[");
			print_room(i, j);
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

void room_remove_creature(struct creature *creature)
{
	struct room *room = current_room();

	(void) creature;

	room->creatures = NULL;
}


