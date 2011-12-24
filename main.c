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
	struct creature *creature;

	init_world();
	init_player();

	while (1) {
		room = current_room();

		printf("\n\n");
		print_map();

		printf("In this room, there are:\n");
		print_current_room_contents();

		line = prompt();

		if (strcmp(line, "attack") == 0 || strcmp(line, "a") == 0) {
			if (!list_empty(&room->creatures)) {
				int first = 1;

				list_for_each_entry(creature, &room->creatures, list) {
					if (first) {
						player.self.attack(&player.self,
								   creature);
						first = 0;
					}

					if (creature->health > 0)
						creature->attack(creature,
								 &player.self);
				}
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

			if (!list_empty(&room->items)) {
				struct item *item, *n;

				list_for_each_entry_safe(item, n, &room->items, list) {
					printf("You take %s.\n", item->name);
					list_add_tail(&item->list, &player.inventory);
				}

				INIT_LIST_HEAD(&room->items);
			}
		} else if (strcmp(line, "q") == 0) {
			free(line);
			break;
		} else {
			printf("Unknown command!\n");
		}

		room_remove_dead_creatures();

		free(line);
	}

	return 0;
}

