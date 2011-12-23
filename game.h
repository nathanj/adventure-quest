#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <readline/readline.h>
#include <readline/history.h>

struct player {
	int health, max_health;
	int mana, max_mana;
	int strength;
	int intelligence;
	int dexterity;
	int level;

	struct item *inventory;
};

struct item {
	char *name;
	void (*interact) (struct player *player);
};

struct armor {
	struct item item;
	int armour_class;
	int strength;
	int intelligence;
	int dexterity;
};

struct armor *create_random_armor();
void print_armor(struct armor *armor);

#endif /* __GAME_H__ */

