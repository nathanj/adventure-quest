#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "list.h"
#include "colors.h"

struct creature {
	const char *color;
	char symbol;
	char *name;

	int health, max_health;
	int mana, max_mana;
	int strength;
	int intelligence;
	int dexterity;
	int level;

	void (*attack) (struct creature *this, struct creature *other);
	void (*do_hurt) (struct creature *this, struct creature *hurter);
};

struct player {
	struct creature self;

	int gold;

	int world_level;
	int world_x;
	int world_y;

	struct list_head inventory;

	void (*move) (int x, int y, int z);
};

enum item_type {
	ITEM_USE,
	ITEM_ARMOR
};

struct item {
	char *name;
	enum item_type type;

	int armour_class;
	int strength;
	int intelligence;
	int dexterity;

	struct list_head list;

	void (*interact) (struct player *player);
};

struct room {
	int stairs_down : 1;
	int stairs_up : 1;

	int gold;

	struct creature *creatures;
	struct item *items;
};

struct item *create_random_armor();
void print_armor(struct item *armor);

void free_creature(struct creature *creature);

struct creature *create_bat();

void init_player();

void init_world();
void print_map();
struct room *current_room();
void room_remove_creature(struct creature *creature);
void print_inventory();

extern struct player player;

#endif /* __GAME_H__ */

