#include "game.h"

struct player player;

char *prompt()
{
	char prompt[128];

	snprintf(prompt, sizeof(prompt), "%d/%d H, %d/%d M >",
		 player.self.health, player.self.max_health,
		 player.self.mana, player.self.max_mana);

	return readline(prompt);
}

int main()
{
	char *line;

	struct room room;
	struct creature *bat = create_bat();

	room.creatures = bat;
	room.items = NULL;

	init_player();

	while (1) {
		printf("In this room, there are:\n");
		printf("%s\n", room.creatures->name);

		line = prompt();

		if (strcmp(line, "attack") == 0) {
			player.self.attack(&player.self, room.creatures);
		} else if (strcmp(line, "quit") == 0) {
			break;
		}

		if (room.creatures->health > 0)
			room.creatures->attack(room.creatures,
					       &player.self);

		free(line);
	}

	free_creature(bat);

	return 0;
}

