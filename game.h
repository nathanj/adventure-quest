#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include <curses.h>

#include "list.h"
#include "colors.h"

struct drop_table {
	int probability;

	int min_gold;
	int max_gold;

	struct item *(*creator) (int level);
};

struct creature {
	int color;
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
	void (*do_hurt) (struct creature *this, struct creature *hurter, int damage);
	void (*die) (struct creature *this);
	void (*give_experience) (struct creature *this, int experience);

	struct drop_table *drop_table;
};

struct player {
	struct creature self;

	int gold;

	int world_level;
	int world_x;
	int world_y;

	struct item *weapon;
	struct item *armor_head;
	struct item *armor_torso;
	struct item *armor_feet;

	int num_items;
	struct list_head inventory;

	void (*go) (int x, int y, int z);
	void (*equip) (struct player *this, struct item *item);
};

struct store {
	int color;
	char symbol;
	char *name;

	struct list_head inventory;
};

enum item_type {
	ITEM_USE,
	ITEM_ARMOR,
	ITEM_WEAPON
};

enum armor_location {
	ARMOR_HEAD,
	ARMOR_TORSO,
	ARMOR_FEET,
};

struct item {
	char *name;
	int color;
	char symbol;

	enum item_type type;

	enum armor_location location;
	int armor_class;
	int strength;
	int intelligence;
	int dexterity;

	int damage_roll;

	int health;
	int mana;

	struct list_head list;

	void (*interact) (struct item *this, struct player *player);
	void (*print) (struct item *this);
};

struct room {
	int stairs_down : 1;
	int stairs_up : 1;

	int gold;

	struct store *store;

	struct list_head creatures;
	struct list_head items;
};

struct item *create_random_potion(int level);
struct item *create_random_armor(int level);
struct item *create_random_weapon(int level);
void print_armor(struct item *armor);
void print_weapon(struct item *weapon);

void free_creature(struct creature *creature);

struct creature *create_bat();
struct creature *create_slime();
struct creature *create_skeleton();
struct creature *create_bear();
struct creature *create_dragon();

void init_player();

void init_world();
void print_map();
struct room *current_room();
void room_remove_dead_creatures();
void print_inventory();
void print_current_room_contents();
int p(int probability);
int rand_between(int a, int b);
void print_store_inventory(struct store *store);
int buy_item(struct store *store, int i);
void init_messages();
void message(int color, const char *fmt, ...)
	__attribute__((format(printf,2,3)));
void print_messages();
void finish(int sig);
int use_item(int i);
int drop_item(int i);

void creature_die(struct creature *this);
void creature_hurt(struct creature *this, struct creature *player, int damage);
void creature_attack(struct creature *this, struct creature *hurter);

extern struct player player;

#endif /* __GAME_H__ */

