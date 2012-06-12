#ifndef LOG_H
#define LOG_H

int enableLog;
sqlite3* db;

void log_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

void insert_log(const char* event, const char* origin, int count, char* buffer);

int activate_log(sqlite3* db);

int create_table(sqlite3* db);

int open_database(sqlite3* db);

void stop_log(sqlite3* db);

char* get_log(sqlite3* db);

#endif
