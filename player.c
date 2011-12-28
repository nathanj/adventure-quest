#include "game.h"

struct player player;

void player_attack(struct creature *this, struct creature *creature)
{
	message(NORMAL, "You attack %s for %d damage!",
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
		message(NORMAL, "Your armor absorbs some of the blow.");
		this->health -= hurter->strength - ac;
	} else {
		message(NORMAL, "Your armor absorbs all of the blow!");
	}

	if (this->health <= 0) {
		message(B_RED, "You die!");
	}
}

void player_go(int x, int y, int z)
{
	struct room *room = current_room();

	if (z > 0 && room->stairs_down)
		player.world_level += z;
	if (z < 0 && room->stairs_up)
		player.world_level += z;

	if (x > 0 && player.world_x < 9)
		player.world_x += x;
	if (x < 0 && player.world_x > 0)
		player.world_x += x;

	if (y > 0 && player.world_y < 9)
		player.world_y += y;
	if (y < 0 && player.world_y > 0)
		player.world_y += y;
}

void level_up(struct creature *this)
{
	message(MAGENTA, "You gain a level!");

	this->level++;

	this->strength += rand() % 2 + 1;
	this->intelligence += rand() % 2 + 1;
	this->dexterity += rand() % 2 + 1;

	this->max_health += rand() % 8 + 3;
	this->max_mana += rand() % 3 + 1;
	this->health = this->max_health;
	this->mana = this->max_mana;
}

void player_give_experience(struct creature *this, int experience)
{
	this->experience += experience;
	if (this->experience >= 100 * this->level) {
		this->experience -= 100 * this->level;
		level_up(this);
	}
}

void player_equip(struct player *this, struct item *item)
{
	switch (item->location) {
	case ARMOR_HEAD:
		message(NORMAL, "You equip %s on your head.",
			item->name);
		this->armor_head = item;
		break;
	case ARMOR_TORSO:
		message(NORMAL, "You equip %s on your torso.",
			item->name);
		this->armor_torso = item;
		break;
	case ARMOR_FEET:
		message(NORMAL, "You equip %s on your feet.",
			item->name);
		this->armor_feet = item;
		break;
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
	player.go = player_go;
	player.equip = player_equip;

	player.num_items = 0;
	INIT_LIST_HEAD(&player.inventory);
}

void print_inventory()
{
	struct item *item;
	char c = 'a';

	amvprintw(NORMAL, 0, 0, "Inventory:\n");

	list_for_each_entry(item, &player.inventory, list) {
		aprintw(BOLD, "%c", c++);
		aprintw(NORMAL, ": ");

		if (item->type == ITEM_ARMOR) {
			if (item == player.armor_head)
				aprintw(NORMAL, "(Equipped on head) ");
			else if (item == player.armor_torso)
				aprintw(NORMAL, "(Equipped on torso) ");
			else if (item == player.armor_feet)
				aprintw(NORMAL, "(Equipped on feet) ");

			print_armor(item);
			printw("\n");
		} else {
			aprintw(NORMAL, "%s\n", item->name);
		}
	}

	aprintw(BOLD, "z");
	aprintw(NORMAL, ": Exit\n");
}

int use_item(int i)
{
	struct item *item, *n;

	list_for_each_entry_safe(item, n, &player.inventory, list) {
		if (i-- == 0) {
			switch (item->type) {
			case ITEM_USE:
				message(NORMAL, "You use %s.", item->name);
				item->interact(item, &player);
				list_del(&item->list);
				player.num_items--;
				return 1;
				break;
			case ITEM_ARMOR:
				player.equip(&player, item);
				return 1;
				break;
			}
		}
	}

	return 0;
}

int drop_item(int i)
{
	struct item *item, *n;
	struct room *room = current_room();

	list_for_each_entry_safe(item, n, &player.inventory, list) {
		if (i-- == 0) {
			message(NORMAL, "You drop %s.", item->name);

			list_del(&item->list);
			player.num_items--;
			list_add_tail(&item->list, &room->items);

			return 1;
		}
	}

	return 0;
}
