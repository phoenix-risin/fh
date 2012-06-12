#ifndef CONNECT_H
#define CONNECT_H

typedef struct
{
	char* channel;
	char* nick;
} irc_ctx_t;

int connect(irc_callbacks_t* callbacks, irc_ctx_t* ctx, irc_session_t* session, char* server, char* nick, char* channel);

void event_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

void event_join(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

void event_channel(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

#endif
