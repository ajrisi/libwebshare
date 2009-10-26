/*
** types.h
** 
** Made by Adam
** Login   <adam@toxic.rh.rit.edu>
** 
** Started on  Mon Sep 22 19:40:38 2008 Adam
** Last update Mon Sep 22 19:40:38 2008 Adam
*/

#ifndef   	TORUS_TYPES_H_
#define   	TORUS_TYPES_H_

extern const int yes;

/* DATE PROCESSING SECTION ***************************************************/
//data holder
typedef struct {
  unsigned char hours;
  unsigned char minutes;
  unsigned char seconds;
  unsigned char day;
  unsigned char month;
  unsigned short year;
} date_t;

//convert a date to a string
char* __date_to_string(date_t*);

//parse a string to get a date
date_t __string_to_date(char*);

//make a date_t from an epoch
date_t __epoch_to_date(int);

//function holder
const struct {
  char*(*to_string)(date_t*);
  const struct {
    date_t(*string)(char*);
    date_t(*epoch)(int);
  } parse;
} static date = {__date_to_string,
	  {__string_to_date,
	   __epoch_to_date}};

/* END OF DATE PROCESSING ****************************************************/

/* QVALUE PROCESSING SECTION *************************************************/
typedef struct {
  float value;
} qvalue_t;

char* __qvalue_to_string(qvalue_t*);

qvalue_t __string_to_qvalue(char*);

const struct {
  char*(*to_string)(qvalue_t*);
  const struct {
    qvalue_t(*string)(char*);
  } parse;
} static qvalue = {__qvalue_to_string,
	    {__string_to_qvalue}};
/* END OF QVALUE PROCESSING **************************************************/

/* LEVEL PROCESSING SECTION **************************************************/
typedef struct {
  int value;
} level_t;

char* __level_to_string(level_t*);

level_t __string_to_level(char*);

const struct {
  char*(*to_string)(level_t*);
  const struct {
    level_t(*string)(char*);
  } parse;
} static level = {__level_to_string,
	   {__string_to_level}};
/* END OF LEVEL PROCESSING ****************************************************/

/* HOST PROCESSING SECTION ****************************************************/

#define MAX_HOST_STRING_LENGTH 255

typedef struct {
  char* host;
  int port;
} host_t;

/* This converts a host to a string */
char* __host_to_string(host_t*,char*);

host_t __string_to_host(char*);

void __free_host(host_t*);

const struct {
  char*(*to_string)(host_t*,char*);
  void(*free)(host_t*);
  const struct {
    host_t(*string)(char*);
  } parse;
} static host = {__host_to_string,
	  __free_host,
	  {__string_to_host}};

/* END OF HOST PROCESSING SECTION *********************************************/

/* RELATIVE URI PROCESSING SECTION ********************************************/
/*
typedef struct {
  char* path;
} relative_uri_t;

char* __relative_uri_to_string(relative_uri_t*);

relative_uri_t __string_to_relative_uri(char*);

const struct {
  char*(*to_string)(relative_uri_t*);
  const struct {
    relative_uri_t*(*string)(char*);
  } parse;
} relative_uri = {__relative_uri_to_string,
		  {__string_to_relative_uri}};
*/
/* END OF RELATIVE URI PROCESSING SECTION *************************************/

#endif 	    /* !TYPES_H_ */
