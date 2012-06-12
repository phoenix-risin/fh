#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/sqlite3.h"
#include "../include/libircclient.h"
#include "../include/irc.h"
#include "../include/log.h"

/**
* Main application. Use config file if shell is not used. 
* Creates irc connection.
*/
int main(int argc, char** argv)
{
	irc_callbacks_t	callbacks;
	irc_ctx_t ctx;
	irc_session_t* s;
	char* server = "localhost";
	char* channel = "test";
	char* nick = "Bot";
	enableLog = 0;

	if (argc <= 1) {
		FILE* configFile = fopen("config", "r");
		if (configFile == NULL) {
			configFile = fopen("config", "w");
			if (configFile == NULL) {
				printf("Error reading config file\n");
				return 1;
			}
			fprintf(configFile, "%s %s %s", server, channel, nick);
			fclose(configFile);
		} else {
			fscanf(configFile, "%s", server);
			fscanf(configFile, "%s", channel);
			fscanf(configFile, "%s", nick);
		}
	} else if (argc >= 1) {
		server = argv[1];
	}
	if (argc >= 2) {
		nick = argv[2];
	}
	if (argc >= 3) {
		channel = argv[3];
	}

	if (connect(&callbacks, &ctx, s, server, nick, channel) != 0) {
		return 1;
	}

	return 0;
}