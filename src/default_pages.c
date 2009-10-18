#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "reply_codes.h"
#include "default_pages.h"

char *status_page_fmt = 
  "<DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
  "<html xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
  "<title>%s - %s</title>"
  "</head><body>"
  "<h1>%s - %s</h1>"
  "<h2>%s</h2>"
  "</body></html>";

char *status_code_strs[] = { "100", "101",
	      "200", "201", "202", "203", "204", "205", "206",
	      "300", "301", "302", "303", "304", "305", "306", "307",
	      "400", "401", "402", "403", "404", "405", "406", "407", "408", "409", "410", "411",
	      "412", "413", "414", "415", "416", "417",
	      "500", "501", "502", "503", "504", "505"
};


char *status_code_detailed_strs[] = {

  "Continue",
  "Switching Protocols",

  "OK",
  "Created",
  "Accepted",
  "Non-Authoratative Information",
  "No Content",
  "Reset Content",
  "Partial Content",

  "Multiple Choices",
  "Moved Permanently",
  "Found",
  "See Other",
  "Not Modified",
  "Use Proxy",
  "(Unused)",
  "Temporary Redirect",

  "Bad Request",
  "Unauthorized",
  "Payment Required",
  "Forbidden",
  "Not Found",
  "Method Not Allowed",
  "Not Acceptable",
  "Proxy Authentication Required",
  "Request Timeout",
  "Conflict",
  "Gone",
  "Length Required",
  "Precondition Failed",
  "Request Entity Too Large",
  "Request-URI Too Long",
  "Unsupported Media Type",
  "Requested Range Not Satisfiable",
  "Expectation Failed",

  "Internal Server Status",
  "Not Implemented",
  "Bad Gateway",
  "Service Unavailable",
  "Gateway Timeout",
  "HTTP Version Not Supported"

};


char *wsserver_status_page(int status_code, char *details) {
  char *buffer = malloc(1+strlen(status_code_strs[status_code])*2+
			strlen(status_code_detailed_strs[status_code])*2+
			strlen(details) + strlen(status_page_fmt));
  sprintf(buffer, status_page_fmt, status_code_strs[status_code], 
	  status_code_detailed_strs[status_code], 
	  status_code_strs[status_code], 
	  status_code_detailed_strs[status_code], details);
  return buffer;
}
