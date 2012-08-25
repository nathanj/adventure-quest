#include "game.h"

enum status {
	EMPTY,
	OLD,
	NEW
};

struct message {
	enum status status;
	int color;
	char buffer[128];
};

#define NUM_MESSAGES 8

struct message messages[NUM_MESSAGES];
static int last_msg_num;

void init_messages()
{
	memset(messages, 0, sizeof(messages));
	last_msg_num = 0;
}

void message(int color, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	vsnprintf(messages[last_msg_num].buffer,
		  sizeof(messages[last_msg_num].buffer), fmt, ap);
	messages[last_msg_num].status = NEW;
	messages[last_msg_num].color = color;

	last_msg_num++;
	if (last_msg_num > NUM_MESSAGES - 1)
		last_msg_num = 0;

	va_end(ap);
}

void print_messages()
{
	int i, j;

	move(25, 0);
	for (i = 0; i < NUM_MESSAGES; i++) {
		j = (last_msg_num + i) % NUM_MESSAGES;

		if (messages[j].status != EMPTY) {
			if (messages[j].status == NEW)
				attrset(messages[j].color | BOLD);
			else
				attrset(messages[j].color);

			messages[j].status = OLD;

			printw("%s\n", messages[j].buffer);
		}
	}
}

