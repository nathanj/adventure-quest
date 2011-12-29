#include "game.h"

static void use_healing_potion(struct item *this, struct player *player)
{
	if (this->health) {
		message(GREEN, "You heal %d health!", this->health);
		player->self.health += this->health;
		if (player->self.health > player->self.max_health)
			player->self.health = player->self.max_health;
	}

	if (this->mana) {
		message(GREEN, "You heal %d mana!", this->mana);
		player->self.mana += this->mana;
		if (player->self.mana > player->self.max_mana)
			player->self.mana = player->self.max_mana;
	}
}

static void print(struct item *this)
{
	aprintw(this->color, "%s", this->name);
}

struct item potions[] = {
	{ "Health Potion", B_GREEN, '!', ITEM_USE, .health = 20,
		.interact = use_healing_potion, .print = print },
	{ "Mana Potion",   B_BLUE,  '!', ITEM_USE, .mana = 5,
		.interact = use_healing_potion, .print = print },
};

#define NUM_POTIONS (sizeof(potions) / sizeof(potions[0]))

struct item *create_random_potion(int level)
{
	int n = rand() % NUM_POTIONS;
	struct item *potion;

	(void) level;

	potion = malloc(sizeof(*potion));

	memcpy(potion, &potions[n], sizeof(*potion));

	return potion;
}

