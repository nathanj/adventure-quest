#include "game.h"

void player_attack(struct creature *this, struct creature *creature)
{
	printf("You attack %s for %d damage!\n",
	       creature->name, this->strength);

	creature->do_hurt(creature, this);
}

void player_hurt(struct creature *this, struct creature *hurter)
{
	this->health -= hurter->strength;
	if (this->health <= 0)
		printf("You die!\n");
}

void player_move(int x, int y, int z)
{
	player.world_level += z;
	player.world_x += x;
	player.world_y += y;
}

void init_player()
{
	memset(&player, 0, sizeof(player));

	player.self.health = 30;
	player.self.max_health = 30;
	player.self.mana = 10;
	player.self.max_mana = 10;

	player.self.level = 1;
	player.self.strength = 10;
	player.self.intelligence = 5;
	player.self.dexterity = 8;

	player.self.attack = player_attack;
	player.self.do_hurt = player_hurt;
	player.move = player_move;
}

