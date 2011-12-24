#include "game.h"

struct player player;

char *prompt()
{
	char prompt[128];

	snprintf(prompt, sizeof(prompt),
		 GREEN "%d/%d " NORMAL "Health, "
		 BLUE "%d/%d " NORMAL "Mana, "
		 YELLOW "%d " NORMAL "Gold "
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

		if (room->gold)
			printf(YELLOW "%d gold coins.\n" NORMAL, room->gold);

		if (room->items)
			printf("%s\n", room->items->name);

		line = prompt();

		if (strcmp(line, "attack") == 0 || strcmp(line, "a") == 0) {
			if (room->creatures) {
				player.self.attack(&player.self,
						   room->creatures);

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
		} else if (strcmp(line, "i") == 0) {
			print_inventory();
		} else if (strcmp(line, "t") == 0) {
			if (room->gold) {
				printf("You take %d gold coins.\n", room->gold);
				player.gold += room->gold;
				room->gold = 0;
			}

			if (room->items) {
				printf("You take %s.\n", room->items->name);
				list_add_tail(&room->items->list, &player.inventory);
				room->items = NULL;
			}
		} else if (strcmp(line, "q") == 0) {
			free(line);
			break;
		} else {
			printf("Unknown command!\n");
		}

		if (room->creatures && room->creatures->health <= 0)
			room_remove_creature(room->creatures);

		free(line);
	}

	return 0;
}

