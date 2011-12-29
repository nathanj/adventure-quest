#include "game.h"

struct room world[10][10][10];

void print_room(int i, int j)
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;

	struct room *room = &world[l][i][j];

	if (x == i && y == j) {
		aprintw(B_RED, "X");
	} else if (!list_empty(&room->creatures)) {
		struct creature *creature = list_entry(room->creatures.next,
						       struct creature, list);
		aprintw(creature->color, "%c", creature->symbol);
	} else if (!list_empty(&room->items)) {
		struct item *item = list_entry(room->items.next,
					       struct item, list);
		aprintw(item->color, "%c", item->symbol);
	} else if (room->gold) {
		aprintw(B_YELLOW, "$");
	} else if (room->store) {
		aprintw(room->store->color, "%c", room->store->symbol);
	} else if (room->stairs_down) {
		printw(">");
	} else if (room->stairs_up) {
		printw("<");
	} else {
		printw(" ");
	}
}

void print_map()
{
	int i, j;

	move(0, 0);

	for (i = 0; i < 10; i++) {
		move(i, 0);
		for (j = 0; j < 10; j++) {
			aprintw(NORMAL, "[");
			print_room(i, j);
			aprintw(NORMAL, "]");
		}
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
	int n, i, j, l;
	struct creature *creature;

	for (l = 0; l < num; l++) {
		for (n = 0; n < num; n++) {
			i = rand() % 10;
			j = rand() % 10;

			switch (rand() % 4) {
			case 0:
				creature = create_bat();
				break;
			case 1:
				creature = create_slime();
				break;
			case 2:
				if (l > 0)
					creature = create_skeleton();
				else
					creature = create_bat();
				break;
			case 3:
				if (l > 1)
					creature = create_bear();
				else
					creature = create_slime();
				break;
			default:
				assert(0);
				break;
			}

			list_add_tail(&creature->list,
				      &world[l][i][j].creatures);
		}
	}

	/* Add the dragon on the last floor. */
	i = rand() % 10;
	j = rand() % 10;
	creature = create_dragon();
	list_add_tail(&creature->list, &world[9][i][j].creatures);
}

void create_store()
{
	struct store *store;
	int i, j;
	struct room *room;

	store = calloc(1, sizeof(*store));

	asprintf(&store->name, "Merlin's Magic Shop");
	store->symbol = '@';
	store->color = B_MAGENTA;
	INIT_LIST_HEAD(&store->inventory);

	list_add_tail(&(create_random_potion(1)->list), &store->inventory);
	list_add_tail(&(create_random_potion(1)->list), &store->inventory);
	list_add_tail(&(create_random_potion(1)->list), &store->inventory);
	list_add_tail(&(create_random_potion(1)->list), &store->inventory);
	list_add_tail(&(create_random_potion(1)->list), &store->inventory);

	list_add_tail(&(create_random_armor(1)->list), &store->inventory);
	list_add_tail(&(create_random_armor(1)->list), &store->inventory);
	list_add_tail(&(create_random_armor(1)->list), &store->inventory);
	list_add_tail(&(create_random_armor(1)->list), &store->inventory);
	list_add_tail(&(create_random_armor(1)->list), &store->inventory);

	list_add_tail(&(create_random_weapon(1)->list), &store->inventory);

retry:
	i = rand() % 10;
	j = rand() % 10;
	room = &world[0][i][j];
	if (room->stairs_up || room->stairs_down
	    || !list_empty(&room->creatures))
		goto retry;

	room->store = store;
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
	int x = 0, y = 32;

	amvprintw(NORMAL, x++, y, "This room contains:\n");

	if (!list_empty(&room->creatures)) {
		struct creature *creature;

		list_for_each_entry(creature, &room->creatures, list) {
			amvprintw(NORMAL, x++, y, "%s", creature->name);
		}
	}

	if (room->gold)
		amvprintw(YELLOW, x++, y, "%d gold coins.", room->gold);

	if (!list_empty(&room->items)) {
		struct item *item;

		list_for_each_entry(item, &room->items, list) {
			move(x++, y);
			attrset(NORMAL);
			item->print(item);
		}
	}

	if (room->stairs_down)
		amvprintw(NORMAL, x++, y, "Stairs leading down.");

	if (room->stairs_up)
		amvprintw(NORMAL, x++, y, "Stairs leading up.");

	if (room->store)
		amvprintw(B_MAGENTA, x++, y, "%s", room->store->name);
}

