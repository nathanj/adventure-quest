#include "game.h"

static struct drop_table drop_table[] = {
	{100,  500, 700, NULL},
	{100,  0,   0,   create_random_armor},
	{100,  0,   0,   create_random_weapon},
	{0,    0,   0,   NULL},
};

void dragon_attack(struct creature *this, struct creature *player)
{
	int damage;

	if (rand() % 2 && this->mana > 10) {
		damage = this->intelligence * 1.5;
		this->mana -= 10;
		message(NORMAL, "%s breathes fire!",
			this->name);
		message(NORMAL, "The fire deals %d damage to you!",
			damage);
		player->do_hurt(player, this, damage);
	} else {
		damage = this->strength;
		message(NORMAL, "%s attacks you for %d damage!",
			this->name, damage);
		player->do_hurt(player, this, damage);
	}
}

void dragon_hurt(struct creature *this, struct creature *hurter, int damage)
{
	this->health -= damage;
	if (this->health <= 0) {
		message(NORMAL, "%s dies!", this->name);
		message(B_GREEN, "You win! Hooray!");
		this->die(this);
		hurter->give_experience(hurter, this->experience);
	}
}


struct creature *create_dragon()
{
	int rc;
	struct creature *dragon;

	dragon = calloc(1, sizeof(*dragon));
	assert(dragon);

	dragon->color = B_RED;
	dragon->symbol = 'D';

	rc = asprintf(&dragon->name, "a dragon");
	assert(rc != -1);
	dragon->health = dragon->max_health = 500;
	dragon->mana = dragon->max_mana = 100;
	dragon->strength = 50;
	dragon->intelligence = 50;
	dragon->dexterity = 50;
	dragon->level = 10;
	dragon->experience = 1000;

	dragon->do_hurt = dragon_hurt;
	dragon->attack = dragon_attack;
	dragon->die = creature_die;

	dragon->drop_table = drop_table;

	return dragon;
}

