#include "game.h"

struct room world[10][HEIGHT][WIDTH];

void print_room(int i, int j)
{
	int l = player.world_level;
	int x = player.world_x;
	int y = player.world_y;

	struct room *room = &world[l][i][j];

	if (!room->lit) {
		printw(" ");
		return;
	}

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
		aprintw(NORMAL, ">");
	} else if (room->stairs_up) {
		aprintw(NORMAL, "<");
	} else if (room->open) {
		aprintw(NORMAL, ".");
	} else {
		aprintw(NORMAL, "#");
	}
}

void print_map()
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		move(i, 0);
		for (j = 0; j < WIDTH; j++)
			print_room(i, j);
	}
}

static int is_path_between_stairs(int level, int ux, int uy, int dx, int dy)
{
	struct node_list open, closed;
	struct node_list start;

	init_node_list(&open);
	init_node_list(&start);
	init_node_list(&closed);

	start.node.x = ux;
	start.node.y = uy;
	start.node.h = heuristic(ux, uy, dx, dy);

	open.next = &start;

	while (open.next) {
		if (open.next->node.x == dx && open.next->node.y == dy)
			return 1;

		search_nodes(&open, &closed, open.next, dx, dy, level, 1);
	}

	return 0;
}

void create_stairs()
{
	int l, i, j;

	for (l = 0; l < 10; l++) {
		int ux, uy;
		int dx, dy;
retry:
		do {
			i = rand() % HEIGHT;
			j = rand() % WIDTH;
		} while (!world[l][i][j].open);

		dx = j;
		dy = i;

		do {
			i = rand() % HEIGHT;
			j = rand() % WIDTH;
		} while (!(world[l][i][j].open
			   && !world[l][i][j].stairs_down));

		ux = j;
		uy = i;

		if (!is_path_between_stairs(l, ux, uy, dx, dy))
			goto retry;

		if (l < 9)
			world[l][dy][dx].stairs_down = 1;
		if (l == 0) {
			player.world_x = uy;
			player.world_y = ux;
		} else {
			world[l][uy][ux].stairs_up = 1;
		}
	}
}

void create_monsters()
{
	int num = 10;
	int n, i, j, l;
	struct creature *creature;

	for (l = 0; l < 10; l++) {
		for (n = 0; n < num; n++) {
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

			do {
				i = rand() % HEIGHT;
				j = rand() % WIDTH;
			} while (!world[l][i][j].open);

			list_add_tail(&creature->list,
				      &world[l][i][j].creatures);
		}
	}

	/* Add the dragon on the last floor. */
	do {
		i = rand() % HEIGHT;
		j = rand() % WIDTH;
	} while (!world[9][i][j].open);
	creature = create_dragon();
	list_add_tail(&creature->list, &world[9][i][j].creatures);
}

void create_store()
{
	struct store *store;
	int i, j;
	struct room *room;
	int rc;

	store = calloc(1, sizeof(*store));

	rc = asprintf(&store->name, "Merlin's Magic Shop");
	assert(rc != -1);

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

	do {
		i = rand() % HEIGHT;
		j = rand() % WIDTH;
		room = &world[0][i][j];
	} while (!(room->open && !room->stairs_up && !room->stairs_down
		   && list_empty(&room->creatures)));

	room->store = store;
}

void init_world()
{
	int l, i, j;

	memset(world, 0, sizeof(world));
	generate_world();

	for (l = 0; l < 10; l++) {
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
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
	int x = 0, y = 81;

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

#define RANGE 10

struct vis {
	int can_see : 1;
};

static void handle_light_direction(int px, int py, struct vis (*vis)[WIDTH],
				   int num, float dx, float dy)
{
	int i;
	float x, y;

	for (i = 0, x = px, y = py; i < num; i++, x += dx, y += dy) {
		int a = (int) x;
		int b = (int) y;
		if (a < 0 || a >= HEIGHT)
			break;
		if (b < 0 || b >= WIDTH)
			break;
		vis[a][b].can_see = 1;
		if (!world[player.world_level][a][b].open)
			break;
	}
}

void handle_lighting()
{
	int pl = player.world_level;
	int px = player.world_x;
	int py = player.world_y;
	int i, j;

	struct vis vis[HEIGHT][WIDTH];

	memset(&vis, 0, sizeof(vis));

	for (i = 0; i < RANGE * 10 + 1; i++) {
		float angle = i * M_PI_2 / RANGE / 10;
		float dx = cosf(angle);
		float dy = sinf(angle);

		handle_light_direction(px, py, vis, RANGE, +dx, +dy);
		handle_light_direction(px, py, vis, RANGE, +dx, -dy);
		handle_light_direction(px, py, vis, RANGE, -dx, +dy);
		handle_light_direction(px, py, vis, RANGE, -dx, -dy);
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (vis[i][j].can_see)
				world[pl][i][j].lit = 1;
		}
	}
}
