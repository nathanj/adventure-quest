#include "game.h"

static const char *prefixes[] = {
	"Fuzzy", "Shiny", "Shimmering", "Glowing", "Metallic"
};

static const char *suffixes[] = {
	"of the Bat", "of Light", "of Darkness",
	"of Destruction"
};

#define NUM_PREFIXES (sizeof(prefixes) / sizeof(prefixes[0]))
#define NUM_SUFFIXES (sizeof(suffixes) / sizeof(suffixes[0]))

static void print(struct item *this)
{
	printw("%s (%d DR", this->name, this->damage_roll);
	if (this->strength)
		printw(", %d STR", this->strength);
	if (this->intelligence)
		printw(", %d INT", this->intelligence);
	if (this->dexterity)
		printw(", %d DEX", this->dexterity);
	printw(")");
}

struct item *create_random_weapon(int level)
{
	int prefix = rand() % NUM_PREFIXES;
	int suffix = rand() % NUM_SUFFIXES;
	int rc;
	struct item *weapon;

	weapon = calloc(1, sizeof(*weapon));
	assert(weapon);

	weapon->color = RED;
	weapon->symbol = ')';
	weapon->type = ITEM_WEAPON;
	weapon->damage_roll = rand_between(level * 5, level * 10);

	rc = asprintf(&weapon->name, "%s Sword %s",
		      prefixes[prefix], suffixes[suffix]);
	assert(rc != -1);

	weapon->print = print;

	return weapon;
}
