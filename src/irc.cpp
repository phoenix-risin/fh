#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/sqlite3.h"
#include "../include/libircclient.h"
#include "../include/connect.h"
#include "../include/log.h"

/**
* Event for connecting to server
*
* @param irc_session_t session
* @param char event
* @param char origin
* @param char params
* @param in count
*
* @return void
*/
void event_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	irc_ctx_t* ctx = (irc_ctx_t*) irc_get_ctx(session);
	irc_cmd_join(session, ctx->channel, 0);

	if (enableLog) {
		log_event(session, event, origin, params, count);
	}
}

/**
* Event for joining to server
*
* @param irc_session_t session
* @param char event
* @param char origin
* @param char params
* @param in count
*
* @return void
*/
void event_join(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	irc_cmd_user_mode(session, "+i");
	irc_cmd_msg(session, params[0], "FH-Bot bereit.");

	if (enableLog) {
		log_event(session, event, origin, params, count);
	}
}

/**
* Event for channel operations
*
* @param irc_session_t session
* @param char event
* @param char origin
* @param char params
* @param in count
*
* @return void
*/
void event_channel(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	irc_ctx_t* ctx = (irc_ctx_t*) irc_get_ctx(session);

	if (!strcmp(params[1], ctx->nick))
		irc_cmd_msg(session, params[0], "Ja bitte?");

	if (!strcmp (params[1], "!quit")) {
		stop_log(db);
		irc_cmd_quit (session, "Ich beende.");
	}

	if (!strcmp (params[1], "!log")) {
		if (enableLog) {
			stop_log(db);
			irc_cmd_msg(session, params[0], "Logging ausgeschaltet.");
		} else {
			activate_log(db);
			irc_cmd_msg(session, params[0], "Logging eingeschaltet.");
		}
	}

	if (!strcmp (params[1], "!post_log")) {
		if (enableLog) {
			irc_cmd_msg(session, params[0], "Poste log:");
			irc_cmd_msg(session, params[0], get_log(db));
		} else {
			irc_cmd_msg(session, params[0], "Logging ausgeschaltet, kann nicht posten.");
		}
	}

	if (strstr (params[1], "!nick ") == params[1]) {
		irc_cmd_nick (session, params[1] + 6);
	}

	if (enableLog) {
		log_event(session, event, origin, params, count);
	}
}

/**
* Join to Irc server with nick and join channel
*
* @param irc_callbacks_t callbacks
* @param irc_ctx_t ctx
* @param irc_session_t session
* @param char server
* @param char nick
* @param char channel
*
* @return int
*/
int connect(irc_callbacks_t* callbacks, irc_ctx_t* ctx, irc_session_t* session, char* server, char* nick, char* channel)
{
	memset(callbacks, 0, sizeof(*callbacks));

	callbacks->event_connect = event_connect;
	callbacks->event_join = event_join;
	callbacks->event_channel = event_channel;

	callbacks->event_nick = log_event;
	callbacks->event_quit = log_event;
	callbacks->event_part = log_event;
	callbacks->event_mode = log_event;
	callbacks->event_topic = log_event;
	callbacks->event_kick = log_event;
	callbacks->event_privmsg = log_event;
	callbacks->event_notice = log_event;
	callbacks->event_invite = log_event;
	callbacks->event_umode = log_event;
	callbacks->event_ctcp_rep = log_event;
	callbacks->event_ctcp_action = log_event;
	callbacks->event_unknown = log_event;

	session = irc_create_session(callbacks);

	if (!session) {
		printf ("Could not create session\n");
		return 1;
	}

	unsigned short port;

	ctx->nick = nick;
	ctx->channel = channel;

	irc_set_ctx(session, ctx);

	if (strchr( server, ':' ) != 0) {
		port = 0;
	}

	if (server[0] == '#' && server[1] == '#') {
		server++;	
		irc_option_set(session, LIBIRC_OPTION_SSL_NO_VERIFY);
	}

	if (irc_connect (session, server, port, 0, nick, 0, 0)) {
		printf("Could not connect: %s\n", irc_strerror(irc_errno(session)));
		return 1;
	}

	if (irc_run(session)) {
		printf("Could not connect or I/O error: %s\n", irc_strerror(irc_errno(session)));
		return 1;
	}

	return 0;
}
