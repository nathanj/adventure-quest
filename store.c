#include "game.h"

int calc_price(struct item *item)
{
	if (item->type == ITEM_USE)
		return 1;
	else
		return 5;
}

void print_store_inventory(struct store *store)
{
	struct item *item;
	char c = 'a';

	amvprintw(NORMAL, 0, 0, "Welcome to %s!\n", store->name);
	aprintw(NORMAL, "The following items are for sale:\n");

	list_for_each_entry(item, &store->inventory, list) {
		aprintw(BOLD, "%c", c++);
		aprintw(NORMAL, ": ");
		aprintw(YELLOW, "[%d gold] ", calc_price(item));
		aprintw(NORMAL, "");
		if (item->type == ITEM_ARMOR)
			print_armor(item);
		else
			aprintw(NORMAL, "%s ", item->name);
		printw("\n");
	}

	aprintw(BOLD, "z");
	aprintw(NORMAL, ": Exit\n");
}

int buy_item(struct store *store, int i)
{
	struct item *item, *n;
	int price;

	list_for_each_entry_safe(item, n, &store->inventory, list) {
		if (i-- == 0) {
			price = calc_price(item);

			if (price > player.gold) {
				message(NORMAL, "You do not have enough gold!");
				return 1;
			}

			message(NORMAL, "You buy %s for %d gold.",
				item->name, price);

			player.gold -= price;
			list_del(&item->list);
			list_add_tail(&item->list, &player.inventory);

			return 1;
		}
	}

	return 0;
}

