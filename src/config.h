/**
 * @file   config.h
 * @author Adam Risi <ajrisi@gmail.com>
 * @date   Mon Mar  2 05:06:58 2009
 * 
 * @brief  General program configuration file
 * 
 * 
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_


#define UNKNOWN_MIME "application/octet-stream"

#define DEFAULT_PORT 8081

#define USER_AGENT "webshare"

#define DEFAULT_HOST "*"

/* the maximim number of bytes to read from incoming http headers */
#define HTTP_MAX_REQUEST_HEADER_SIZE 2048

#define THREAD_POOL_SIZE 10

#endif /* _CONFIG_H_ */
