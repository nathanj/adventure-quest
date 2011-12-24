#include "game.h"

struct player player;

char *prompt()
{
	char prompt[128];

	snprintf(prompt, sizeof(prompt),
		 GREEN "%d/%d " NORMAL "H, "
		 BLUE "%d/%d " NORMAL "M, "
		 YELLOW "%d " NORMAL "G "
		 "> ",
		 player.self.health, player.self.max_health,
		 player.self.mana, player.self.max_mana,
		 player.gold);

	return readline(prompt);
}

int main()
{
	char *line;
	struct room *room;

	init_world();
	init_player();

	while (1) {
		room = current_room();

		printf("\n\n");
		print_map();

		printf("In this room, there are:\n");
		if (room->creatures)
			printf("%s\n", room->creatures->name);
		else
			printf("Nothing!\n");

		line = prompt();

		if (strcmp(line, "attack") == 0 || strcmp(line, "a") == 0) {
			if (room->creatures) {
				player.self.attack(&player.self, room->creatures);

				if (room->creatures->health > 0)
					room->creatures->attack(room->creatures,
								&player.self);
			} else {
				printf("Attack who?\n");
			}
		} else if (strcmp(line, "e") == 0) {
			player.move(0, 1, 0);
		} else if (strcmp(line, "w") == 0) {
			player.move(0, -1, 0);
		} else if (strcmp(line, "n") == 0) {
			player.move(-1, 0, 0);
		} else if (strcmp(line, "s") == 0) {
			player.move(1, 0, 0);
		} else if (strcmp(line, "u") == 0) {
			player.move(0, 0, -1);
		} else if (strcmp(line, "d") == 0) {
			player.move(0, 0, 1);
		} else if (strcmp(line, "q") == 0) {
			free(line);
			break;
		} else {
			printf("Unknown command!\n");
		}

		free(line);
	}

	return 0;
}

