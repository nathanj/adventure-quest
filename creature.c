#include "game.h"

void creature_die(struct creature *this)
{
	struct room *room = current_room();
	struct drop_table *drop;
	int gold;
	struct item *item;

	for (drop = this->drop_table; drop->probability > 0; drop++) {
		if (!p(drop->probability))
			continue;

		if (drop->min_gold > 0) {
			gold = rand_between(drop->min_gold, drop->max_gold);
			message(NORMAL, "%s drops %d gold!", this->name, gold);
			room->gold += gold;
		} else {
			item = drop->creator();
			message(NORMAL, "%s drops %s!", this->name, item->name);
			list_add_tail(&item->list, &room->items);
		}
	}
}

void creature_attack(struct creature *this, struct creature *player)
{
	message(NORMAL, "%s attacks you for %d damage!",
		this->name, this->strength);

	player->do_hurt(player, this);
}

void creature_hurt(struct creature *this, struct creature *hurter)
{
	this->health -= hurter->strength;
	if (this->health <= 0) {
		message(NORMAL, "%s dies!", this->name);
		this->die(this);
		hurter->give_experience(hurter, this->experience);
	}
}

void free_creature(struct creature *creature)
{
	free(creature->name);
	creature->name = NULL;

	free(creature);
}
