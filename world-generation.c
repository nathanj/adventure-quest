#include "game.h"

struct r {
	int sx, w;
	int sy, h;
	int dx, dy;
} rooms[10];
int num_rooms;
int gen_level;

/* Comparison function for sorting by room distance. */
int room_distance(const void *a, const void *b)
{
	const struct r *m = a, *n = b;
	return (m->dx + m->dy) - (n->dx + n->dy);
}

static int can_place_room(int start_x, int start_y, int w, int h)
{
	int i, j;

	if (start_x > 0)
		start_x--, w++;
	if (start_y > 0)
		start_y--, h++;
	if (start_x + w < WIDTH)
		w++;
	if (start_y + h < WIDTH)
		h++;

	for (i = start_y; i < start_y + h; i++) {
		for (j = start_x; j < start_x + w; j++) {
			if (world[gen_level][i][j].open)
				return 0;
		}
	}

	return 1;
}

static void create_room(int rec)
{
	int start_x = rand_between(1, 70);
	int start_y = rand_between(1, 14);
	int w = rand_normal(10, 3);
	int h = rand_normal(10, 3);
	int i, j;

	if (rec >= 50)
		return;

	if (w < 1)
		w = 1;
	if (h < 1)
		h = 1;

	if (start_x + w >= WIDTH)
		w = WIDTH - 1 - start_x;
	if (start_y + h >= HEIGHT)
		h = HEIGHT - 1 - start_y;

	if (!can_place_room(start_x, start_y, w, h)) {
		create_room(rec + 1);
		return;
	}

	rooms[num_rooms].sx = start_x;
	rooms[num_rooms].sy = start_y;
	rooms[num_rooms].w = w;
	rooms[num_rooms].h = h;
	num_rooms++;

	for (i = start_y; i < start_y + h; i++) {
		for (j = start_x; j < start_x + w; j++) {
			world[gen_level][i][j].open = 1;
		}
	}

}

static void print_path(struct node *node)
{
	while (node) {
		world[gen_level][node->y][node->x].open = 1;
		node = node->parent;
	}
}

static void make_paths()
{
	int i;

	for (i = 0; i < num_rooms; i++) {
		int x, y;
		int p = rand_between(0, 4);

		switch (p) {
		case 0:
			x = rooms[i].sx - 1;
			y = rand_between(rooms[i].sy, rooms[i].sy + rooms[i].h);
			break;
		case 1:
			x = rooms[i].sx + rooms[i].w;
			y = rand_between(rooms[i].sy, rooms[i].sy + rooms[i].h);
			break;
		case 2:
			x = rand_between(rooms[i].sx, rooms[i].sx + rooms[i].w);
			y = rooms[i].sy - 1;
			break;
		case 3:
			x = rand_between(rooms[i].sx, rooms[i].sx + rooms[i].w);
			y = rooms[i].sy + rooms[i].h;
			break;
		}

		rooms[i].dx = x;
		rooms[i].dy = y;
	}

	qsort(rooms, num_rooms, sizeof(rooms[0]), room_distance);

	for (i = 0; i < num_rooms - 1; i++) {
		struct node_list open, closed;
		struct node_list start;

		init_node_list(&open);
		init_node_list(&start);
		init_node_list(&closed);

		start.node.x = rooms[i].dx;
		start.node.y = rooms[i].dy;
		start.node.h = heuristic(rooms[i].dx, rooms[i].dy,
					 rooms[i + 1].dx, rooms[i + 1].dy);

		open.next = &start;

		while (open.next) {
			if (open.next->node.x == rooms[i + 1].dx
			    && open.next->node.y == rooms[i + 1].dy) {
				print_path(&open.next->node);
				break;
			}

			search_nodes(&open, &closed, open.next,
				     rooms[i + 1].dx, rooms[i + 1].dy,
				     gen_level, 0);
		}
	}
}

void generate_world()
{
	int i;
	memset(&world, 0, sizeof(world));
	gen_level = 0;
	for (i = 0; i < 10; i++) {
		num_rooms = 0;
		gen_level = i;
		create_room(0);
		create_room(0);
		create_room(0);
		create_room(0);
		create_room(0);
		create_room(0);
		create_room(0);
		create_room(0);
		make_paths();
	}
}
