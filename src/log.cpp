#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/sqlite3.h"
#include "../include/libircclient.h"
#include "../include/log.h"

//! IRC callback.
/*!
  IRC callback zum loggen eines events.
 */
void log_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';

	for (cnt = 0; cnt < count; cnt++)
	{
		if (cnt)
			strcat (buf, "|");

		strcat(buf, params[cnt]);
	}

	log(event, origin ? origin : "NULL", cnt, buf);
}

//! Aktiviert logging.
/*!
  Aktiviert logging. Gibt bei Erfolg 0, bei Misserfolg 1 zurück.
 */
int start_log(sqlite3* db)
{
	remove("log.db");

	if (sqlite3_open("log.db", &db))
	{
		printf("Error opening log database\n");
		sqlite3_close(db);
		loggingEnabled = 0;
		return 1;
	}
	if (sqlite3_exec(db, "CREATE TABLE Log(Event TEXT, Origin TEXT, Count TEXT, Buffer TEXT)", NULL, 0, NULL) != SQLITE_OK)
	{
		printf("Error opening log database\n");
		sqlite3_close(db);
		loggingEnabled = 0;
		return 1;
	}

	loggingEnabled = 1;
	return 0;
}

//! Stoppt logging.
/*!
  Stoppt logging.
 */
void stop_log(sqlite3* db)
{
	sqlite3_close(db);
	loggingEnabled = 0;
}

//! Loggt einen Eintrag.
/*!
  Loggt einen Eintrag.
 */
void log(const char* event, const char* origin, int count, char* buffer)
{
	char* errMsg = 0;
	char* buf;
	sprintf("INSERT INTO Log VALUES (%s, %s, %i, %s)", event, origin, count, buffer);
	sqlite3_exec(db, buf, NULL, 0, NULL);
}

//! Gibt aktuelles log zurück.
/*!
  Gibt aktuelles log zurück oder 0 bei Fehlschlag.
 */
char* get_log(sqlite3* db)
{
	char* sql = "SELECT * FROM Log";
	sqlite3_stmt* stmt;
	char* buffer;
	int i = 0;

	sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	while (1)
	{
		int s;
		s = sqlite3_step(stmt);
		if (s == SQLITE_ROW)
		{
			if (i == 0)
				sprintf(buffer, "Event \"%s\", origin: \"%s\", params: %s [%s]\n", sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3));
			else
				sprintf(buffer, "%sEvent \"%s\", origin: \"%s\", params: %s [%s]\n", buffer, sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3));
		}
		else if (s == SQLITE_DONE)
			break;
		else
			return 0;
	}

	return buffer;
}
