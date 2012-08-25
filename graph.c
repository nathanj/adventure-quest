/*
 * This file contains an A* pathfinding implementation.
 */

#include "game.h"

/* Calculates the Manhattan distance for a node to the destination. */
int heuristic(int x, int y, int tx, int ty)
{
	return abs(ty - y) + abs(tx - x);
}

void init_node_list(struct node_list *n)
{
	memset(n, 0, sizeof(*n));
}

const char *str_node(struct node *node)
{
	static char buf[64];
	struct node *parent = node->parent;

	if (parent)
		sprintf(buf, "(%d, %d, %d) -> (%d, %d)",
			node->x, node->y, node->h,
			parent->x, parent->y);
	else
		sprintf(buf, "(%d, %d, %d) -> nil",
			node->x, node->y, node->h);

	return buf;
}

void add_to_list(struct node_list *list, struct node_list *new)
{
	struct node_list *prev;

	if (!list->next) {
		list->next = new;
		return;
	}

	prev = list;
	list = list->next;

	while (list) {
		if (new->node.h < list->node.h) {
			prev->next = new;
			new->next = list;
			return;
		}
		prev = list;
		list = list->next;
	}

	prev->next = new;
}

void remove_from_list(struct node_list *list, struct node_list *needle)
{
	while (list && list->next != needle)
		list = list->next;

	/* Element not found. */
	if (!list)
		return;

	list->next = needle->next;
	needle->next = NULL;
}

struct node_list *get_node(struct node_list *list, int i, int j)
{
	while (list) {
		if (list->node.x == i && list->node.y == j)
			return list;
		list = list->next;
	}

	return NULL;
}

void add_node_if_walkable(struct node_list *open, struct node_list *closed,
			  struct node_list *entry, int i, int j,
			  int tx, int ty, int level, int want_open)
{
	struct node_list *new;
	struct node_list *old;
	int h = heuristic(i, j, tx, ty);

	if (i < 0 || i >= WIDTH)
		return;
	if (j < 0 || j >= HEIGHT)
		return;
	if (want_open && !world[level][j][i].open)
		return;
	if (!want_open && world[level][j][i].open)
		return;

	old = get_node(closed, i, j);
	if (old)
		return;

	old = get_node(open, i, j);
	if (old) {
		if (h < old->node.h)
			remove_from_list(open, old);
		else
			return;
	}

	new = malloc(sizeof(*new));
	init_node_list(new);
	new->node.x = i;
	new->node.y = j;
	new->node.h = heuristic(i, j, tx, ty);
	new->node.parent = &entry->node;
	add_to_list(open, new);
}

void search_nodes(struct node_list *open, struct node_list *closed,
		  struct node_list *entry, int tx, int ty,
		  int level, int want_open)
{
	int x = entry->node.x;
	int y = entry->node.y;

	remove_from_list(open, entry);
	add_to_list(closed, entry);

	add_node_if_walkable(open, closed, entry, x - 1, y, tx, ty, level, want_open);
	add_node_if_walkable(open, closed, entry, x + 1, y, tx, ty, level, want_open);
	add_node_if_walkable(open, closed, entry, x, y - 1, tx, ty, level, want_open);
	add_node_if_walkable(open, closed, entry, x, y + 1, tx, ty, level, want_open);
}

