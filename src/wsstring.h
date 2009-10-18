/**
 * @file   wsstring.h
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 21:51:50 2009
 * 
 * @brief  An extended string library for use with libwebshare
 * 
 * 
 */

#ifndef _WSSTRING_H_
#define _WSSTRING_H_


#include <string.h>

/** 
 * Strips the last character off of a null terminated string
 * 
 * @param string string to process
 * 
 * @return the processed string
 */
char *chop(char *string);

/** 
 * Removes the last character of a string if it is a new line or a
 * line feed
 * 
 * @param string string to strip
 * 
 * @return processed string
 */
char *chomp(char *string);

/** 
 * Calls chomp until the length of the string no logner changes - this will drop all \r
 * and \n at the end of string
 * 
 * @param string the string to strip
 * 
 * @return address of string
 */
char *strip_eol(char *string);

#endif /* _WSSTRING_H_ */
