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
	if (this->health <= 0) {
		printf("You die!\n");
		exit(0);
	}
}

void player_move(int x, int y, int z)
{
	player.world_level += z;
	player.world_x += x;
	player.world_y += y;
}

void level_up(struct creature *this)
{
	printf(B_MAGENTA "You gain a level!\n" NORMAL);

	this->level++;

	this->strength += rand() % 3 + 1;
	this->intelligence += rand() % 3 + 1;
	this->dexterity += rand() % 3 + 1;

	this->max_health += rand() % 10 + 5;
	this->max_mana += rand() % 5 + 3;
	this->health = this->max_health;
	this->mana = this->max_mana;
}

void player_give_experience(struct creature *this, int experience)
{
	this->experience += experience;
	if (this->experience >= 100 * this->level)
	{
		this->experience -= 100 * this->level;
		level_up(this);
	}
}

void init_player()
{
	memset(&player, 0, sizeof(player));

	player.self.color = B_RED;
	player.self.symbol = 'X';

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
	player.self.give_experience = player_give_experience;
	player.move = player_move;

	INIT_LIST_HEAD(&player.inventory);
}

void print_inventory()
{
	struct item *pos;
	printf("You have the following items:\n");

	list_for_each_entry(pos, &player.inventory, list) {
		printf("%s\n", pos->name);
	}
}
