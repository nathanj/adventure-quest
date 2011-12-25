#include "game.h"

void use_healing_potion(struct item *this, struct player *player)
{
	if (this->health) {
		printf(B_GREEN "You heal %d health!\n", this->health);
		player->self.health += this->health;
		if (player->self.health > player->self.max_health)
			player->self.health = player->self.max_health;
	}

	if (this->mana) {
		printf(B_GREEN "You heal %d mana!\n", this->mana);
		player->self.mana += this->mana;
		if (player->self.mana > player->self.max_mana)
			player->self.mana = player->self.max_mana;
	}
}

struct item potions[] = {
	{ "Health Potion",       B_GREEN, '!', ITEM_USE, .health = 20,  .interact = use_healing_potion },
	{ "Mana Potion",         B_BLUE,  '!', ITEM_USE, .mana = 5,     .interact = use_healing_potion },
};

#define NUM_POTIONS (sizeof(potions) / sizeof(potions[0]))

struct item *create_random_potion()
{
	int n = rand() % NUM_POTIONS;
	struct item *potion;

	potion = malloc(sizeof(*potion));

	memcpy(potion, &potions[n], sizeof(*potion));

	return potion;
}

