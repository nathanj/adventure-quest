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
	printf("%s\n", store->name);
	printf("The following items are for sale:\n");

	list_for_each_entry(item, &store->inventory, list) {
		printf("%s " YELLOW "[%d gold]\n" NORMAL, item->name, calc_price(item));
	}
}

void buy_item(struct store *store)
{
	struct item *item;
	int price;

	if (list_empty(&store->inventory)) {
		printf("There are no items to purchase!\n");
		return;
	}

	item = list_entry(store->inventory.next, struct item, list);
	price = calc_price(item);

	if (price > player.gold) {
		printf("You do not have enough gold!\n");
		return;
	}

	printf("You buy %s for %d gold.\n", item->name, price);
	player.gold -= price;
	list_del(&item->list);
	list_add_tail(&item->list, &player.inventory);
}


