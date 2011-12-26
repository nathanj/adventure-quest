#include "game.h"

enum state {
	WORLD,
	INVENTORY,
	DROP,
	STORE
};

enum state state;

void print_prompt()
{
	move(12, 0);

	if (player.self.health < player.self.max_health * 0.20)
		attrset(B_RED);
	else
		attrset(GREEN);
	printw("%d/%d ",
	       player.self.health, player.self.max_health);
	aprintw(NORMAL, "Health, ");
	aprintw(BLUE, "%d/%d ", player.self.mana, player.self.max_mana);
	aprintw(NORMAL, "Mana, Level ");
	aprintw(CYAN, "%d", player.self.level);
	aprintw(NORMAL, ", ");
	aprintw(CYAN, "%d ", player.self.experience);
	aprintw(NORMAL, "Experience, ");
	aprintw(YELLOW, "%d ", player.gold);
	aprintw(NORMAL, "Gold");
}

void finish(int sig)
{
	(void) sig;

	endwin();

	exit(0);
}

void init_screen()
{
	initscr();
	keypad(stdscr, TRUE);

	/* Input one character at a time. */
	cbreak();
	noecho();

	use_default_colors();

	if (has_colors()) {
		start_color();

		init_pair(1, COLOR_RED,     -1);
		init_pair(2, COLOR_GREEN,   -1);
		init_pair(3, COLOR_YELLOW,  -1);
		init_pair(4, COLOR_BLUE,    -1);
		init_pair(5, COLOR_CYAN,    -1);
		init_pair(6, COLOR_MAGENTA, -1);
		init_pair(7, COLOR_WHITE,   -1);
	}
}

void draw()
{
	struct room *room = current_room();

	switch (state) {
	case WORLD:
		clear();
		print_map();
		print_prompt();
		print_current_room_contents();
		print_messages();
		break;
	case INVENTORY:
	case DROP:
		clear();
		print_inventory();
		break;
	case STORE:
		clear();
		print_store_inventory(room->store);
		break;
	}
}

void handle_attack()
{
	struct room *room = current_room();
	struct creature *creature;
	int first = 1;

	if (list_empty(&room->creatures)) {
		message(NORMAL, "There is no one to attack");
		return;
	}

	list_for_each_entry(creature, &room->creatures, list) {
		if (first) {
			player.self.attack(&player.self, creature);
			first = 0;
		}

		if (creature->health > 0)
			creature->attack(creature, &player.self);
	}
}

void handle_take()
{
	struct item *item, *n;
	struct room *room = current_room();

	if (room->gold) {
		message(NORMAL, "You take %d gold coins.", room->gold);
		player.gold += room->gold;
		room->gold = 0;
	}

	if (!list_empty(&room->items)) {

		list_for_each_entry_safe(item, n, &room->items, list) {
			if (player.num_items >= 20) {
				message(NORMAL, "Your inventory is full!");
				break;
			}
			message(NORMAL, "You take %s.", item->name);
			list_del(&item->list);
			list_add_tail(&item->list, &player.inventory);
			player.num_items++;
		}
	}
}

void handle_world_input(int c)
{
	struct room *room = current_room();

	switch (c) {
	case 'a':
		handle_attack();
		break;
	case 'k':
		player.go(-1, 0, 0);
		break;
	case 'j':
		player.go(1, 0, 0);
		break;
	case 'h':
		player.go(0, -1, 0);
		break;
	case 'l':
		player.go(0, 1, 0);
		break;
	case '<':
		player.go(0, 0, -1);
		break;
	case '>':
		player.go(0, 0, 1);
		break;
	case ',':
		handle_take();
		break;
	case 'i':
		state = INVENTORY;
		break;
	case 'd':
		state = DROP;
		break;
	case 'b':
		if (room->store)
			state = STORE;
		break;
	case 'Z':
		finish(0);
		break;
	default:
		message(NORMAL, "Unknown command: %02x", c);
		break;
	}
}

void handle_inventory_input(int c)
{
	switch (c) {
	case 'z':
		state = WORLD;
		break;
	default:
		if (use_item(c - 'a'))
			state = WORLD;
		break;
	}
}

void handle_drop_input(int c)
{
	switch (c) {
	case 'z':
		state = WORLD;
		break;
	default:
		if (drop_item(c - 'a'))
			state = WORLD;
		break;
	}
}

void handle_store_input(int c)
{
	struct room *room = current_room();

	switch (c) {
	case 'z':
		state = WORLD;
		break;
	default:
		if (buy_item(room->store, c - 'a'))
			state = WORLD;
		break;
	}
}

void handle_input(int c)
{
	switch (state) {
	case WORLD:
		handle_world_input(c);
		break;
	case INVENTORY:
		handle_inventory_input(c);
		break;
	case DROP:
		handle_drop_input(c);
		break;
	case STORE:
		handle_store_input(c);
		break;
	}
}

int main()
{
	int c;

	state = WORLD;

	init_world();
	init_player();
	init_screen();

	signal(SIGINT, finish);

	while (1) {
		draw();

		c = getch();

		handle_input(c);

		room_remove_dead_creatures();
	}

	finish(0);

	return 0;
}

