#include "game.h"

struct player player;

void init_player()
{
	player.health = 30;
	player.max_health = 30;
	player.mana = 10;
	player.max_mana = 10;

	player.level = 1;
	player.strength = 10;
	player.intelligence = 5;
	player.dexterity = 8;
}

int main()
{
	char *line;

	print_armor(create_random_armor());
	print_armor(create_random_armor());
	print_armor(create_random_armor());

	while (1) {
		line = readline("> ");
		printf("the line is %s\n", line);
		if (strcmp(line, "quit") == 0)
			break;
		free(line);
	}

	return 0;
}

