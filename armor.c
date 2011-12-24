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
	{ "Fuzzy Dice", B_WHITE, '#', ITEM_ARMOR, ARMOR_HEAD, 10, 0, -1, 0 },
};

void print_armor(struct item *armor)
{
	printf("%s (%d AC", armor->name, armor->armour_class);
	if (armor->strength)
		printf(", %d STR", armor->strength);
	if (armor->intelligence)
		printf(", %d INT", armor->intelligence);
	if (armor->dexterity)
		printf(", %d DEX", armor->dexterity);
	printf(")\n");
}

struct item *create_random_armor()
{
	int prefix = rand() % NUM_PREFIXES;
	int suffix = rand() % NUM_SUFFIXES;
	int rc;

	struct item *armor;

	armor = calloc(1, sizeof(struct item));
	assert(armor);

	armor->color = B_CYAN;
	armor->symbol = '(';
	rc = asprintf(&armor->name, "%s Leather Armor %s",
		      prefixes[prefix], suffixes[suffix]);
	assert(rc != -1);

	armor->location = ARMOR_TORSO;
	armor->armour_class = 3 + rand() % 10;

	return armor;
}

