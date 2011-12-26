#include "game.h"

const char *prefixes[] = {
	"Fuzzy", "Shiny", "Shimmering", "Glowing", "Metallic"
};

const char *suffixes[] = {
	"of the Bat", "of Light", "of Darkness",
	"of Destruction"
};

#define NUM_PREFIXES (sizeof(prefixes) / sizeof(prefixes[0]))
#define NUM_SUFFIXES (sizeof(suffixes) / sizeof(suffixes[0]))

struct item unique_armors[] = {
	{ "Fuzzy Dice", B_WHITE, '#', ITEM_ARMOR, ARMOR_HEAD, 10, 0, -1, 0,
		0, 0, {0, 0}, NULL },
};

void print_armor(struct item *armor)
{
	printw("%s (%d AC", armor->name, armor->armor_class);
	if (armor->strength)
		printw(", %d STR", armor->strength);
	if (armor->intelligence)
		printw(", %d INT", armor->intelligence);
	if (armor->dexterity)
		printw(", %d DEX", armor->dexterity);
	printw(")");
}

struct item *create_random_armor()
{
	int prefix = rand() % NUM_PREFIXES;
	int suffix = rand() % NUM_SUFFIXES;
	int rc;

	struct item *armor;

	armor = calloc(1, sizeof(*armor));
	assert(armor);

	armor->color = B_CYAN;
	armor->symbol = '(';
	rc = asprintf(&armor->name, "%s Leather Armor %s",
		      prefixes[prefix], suffixes[suffix]);
	assert(rc != -1);

	armor->type = ITEM_ARMOR;
	armor->location = ARMOR_TORSO;
	armor->armor_class = 1 + rand() % 2;

	return armor;
}

