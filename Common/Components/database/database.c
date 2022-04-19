#include "ehs_fb_types.h"
#include "url_get.h"
#include "setCompletes.h"
#include "target.h"

#include "app_data.h"
//#include "callback_queue.h"
#include "hal-api.h"


/***
 * \todo This file is not implemented! It just seems to be a hack of the URL getter, probably as a template.
 */


EHS_FB_FUNCTIONS_START(UrlGet)
EHS_FB_FUNCTION_ENTRY("getpost", UrlGet_get)
EHS_FB_FUNCTION_ENTRY("abort", UrlGet_abort)
EHS_FB_FUNCTIONS_END

/* port identifiers for getpost */
#define EHS_FB_URLGET_GETTING 		(9)
#define EHS_FB_URLGET_URL 			(1)
#define EHS_FB_URLGET_POST_DATA 	(2)
#define EHS_FB_URLGET_LOCAL_PATH	(3)
#define EHS_FB_URLGET_URLDIRDUMP	(4)
#define EHS_FB_URLGET_PRIORITY		(5)
#define EHS_FB_URLGET_RESPONSE		(6)
#define EHS_FB_URLGET_DATAREADY		(10)
#define EHS_FB_URLGET_ERROR			(11)
#define EHS_FB_URLGET_ENUM			(7)
#define EHS_FB_URLGET_ESTRING		(8)

/* port identifiers for abort function */
#define EHS_FB_URLGET_ABORTED 1

#define EHS_STRING_LENGTH_MAX_LARGE (EHS_STRING_LENGTH_MAX*4) //@todo formalise this
struct EhsFbDatabaseStruct { //Note make all ints and bools ehs_uint32 to avoid scanf ("%d") problem overwriting the next entry...!
	ehs_char database[EHS_STRING_LENGTH_MAX];
	ehs_char Current Query[EHS_STRING_LENGTH_MAX];
	int databasehandle;
/*	EhsCallbackQueueEntryType xCallbackOut;		/* [ internal system variable!! */
};

#include <stdio.h>
#include <sqlite3.h>

static int EhsT_DatabaseCallback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); // Need to write this
  }
  printf("\n");
  return 0;
}

/* The T functions here maybe moved to the target directory if they become "uncommon" */
int EhsT_DatabaseOpenDatabase(sqlite3 *db, ehs_char* database) {
	if (sqlite3_open(database, db)) {
		printf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		db = NULL;
		return EHS_FALSE;
	}
	else {
		return EHS_TRUE;
	}

}

int EhsT_DatabaseCloseDatabase(sqlite3 *db) {
	if (db) sqlite3_close(db);
}

int EhsT_DatabaseSetQuery(sqlite3 *db;, ehs_char* query,void * answer){

  int rc;

  rc = sqlite3_exec(db, argv[2], EhsT_DatabaseCallback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  return 0;
}


/**
 * Define the identify function.
 */
EHS_FB_IDENTIFY_FUNCTION(Database) {
	EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsFbUrlGetStruct);
}

/**
 * Initialise the curl getter.
 *
 */
EHS_FB_INIT_FUNCTION(Database) {

	return EHS_TRUE; /* initialisation always succeeds */
}





// pass in a copy of the args so that we are rentrant without a critical section.
EHS_FB_THREAD_FUNCTION( Database ) {


}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(Database_open) {

}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(Database_query) {

}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(Database_close) {

}

