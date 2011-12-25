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
	int experience;

	struct list_head list;

	void (*attack) (struct creature *this, struct creature *other);
	void (*do_hurt) (struct creature *this, struct creature *hurter);
	void (*die) (struct creature *this);
	void (*give_experience) (struct creature *this, int experience);
};

struct player {
	struct creature self;

	int gold;

	int world_level;
	int world_x;
	int world_y;

	struct item *armor_head;
	struct item *armor_torso;
	struct item *armor_feet;

	struct list_head inventory;

	void (*move) (int x, int y, int z);
};

struct store {
	const char *color;
	char symbol;
	char *name;

	struct list_head inventory;
};

enum item_type {
	ITEM_USE,
	ITEM_ARMOR
};

enum armor_location {
	ARMOR_HEAD,
	ARMOR_TORSO,
	ARMOR_FEET,
};

struct item {
	char *name;
	const char *color;
	char symbol;

	enum item_type type;

	enum armor_location location;
	int armour_class;
	int strength;
	int intelligence;
	int dexterity;

	int health;
	int mana;

	struct list_head list;

	void (*interact) (struct item *this, struct player *player);
};

struct room {
	int stairs_down : 1;
	int stairs_up : 1;

	int gold;

	struct store *store;

	struct list_head creatures;
	struct list_head items;
};

struct item *create_random_armor();
void print_armor(struct item *armor);

void free_creature(struct creature *creature);

struct creature *create_bat();
struct creature *create_slime();

void init_player();

void init_world();
void print_map();
struct room *current_room();
void room_remove_dead_creatures();
void print_inventory();
void print_current_room_contents();
struct item *create_random_potion();
int p(int probability);
void print_store_inventory(struct store *store);
void buy_item(struct store *store);

extern struct player player;

#endif /* __GAME_H__ */

