#include "game.h"

void slime_attack(struct creature *this, struct creature *player)
{
	printf("%s attacks you for %d damage!\n",
	       this->name, this->strength);

	player->do_hurt(player, this);
}

void slime_die(struct creature *this)
{
	struct room *room = current_room();
	int gold = rand() % 20;
	struct item *armor = create_random_armor();

	printf("%s drops %d gold!\n", this->name, gold);
	room->gold += gold;

	printf("%s drops %s!\n", this->name, armor->name);
	list_add_tail(&armor->list, &room->items);
}

void slime_hurt(struct creature *this, struct creature *hurter)
{
	this->health -= hurter->strength;
	if (this->health <= 0) {
		printf("%s dies!\n", this->name);
		this->die(this);
	}
}

struct creature *create_slime()
{
	int rc;
	struct creature *slime;

	slime = calloc(1, sizeof(struct creature));
	assert(slime);

	slime->color = GREEN;
	slime->symbol = 's';

	rc = asprintf(&slime->name, "a slime");
	assert(rc != -1);
	slime->health = slime->max_health = 50;
	slime->mana = slime->max_mana = 0;
	slime->strength = 4;
	slime->intelligence = 3;
	slime->dexterity = 3;

	slime->do_hurt = slime_hurt;
	slime->attack = slime_attack;
	slime->die = slime_die;

	return slime;
}
