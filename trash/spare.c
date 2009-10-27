/*******************************************************************************
 HTTP Request Generation Code
 ******************************************************************************/

//char *http_set_header_opt(struct http_header_s *header, int option, void *data) {
//if it was set before, erase it
//    if (header->header_lines[option] != NULL) {
//        free(header->header_lines[option]);
//    }
//    char *line = malloc(256);
//    memset(line, 0, 256);
//now, generate the line based on the passed option
//switch (option) {
/*        case ACCEPT:
  
  case ACCEPT_CHARSET:
  case ACCEPT_ENCODING:
  case ACCEPT_LANGUAGE:
  case ACCEPT_RANGES:
  case AGE:
  case ALLOW:
  case AUTHORIZATION:
  case CACHE_CONTROL:
  case CONNECTION:
  case CONTENT_ENCODING:
  case CONTENT_LANGUAGE:
  case CONTENT_LENGTH:
  case CONTENT_LOCATION:
  case CONTENT_SHA256:
  case CONTENT_RANGE:
  case CONTENT_TYPE:
  case DATE:
  case ETAG:
  case EXPECT:
  case EXPIRES:
  case FROM:
  case HOST:
  case IF_MATCH:
  case IF_MODIFIED_SINCE:
  case IN_NONE_MATCH:
  case IF_RANGE:
  case IF_UNMODIFIED_SINCE:
  case LAST_MODIFIED:
  case LOCATION:
  case MAX_FORWARDS:
  case PRAGMA:
  case PROXY_AUTHENTICATE:
  case PROXY_AUTHENTICATION:
  case RANGE:
  case REFERER:
  case RETRY_AFTER:
  case SERVER:
  case TE:
  case TRAILER:
  case TRANSFER_ENCODING:
  case UPGRADE:
  case USER_AGENT:
  case VARY:
  case VIA:
  case WARNING:
  case WWW_AUTHENTICATE:*/
//default:
//        header->header_lines[option] = (char*)data;
//        break;
//    }; /*HTTP header option switch*/
//    return header->header_lines[option];
//}

/*http_header *http_make_default_header(int type) {
    struct http_header_s *header = malloc(sizeof(struct http_header_s));
    if(type == REQUEST) {
        //set the defaults up
        //header->header_lines[USER_AGENT] = HTTP_USER_AGENT;
        //header->header_lines[ACCEPT] = HTTP_ACCEPT;
        //header->header_lines[CONNECTION] = HTTP_CONNECTION;
        //header->header_lines[ACCEPT_ENCODING] = HTTP_ACCEPT_ENCODING;
        //header->header_lines[ACCEPT_LANGUAGE] = HTTP_ACCEPT_LANGUAGE;
        //header->header_lines[ACCEPT_RANGES] = HTTP_ACCEPT_RANGES;
        //header->header_lines[HOST] = HTTP_HOST;

        //header->method = http_methods[GET];
        //default header loads the root
        //header->resource = strcpy(malloc(2), "/\0");
    } else if (type == REPLY) {
        //its ok
        header->reply_type = C200;
        char *fl = malloc(200);
        memset(fl, 0, 200);
        strcpy(fl, "HTTP/1.0 ");
        strcat(fl, http_reply_codes[header->reply_type]);
        strcat(fl, "\r\n");
        //header->header_lines[CONTENT_TYPE] =
    } else {
        g_error("Unknown http header type %d", type);
    }
    return header;
}
*/

char *http_make_request(struct http_header_s *header) {
	int i;
	char *header_string;
	
    /* we are building this in memory, so lets allocate some mem for it */
    header_string = malloc(2048);
    memset(header_string, 0, 2048); 

    strcat(header_string, header->method);
    strcat(header_string, " ");
    strcat(header_string, header->resource);
    strcat(header_string, " HTTP/1.1\r\n");

    for(i = 0; i < NUM_HTTP_HEADER_STRINGS; i++) {
        if (header->header_lines[i] != NULL) {
            strcat(header_string, http_header_strings[i]);
            strcat(header_string, ": ");
            strcat(header_string, header->header_lines[i]);
            strcat(header_string, "\r\n");
        }
    }

    //alright, no more header
    strcat(header_string, "\r\n");

    return header_string;
}
/*int parse_http_header_line(char *line) {
    int i  = 0;
    char *repl = strchr(line, ':');
    if (repl != 0) {
        *repl = 0;
    } else {
        //some strange string
    }
    for(i=0;i<NUM_HTTP_HEADER_STRINGS; i++) {
        if (strcasecmp(http_header_strings[i], line) == 0) {
            printf("recognised %s - %s\r\n", line, repl+1);
        }
    }
    return 0;
    }*/

/* void tokit(char *tokme) {
    char *new_tok = strtok(tokme, "\r\n");
    while (new_tok != 0) {
        parse_http_header_line(new_tok);
        new_tok = strtok(0, "\r\n");
    }
} */


/*
//called for all new connections
void *new_connection(void* new_conn) {
    char buffer[1024] = {0};
    struct newth_s *p = (struct newth_s *)new_conn;
    int nbytes = 0;
    while((nbytes = recv(p->losockfd, buffer, 1023, 0)) > 0) {
        printf("%d: %s\r\n", p->id_number, buffer);
        //tokit(buffer);
        if (strstr(buffer, "\r\n\r\n") != NULL) {
            printf("HEADERS ARE DONE\r\n");
            break;
        }
        memset(buffer, 0 , 1024);
    }
    //write a reply!
    char *rep = "HTTP/1.1 409 Conflict\r\n\r\n<html>409 Conflict - like the diamonds</html>";
    send(p->losockfd, rep, strlen(rep), 0);
    if (nbytes == 0) {
        printf("%d CLOSED CONN\r\n", p->id_number);
    } else {
        printf("%d SOME ERROR-%d\r\n", p->id_number, nbytes);
        perror("recv");
    }
    close(p->losockfd);
    free(new_conn);
    return((void*)new_conn);
    }*/

/*
struct http_range http_parse_range(char *str) {
    printf("parsing %s\r\n", str);
    struct string_ll ranges = {str, NULL};
    struct string_ll *current = &ranges;
    //look ma! its a linked list!
    while(current->str != NULL) {
        printf("breaking down %s\r\n", current->str);
        struct string_ll *new_node = malloc(sizeof(struct string_ll));
        new_node->str = strchr(current->str+1, ',');
        if (new_node->str != NULL) new_node->str++;
        printf("on %s, next is %s\r\n", current->str, new_node->str);
        new_node->next = NULL;
        current->next = new_node;
        current = new_node;
    }
    printf("done parsing\r\n");
    current = &ranges;
    replace_char(str, ',', 0);
    do {
        printf("range: %s\r\n", current->str);
        current = current->next;
    } while(current->next != NULL);




    char *pch;
    pch = strtok(str, ",");
    while(pch != NULL) {
        printf("range: %s\r\n", pch);
        pch = strtok(NULL, ",");
    }

    printf("done\r\n");
    struct http_range ret = {0};
    return ret;
}*/


/*

void http_free_header(struct http_header_s *header) {
    //iterate over header data and free all memory associated.
}

int parse_http_header_accept(http_header *header, char* line) {

}

int parse_http_header_accept_charset(http_header *header, char* line){
}
int parse_http_header_accept_encoding(http_header *header, char* line){
}
int parse_http_header_accept_language(http_header *header, char* line){
 }
int parse_http_header_accept_ranges(http_header *header, char* line){
}
int parse_http_header_age(http_header *header, char* line){
}
int parse_http_header_allow(http_header *header, char* line){
}
int parse_http_header_authorization(http_header *header, char* line){
}
int parse_http_header_cache_control(http_header *header, char* line){
}
int parse_http_header_connection(http_header *header, char* line){
}
int parse_http_header_content_encoding(http_header *header, char* line){
}
int parse_http_header_content_language(http_header *header, char* line){
}
int parse_http_header_content_length(http_header *header, char* line){
}
int parse_http_header_content_location(http_header *header, char* line){
}
int parse_http_header_content_sha256(http_header *header, char* line){
}
int parse_http_header_content_range(http_header *header, char* line){
}
int parse_http_header_content_type(http_header *header, char* line){
}
int parse_http_header_date(http_header *header, char* line){
}
int parse_http_header_etag(http_header *header, char* line){
}
int parse_http_header_expect(http_header *header, char* line){
}
int parse_http_header_expires(http_header *header, char* line){
}
int parse_http_header_from(http_header *header, char* line){
}
int parse_http_header_host(http_header *header, char* line){
}
int parse_http_header_if_match(http_header *header, char* line){
}
int parse_http_header_if_modified_since(http_header *header, char* line){
}
int parse_http_header_if_none_match(http_header *header, char* line){
}
int parse_http_header_if_range(http_header *header, char* line){
}
int parse_http_header_if_unmodified_since(http_header *header, char* line){
}
int parse_http_header_last_modified(http_header *header, char* line){
}
int parse_http_header_location(http_header *header, char* line){
}
int parse_http_header_max_forwards(http_header *header, char* line){
}
int parse_http_header_pragma(http_header *header, char* line){
}
int parse_http_header_proxy_authenticate(http_header *header, char* line){
}
int parse_http_header_proxy_authorixation(http_header *header, char* line){
}
int parse_http_header_range(http_header *header, char* line){
}
int parse_http_header_referer(http_header *header, char* line){
}
int parse_http_header_retry_after(http_header *header, char* line){
}
int parse_http_header_server(http_header *header, char* line){
}
int parse_http_header_te(http_header *header, char* line){
}
int parse_http_header_trailer(http_header *header, char* line){
}
int parse_http_header_transfer_encoding(http_header *header, char* line){
}
int parse_http_header_upgrade(http_header *header, char* line){
}
int parse_http_header_user_agent(http_header *header, char* line){
 }
int parse_http_header_vary(http_header *header, char* line){
}
int parse_http_header_via(http_header *header, char* line){
}
int parse_http_header_warning(http_header *header, char* line){
}
int parse_http_header_www_authenticate(http_header *header, char* line){
}

*/

gchar *regex_http_first_line;
gchar *regex_http_general;

gboolean parse_http_header(http_header *header, gchar *header_data) {
    //TODO: sanitize the input of null characters and force the lines to
    //terminate in an expected format (for this program, that will be
    // a carriage return and then a line feed.
    return FALSE;
}

gboolean parse_http_header_line(http_header *header, gchar *line, guint line_num) {
    if (line_num == 0) {
        //in the case that you are processing the first line, we need to
        //extract the method, uri, and http_version.
        parse_first_http_line(header, line);
    } else {
        //if its not the first line, process it like any other line
    }
    return FALSE;
}

#define OVECCOUNT 30

gboolean parse_first_http_line(http_header *header, gchar *line) {
    pcre *myregexp;
    const char *error;
    int erroroffset;
    myregexp = pcre_compile("^([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)$", 0, &error, &erroroffset, NULL);
    if (myregexp != NULL) {
        // myregexp successfully created
        int ovector[OVECCOUNT];
        int rc = 0;
        rc = pcre_exec(myregexp, //the compiled pattern
                       NULL, //no extra data, no studying was done
                       line, //line to match against
                       (int)strlen(line), //length of data to be matched
                       0, //no offset
                       0, //default options
                       ovector, //vector for substring info
                       OVECCOUNT); //max vector size
        if (rc != 4) {
            header->type = HTTP_ERR;
            return FALSE;
        } else {
            //match succeeded and the right number of substrings was
            //captured 2 4 and 6 are used because of the PCRE output vector
            //type where a match is determined as the offset, then the
            //ending offset, and lastly a value we ignore.
            header->method = g_strdup(line+ovector[2]);
            header->uri = g_strdup(line+ovector[4]);
            header->http_version = g_strdup(line+ovector[6]);

            //note that the URI is not sanitized in a way you might
            //expect. Because plugins should have the option of working with
            //a URI as much as they want, they should have the opportunity
            //of being able to sanitize URI's for their owh use. For
            //example, the file system access plugin will sanitize URI to
            //not have ../ or ./ in them, to prevent path manipulation by a
            //malicious user.

            //TODO: make sure that the values conform to standards
            return TRUE;
        }
    } else {
        // Syntax error in the regular expression at erroroffset
        return FALSE;
    }
}

void http_parser_init() {
    //strangely, we need to have pointers to integers in order to have
    //them used as the value for the hash table. Because of this, the
    //simple approach is to create an array of integers that count up.

    //add all of the http header strings to the header fileds hash map
    header_fields = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);
    int i;
    for (i = 0; i < NUM_REPLY_CODES; i++) {
        int *ipointer = g_new0(int, 1);
        *ipointer = i;
        g_hash_table_insert(header_fields, (gpointer)http_header_strings[i], (gpointer)ipointer);
    }
}


void http_parser_free() {
    g_hash_table_destroy(header_fields);
}

/* Load the http server configuration data. */
void http_load_config() {
    HTTP_SERVER_PORT = 8081;
    HTTP_SERVER_CLIENT_THREAD_POOL_SIZE = 10;
    HTTP_SERVER_REQUEST_THREAD_POOL_SIZE = 20;

    //sanity checks for the loaded configuration
    if (HTTP_SERVER_CLIENT_THREAD_POOL_SIZE > HTTP_SERVER_REQUEST_THREAD_POOL_SIZE)
        g_warning("Generaly, the number of allowed clients should be less than the number of\n"
                  "allowed concurrent requests");
}

/* http_server_construct_socket
 *
 * construct the main http server socket
 */
void http_server_construct_socket() {
    char yes = 1;

    HTTP_SERVER_MAIN_FD = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(HTTP_SERVER_MAIN_FD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
        g_critical("Could not set options on torus http server socket.");
        exit(1);
    }

}

void http_server_bind_socket() {
    HTTP_SERVER_LOCAL_ADDRESS.sin_family = AF_INET;
    HTTP_SERVER_LOCAL_ADDRESS.sin_port = htons(HTTP_SERVER_PORT);
    HTTP_SERVER_LOCAL_ADDRESS.sin_addr.s_addr = INADDR_ANY;
    memset(HTTP_SERVER_LOCAL_ADDRESS.sin_zero, 0, sizeof(HTTP_SERVER_LOCAL_ADDRESS.sin_zero));

    if (bind(HTTP_SERVER_MAIN_FD, (struct sockaddr*)&HTTP_SERVER_LOCAL_ADDRESS, sizeof(HTTP_SERVER_LOCAL_ADDRESS)) != 0) {
        g_critical("Could not bind torus http server.");
        exit(1);
    }

    //send to the log a message about the bound local address and port
    g_message("bound server to %s port %d", inet_ntoa(HTTP_SERVER_LOCAL_ADDRESS.sin_addr), HTTP_SERVER_PORT);
}

void http_server_start_listening() {
    if (listen(HTTP_SERVER_MAIN_FD, HTTP_SERVER_CLIENT_THREAD_POOL_SIZE) != 0) {
        g_critical("torus http server could not start listening for connections.");
        exit(1);
    }
}

/*******************************************************************************
 * HTTP header line parsing routines
 ******************************************************************************/
/*int parse_http_header_accept(http_header *header, char* line);
int parse_http_header_accept_charset(http_header *header, char* line);
int parse_http_header_accept_encoding(http_header *header, char* line);
int parse_http_header_accept_language(http_header *header, char* line);
int parse_http_header_accept_ranges(http_header *header, char* line);
int parse_http_header_age(http_header *header, char* line);
int parse_http_header_allow(http_header *header, char* line);
int parse_http_header_authorization(http_header *header, char* line);
int parse_http_header_cache_control(http_header *header, char* line);
int parse_http_header_connection(http_header *header, char* line);
int parse_http_header_content_encoding(http_header *header, char* line);
int parse_http_header_content_language(http_header *header, char* line);
int parse_http_header_content_length(http_header *header, char* line);
int parse_http_header_content_location(http_header *header, char* line);
int parse_http_header_content_sha256(http_header *header, char* line);
int parse_http_header_content_range(http_header *header, char* line);
int parse_http_header_content_type(http_header *header, char* line);
int parse_http_header_date(http_header *header, char* line);
int parse_http_header_etag(http_header *header, char* line);
int parse_http_header_expect(http_header *header, char* line);
int parse_http_header_expires(http_header *header, char* line);
int parse_http_header_from(http_header *header, char* line);
int parse_http_header_host(http_header *header, char* line);
int parse_http_header_if_match(http_header *header, char* line);
int parse_http_header_if_modified_since(http_header *header, char* line);
int parse_http_header_if_none_match(http_header *header, char* line);
int parse_http_header_if_range(http_header *header, char* line);
int parse_http_header_if_unmodified_since(http_header *header, char* line);
int parse_http_header_last_modified(http_header *header, char* line);
int parse_http_header_location(http_header *header, char* line);
int parse_http_header_max_forwards(http_header *header, char* line);
int parse_http_header_pragma(http_header *header, char* line);
int parse_http_header_proxy_authenticate(http_header *header, char* line);
int parse_http_header_proxy_authorixation(http_header *header, char* line);
int parse_http_header_range(http_header *header, char* line);
int parse_http_header_referer(http_header *header, char* line);
int parse_http_header_retry_after(http_header *header, char* line);
int parse_http_header_server(http_header *header, char* line);
int parse_http_header_te(http_header *header, char* line);
int parse_http_header_trailer(http_header *header, char* line);
int parse_http_header_transfer_encoding(http_header *header, char* line);
int parse_http_header_upgrade(http_header *header, char* line);
int parse_http_header_user_agent(http_header *header, char* line);
int parse_http_header_vary(http_header *header, char* line);
int parse_http_header_via(http_header *header, char* line);
int parse_http_header_warning(http_header *header, char* line);
int parse_http_header_www_authenticate(http_header *header, char* line);*/
/** END OF LINE PARSING ROUTINES ***********************************************/


/*void *new_connection(void* new_conn);
int parse_http_header_line(char *line);
struct http_header_s *http_make_header();
char *replace_char(char *str, char oldc, char newc);

char *http_set_header_opt(struct http_header_s *header, int option, void *data);
char *http_make_header_string(struct http_header_s *header);
struct http_header_s *http_make_default_header(int type);
void http_free_header(struct http_header_s *header);
*/
/*
extern gchar *regex_http_first_line;
extern gchar *regex_http_general;

extern GHashTable *header_fields;

void http_parser_init(void);

void http_parser_free(void);


gboolean parse_first_http_line(http_header *header, gchar *line);

void http_load_config(void);

void http_server_construct_socket();
void http_server_bind_socket();
void http_server_start_listening();
void http_server_main();
*/
