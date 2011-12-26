
//#define NORMAL    "\x1b[0m"
//#define BLACK     "\x1b[30m"
//#define RED       "\x1b[31m"
//#define GREEN     "\x1b[32m"
//#define YELLOW    "\x1b[33m"
//#define BLUE      "\x1b[34m"
//#define MAGENTA   "\x1b[35m"
//#define CYAN      "\x1b[36m"
//#define WHITE     "\x1b[37m"
//#define B_BLACK   "\x1b[30;1m"
//#define B_RED     "\x1b[31;1m"
//#define B_GREEN   "\x1b[32;1m"
//#define B_YELLOW  "\x1b[33;1m"
//#define B_BLUE    "\x1b[34;1m"
//#define B_MAGENTA "\x1b[35;1m"
//#define B_CYAN    "\x1b[36;1m"
//#define B_WHITE   "\x1b[37;1m"

#define NORMAL   COLOR_PAIR(0)
#define RED      COLOR_PAIR(1)
#define GREEN    COLOR_PAIR(2)
#define YELLOW   COLOR_PAIR(3)
#define BLUE     COLOR_PAIR(4)
#define CYAN     COLOR_PAIR(5)
#define MAGENTA  COLOR_PAIR(6)
#define WHITE    COLOR_PAIR(7)

#define BOLD       (COLOR_PAIR(0) | A_BOLD)
#define B_RED      (COLOR_PAIR(1) | A_BOLD)
#define B_GREEN    (COLOR_PAIR(2) | A_BOLD)
#define B_YELLOW   (COLOR_PAIR(3) | A_BOLD)
#define B_BLUE     (COLOR_PAIR(4) | A_BOLD)
#define B_CYAN     (COLOR_PAIR(5) | A_BOLD)
#define B_MAGENTA  (COLOR_PAIR(6) | A_BOLD)
#define B_WHITE    (COLOR_PAIR(7) | A_BOLD)

#define amvprintw(attr, x, y, fmt, ...) do {\
	attrset(attr); \
	mvprintw(x, y, fmt, ##__VA_ARGS__); \
} while (0)

#define aprintw(attr, fmt, ...) do {\
	attrset(attr); \
	printw(fmt, ##__VA_ARGS__); \
} while (0)

