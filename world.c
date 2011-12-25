#include "game.h"

struct room world[10][10][10];

void print_room(int i, int j)
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;

	struct room *room = &world[l][i][j];

	if (x == i && y == j)
		printf(B_RED "X");
	else if (!list_empty(&room->creatures)) {
		struct creature *creature = list_entry(room->creatures.next,
						       struct creature, list);
		printf("%s%c", creature->color, creature->symbol);
	} else if (!list_empty(&room->items)) {
		struct item *item = list_entry(room->items.next,
					       struct item, list);
		printf("%s%c", item->color, item->symbol);
	} else if (room->gold)
		printf(B_YELLOW "$");
	else if (room->store) {
		printf("%s%c", room->store->color, room->store->symbol);
	}
	else if (room->stairs_down)
		printf(">");
	else if (room->stairs_up)
		printf("<");
	else
		printf(" ");

	printf(NORMAL);
}

void print_map()
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("[");
			print_room(i, j);
			printf("]");
		}
		printf("\n");
	}
}

void create_stairs()
{
	int l, i, j;

	for (l = 0; l < 9; l++) {
retry:
		i = rand() % 10;
		j = rand() % 10;

		/* Cannot have both sets of stairs on same room. */
		if (world[l][i][j].stairs_up)
			goto retry;

		world[l][i][j].stairs_down = 1;
		world[l+1][i][j].stairs_up = 1;
	}
}

void create_monsters()
{
	int num = 10;
	int n, i, j;
	struct creature *creature;

	for (n = 0; n < num; n++) {
		i = rand() % 10;
		j = rand() % 10;

		switch(rand() % 2) {
		case 0:
			creature = create_bat();
			break;
		case 1:
			creature = create_slime();
			break;
		default:
			assert(0);
			break;
		}

		list_add_tail(&creature->list, &world[0][i][j].creatures);
	}
}

void create_store()
{
	struct store *store;

	store = calloc(1, sizeof(*store));

	asprintf(&store->name, "Merlin's Shop");
	store->symbol = '@';
	store->color = B_MAGENTA;
	INIT_LIST_HEAD(&store->inventory);

	list_add_tail(&(create_random_potion()->list), &store->inventory);
	list_add_tail(&(create_random_potion()->list), &store->inventory);
	list_add_tail(&(create_random_potion()->list), &store->inventory);
	list_add_tail(&(create_random_potion()->list), &store->inventory);
	list_add_tail(&(create_random_potion()->list), &store->inventory);

	list_add_tail(&(create_random_armor()->list), &store->inventory);
	list_add_tail(&(create_random_armor()->list), &store->inventory);
	list_add_tail(&(create_random_armor()->list), &store->inventory);
	list_add_tail(&(create_random_armor()->list), &store->inventory);
	list_add_tail(&(create_random_armor()->list), &store->inventory);

	world[0][5][5].store = store;
}

void init_world()
{
	int l, i, j;

	memset(world, 0, sizeof(world));

	for (l = 0; l < 10; l++) {
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {
				INIT_LIST_HEAD(&world[l][i][j].creatures);
				INIT_LIST_HEAD(&world[l][i][j].items);
			}
		}
	}

	create_stairs();
	create_monsters();
	create_store();
}

struct room *current_room()
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;

	return &world[l][x][y];
}

void room_remove_dead_creatures()
{
	struct room *room = current_room();
	struct list_head *pos, *n;
	struct creature *creature;

	list_for_each_safe(pos, n, &room->creatures) {
		creature = list_entry(pos, struct creature, list);
		if (creature->health <= 0) {
			list_del(pos);
			free_creature(creature);
		}
	}
}

void print_current_room_contents()
{
	struct room *room = current_room();

	if (!list_empty(&room->creatures)) {
		struct creature *creature;

		list_for_each_entry(creature, &room->creatures, list) {
			printf("%s\n", creature->name);
		}
	}

	if (room->gold)
		printf(YELLOW "%d gold coins.\n" NORMAL, room->gold);

	if (!list_empty(&room->items)) {
		struct item *item;

		list_for_each_entry(item, &room->items, list) {
			printf("%s\n", item->name);
		}
	}

	if (room->stairs_down)
		printf("Stairs leading down.\n");

	if (room->stairs_up)
		printf("Stairs leading up.\n");

	if (room->store)
		print_store_inventory(room->store);
}

