/**
 * @file   reply_codes.h
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Sun Feb 15 19:27:23 2009
 * 
 * @brief  Reply codes for html
 * 
 * 
 */

#ifndef _REPLY_CODES_H_
#define _REPLY_CODES_H_

enum {
  //Informational 1xx
  C100,	C101,
  //Successful 2xx
  C200,	C201,	C202,	C203,	C204,	C205,	C206,
  //Redirection 3xx
  C300,	C301,	C302,	C303,	C304,	C305,	C306,	C307,
  //Client Error 4xx
  C400,	C401,	C402,	C403,	C404,	C405,	C406,	C407,	C408,
  C409,	C410,	C411,	C412,	C413,	C414,	C415,	C416,	C417,
  //Server Error 5xx
  C500,	C501,	C502,	C503,	C504,	C505,

  CNUM
};

#endif

