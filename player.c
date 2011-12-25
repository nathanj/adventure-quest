#include "game.h"

void player_attack(struct creature *this, struct creature *creature)
{
	printf("You attack %s for %d damage!\n",
	       creature->name, this->strength);

	creature->do_hurt(creature, this);
}

int armor_class(struct player *this)
{
	int ac = 0;

	if (this->armor_head)
		ac += this->armor_head->armor_class;
	if (this->armor_torso)
		ac += this->armor_torso->armor_class;
	if (this->armor_feet)
		ac += this->armor_feet->armor_class;

	return ac;
}

void player_hurt(struct creature *this, struct creature *hurter)
{
	int ac;

	ac = armor_class((struct player *) this);
	if (ac == 0) {
		this->health -= hurter->strength;
	} else if (ac < hurter->strength) {
		printf("Your armor absorbs some of the blow.\n");
		this->health -= hurter->strength - ac;
	} else {
		printf("Your armor absorbs all of the blow!\n");
	}

	if (this->health <= 0) {
		printf("You die!\n");
		exit(0);
	}
}

void player_move(int x, int y, int z)
{
	player.world_level += z;
	player.world_x += x;
	player.world_y += y;
}

void level_up(struct creature *this)
{
	printf(B_MAGENTA "You gain a level!\n" NORMAL);

	this->level++;

	this->strength += rand() % 3 + 1;
	this->intelligence += rand() % 3 + 1;
	this->dexterity += rand() % 3 + 1;

	this->max_health += rand() % 10 + 5;
	this->max_mana += rand() % 5 + 3;
	this->health = this->max_health;
	this->mana = this->max_mana;
}

void player_give_experience(struct creature *this, int experience)
{
	this->experience += experience;
	if (this->experience >= 100 * this->level)
	{
		this->experience -= 100 * this->level;
		level_up(this);
	}
}

void player_equip(struct player *this)
{
	struct item *item;

	list_for_each_entry(item, &player.inventory, list) {
		if (item->type == ITEM_ARMOR) {
			switch (item->location) {
			case ARMOR_HEAD:
				printf("You equip %s on your head.\n",
				       item->name);
				this->armor_head = item;
				break;
			case ARMOR_TORSO:
				printf("You equip %s on your torso.\n",
				       item->name);
				this->armor_torso = item;
				break;
			case ARMOR_FEET:
				printf("You equip %s on your feet.\n",
				       item->name);
				this->armor_feet = item;
				break;
			}
		}
	}
}

void init_player()
{
	memset(&player, 0, sizeof(player));

	player.self.color = B_RED;
	player.self.symbol = 'X';

	player.self.health = 30;
	player.self.max_health = 30;
	player.self.mana = 10;
	player.self.max_mana = 10;

	player.self.level = 1;
	player.self.strength = 10;
	player.self.intelligence = 5;
	player.self.dexterity = 8;

	player.self.attack = player_attack;
	player.self.do_hurt = player_hurt;
	player.self.give_experience = player_give_experience;
	player.move = player_move;
	player.equip = player_equip;

	INIT_LIST_HEAD(&player.inventory);
}

void print_inventory()
{
	struct item *item;
	printf("You have the following items:\n");

	list_for_each_entry(item, &player.inventory, list) {
		if (item == player.armor_head)
			printf("(Equipped on head) ");
		else if (item == player.armor_torso)
			printf("(Equipped on torso) ");
		else if (item == player.armor_feet)
			printf("(Equipped on feet) ");

		if (item->type == ITEM_ARMOR)
			print_armor(item);
		else
			printf("%s\n", item->name);
	}
}
