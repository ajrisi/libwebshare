#include <sys/types.h>

#include <ctype.h>

#ifdef HAVE_MEMORY_H
  #include <memory.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wsdate.h"


char *wsdate_month[] = {"jan" , "feb" , "mar" , "apr",
			"may" , "jun" , "jul" , "aug",
			"sep" , "oct" , "nov" , "dec"};

char *wsdate_wkday[] = {"sun", "mon" , "tue" ,
			"wed", "thu" , "fri" ,
			"sat"};


/** 
 * Converts all upper case characters to lower case characters in a string
 * 
 * @param str string to convert, must be null terminated
 */
static void downcase( char* str ) {
  for ( ; *str != '\0'; ++str ) {
    if ( isupper( (int) *str ) )
      *str = tolower( (int) *str );
  }
}

/** 
 * Scans a string and determines the weekday contained within. ONLY
 * pass lower case strings. This used to be implemented with a sorted
 * list and a search
 * 
 * @param str_wday string to identify the weekday in 
 * 
 * @return an int 0-6 for the weekday (sunday is 0), or -1 for an error
 */
static int scan_wday( char* str_wday ) {
  int index = 0;
  for(index=0; index < 12; ++index)
    if(strstr(str_wday, wsdate_wkday[index]) != NULL) break;
  if(index==7) index = -1;
  return index;
}


/** 
 * Scans a string and determines the month that string is referring
 * to. ONLY pass lower case strings. This used to be implemented with
 * a sorted list and a search
 * 
 * @param str_mon the sting to identify the month in
 * 
 * @return an int 0-11 for the month, or -1 for an error determining it
 */
static int scan_mon( char* str_mon ) {
  int index = 0;
  //just use strstr for this, just searches for the abbrev. of the month
  for(index=0; index < 12; ++index)
    if(strstr(str_mon, wsdate_month[index]) != NULL) break;
  if(index==13) index = -1;
  return index;
}

/** 
 * Detects if a year is a leap year
 * 
 * @param year 
 * 
 * @return 1 if leap year, 0 if not
 */
static int is_leap_year( int year ) {
  return year % 400? ( year % 100 ? ( year % 4 ? 0 : 1 ) : 0 ) : 1;
}

/** 
 * Parses a character string and generates a time_t form.
 * 
 * @param str input character string to process
 * 
 * @return the address of a time_t structure that is correct, or NULL
 */
time_t *wsdate_parse( char* str ) {
  struct tm tm;
  char* cp;
  char str_mon[401] = {0};
  char str_wday[401] = {0};
  int tm_sec, tm_min, tm_hour, tm_mday, tm_year;
  long tm_mon, tm_wday;
  time_t *t;
  
  /* Initialize. */
  memset( (char*) &tm, 0, sizeof(struct tm) );
  t = (time_t*)malloc(sizeof(time_t));
  //if memory for time could not allocate, returnn null immediately
  if(t == NULL) return NULL;
  memset((char*)t, 0, sizeof(time_t));

  /* Skip initial whitespace ourselves - sscanf is clumsy at this. */
  for ( cp = str; *cp == ' ' || *cp == '\t'; ++cp )
    continue;

  /* drop the case of the input character string */
  downcase(cp);
  
  /* And do the sscanfs.  WARNING: you can add more formats here,
  ** but be careful!  You can easily screw up the parsing of existing
  ** formats when you add new ones.  The order is important
  */

    
  /* DD-mth-YY HH:MM:SS GMT */
  if ( sscanf( cp, "%d-%400[a-z]-%d %d:%d:%d GMT",
	       &tm_mday, str_mon, &tm_year, &tm_hour,
	       &tm_min, &tm_sec ) == 6 &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_mday = tm_mday;
    tm.tm_year = tm_year;
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
  }
  
  /* DD mth YY HH:MM:SS GMT */
  else if ( sscanf( cp, "%d %400[a-z] %d %d:%d:%d GMT",
		    &tm_mday, str_mon, &tm_year, &tm_hour, 
		    &tm_min, &tm_sec) == 6 &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_mday = tm_mday;
    tm.tm_year = tm_year;
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
  }
  
  /* HH:MM:SS GMT DD-mth-YY */
  else if ( sscanf( cp, "%d:%d:%d GMT %d-%400[a-z]-%d",
		    &tm_hour, &tm_min, &tm_sec, &tm_mday,
		    str_mon, &tm_year ) == 6 &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
    tm.tm_mday = tm_mday;
    tm.tm_year = tm_year;
  }
  
  /* HH:MM:SS GMT DD mth YY */
  else if ( sscanf( cp, "%d:%d:%d GMT %d %400[a-z] %d",
		    &tm_hour, &tm_min, &tm_sec, &tm_mday, 
		    str_mon, &tm_year ) == 6 &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
    tm.tm_mday = tm_mday;
    tm.tm_year = tm_year;
  }
  
  /* wdy, DD-mth-YY HH:MM:SS GMT */
  else if ( sscanf( cp, "%400[a-z], %d-%400[a-z]-%d %d:%d:%d GMT",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, 
		    &tm_min, &tm_sec ) == 7 &&
	    (tm.tm_wday = scan_wday( str_wday, &tm_wday ) >= 0) &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_mday = tm_mday;
    tm.tm_year = tm_year;
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
  }
  
  /* wdy, DD mth YY HH:MM:SS GMT */
  else if ( sscanf( cp, "%400[a-z], %d %400[a-z] %d %d:%d:%d GMT",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour,
		    &tm_min, &tm_sec ) == 7 &&
	    (tm.tm_wday = scan_wday( str_wday, &tm_wday ) >= 0) &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_mday = tm_mday;
    tm.tm_year = tm_year;
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
  }
  
  /* wdy mth DD HH:MM:SS GMT YY */
  else if ( sscanf( cp, "%400[a-z] %400[a-z] %d %d:%d:%d GMT %d",
		    str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
		    &tm_sec, &tm_year ) == 7 &&
	    (tm.tm_wday = scan_wday( str_wday, &tm_wday ) >= 0) &&
	    (tm.tm_mon = scan_mon( str_mon, &tm_mon ) >= 0) ) {
    tm.tm_mday = tm_mday;
    tm.tm_hour = tm_hour;
    tm.tm_min = tm_min;
    tm.tm_sec = tm_sec;
    tm.tm_year = tm_year;
  }

  /* did not match any of the accepted date formats */
  else {
    return NULL;
  }

  if ( tm.tm_year > 1900 )
    tm.tm_year -= 1900;
  else if ( tm.tm_year < 70 )
    tm.tm_year += 100;

  *t = mktime( &tm );
  
  return t;
}


#ifdef WSDATE_TEST

int main(int argc, char **argv) {
  return 0;
}

#endif
