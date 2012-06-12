#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/sqlite3.h"
#include "../include/libircclient.h"
#include "../include/log.h"

/**
* Log each event
*
* @param irc_session_t session
* @param char event
* @param char origin
* @param char params
* @param int count
*
* @return void
*/
void log_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';
	for (cnt = 0; cnt < count; cnt++) {
		if (cnt) {
			strcat (buf, "|");
		}
		strcat(buf, params[cnt]);
	}

	insert_log(event, origin ? origin : "NULL", cnt, buf);
}

/**
* Start logging
*
* @param sqlite3 db
*
* @return int
*/
int activate_log(sqlite3* db)
{
	remove("log.db");

	if (open_database(db)) {
		if (create_table(db)) {
			enableLog = 1;
			return 1;
		}
	}
}

/**
* Open database connection
*
* @param sqlite3 db
*
* @return in
*/
int open_database(sqlite3* db)
{
	if (sqlite3_open("log.db", &db) != SQLITE_OK) {
		sqlite3_close(db);
		return 0;
	}

	return 1;
}

/**
* Create table for log files
*
* @param sqlite3 db
*
* @return int
*/
int create_table(sqlite3* db)
{
	if (sqlite3_exec(db, "CREATE TABLE Log(Event TEXT, Origin TEXT, Count INT, Buffer TEXT)", NULL, 0, NULL) != SQLITE_OK) {
		printf("Cannot create table\n");
		sqlite3_close(db);
		return 0;
	}

	return 1;
}

/**
* Stop logging
*
* @param sqlite3 db
*
* @return void
*/
void stop_log(sqlite3* db)
{
	sqlite3_close(db);
	enableLog = 0;
}

/**
* Insert into logfile
*
* @param char event
* @param char origin
* @param int count
* @param char buffer
*
* @return void
*/
void insert_log(const char* event, const char* origin, int count, char* buffer)
{
	char* sql_query;
	sprintf(sql_query, "INSERT INTO Log VALUES (%s, %s, %i, %s)", event, origin, count, buffer);
	sqlite3_exec(db, sql_query, NULL, NULL, NULL);
}

/**
* Get complete log
*
* @param sqlite3 db
*
* @return char
*/
char* get_log(sqlite3* db)
{
	char* buffer;
	char* sql_query = "SELECT * FROM Log";
	sqlite3_stmt* vm;
	int i = 0;

	sqlite3_prepare(db, sql_query, strlen(sql_query) + 1, &vm, NULL);
	while(sqlite3_step(vm) != SQLITE_DONE) {
		if (i == 0) {
			sprintf(buffer, "Event \"%s\", Origin: \"%s\", Params: %s [%s]\n", sqlite3_column_text(vm, 0), sqlite3_column_text(vm, 1), sqlite3_column_text(vm, 2), sqlite3_column_text(vm, 3));
		} else {
			sprintf(buffer, "%sEvent \"%s\", Origin: \"%s\", Params: %s [%s]\n", buffer, sqlite3_column_text(vm, 0), sqlite3_column_text(vm, 1), sqlite3_column_text(vm, 2), sqlite3_column_text(vm, 3));
		}

		i++;
	}

	sqlite3_finalize(vm);
	return buffer;
}
