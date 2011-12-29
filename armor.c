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
	printw("%s (%d AC", this->name, this->armor_class);
	if (this->strength)
		printw(", %d STR", this->strength);
	if (this->intelligence)
		printw(", %d INT", this->intelligence);
	if (this->dexterity)
		printw(", %d DEX", this->dexterity);
	printw(")");
}

struct item unique_armors[] = {
	{ "Fuzzy Dice", B_WHITE, '#', ITEM_ARMOR, ARMOR_HEAD, 10, 0, -1, 0,
		0, 0, 0, {0, 0}, NULL, print },
};

struct item *create_random_armor(int level)
{
	int prefix = rand() % NUM_PREFIXES;
	int suffix = rand() % NUM_SUFFIXES;
	int rc;
	struct item *armor;
	const char *type;

	(void) level;

	armor = calloc(1, sizeof(*armor));
	assert(armor);

	armor->color = B_CYAN;
	armor->symbol = '(';
	armor->type = ITEM_ARMOR;
	armor->location = rand() % 3;

	switch (armor->location) {
	case ARMOR_HEAD:
		type = "Helmet";
		armor->armor_class = 1 + rand() % 2;
		break;
	case ARMOR_TORSO:
		type = "Leather Armor";
		armor->armor_class = 2 + rand() % 3;
		break;
	case ARMOR_FEET:
		type = "Boots";
		armor->armor_class = 1 + rand() % 2;
		break;
	}

	rc = asprintf(&armor->name, "%s %s %s",
		      prefixes[prefix], type, suffixes[suffix]);
	assert(rc != -1);

	armor->print = print;

	return armor;
}

