#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#define MSG_LEN 36

// ============================================================================================= //

#ifndef MESSAGE_BOX
#define MESSAGE_BOX

// This is the message window. It's declared globally because almost every window references it.
WINDOW *msg_win;

extern void message_log(char *message) {
	// logs a message to the message window
	mvwaddstr(msg_win, 0, MSG_LEN/2 - strlen(message)/2, message);
}

#endif
