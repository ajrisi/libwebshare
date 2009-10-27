#ifndef _WSFS_H_
#define _WSFS_H_

/** 
 * detect if a path is a directory
 * 
 * @param path the path to check
 * 
 * @return 1 if path is a directory, 0 if it isnt, and -1 on error
 */
int path_is_directory(const char *path);


/** 
 * Detect of a path is "normal", meaning not a sylink.
 * 
 * @param path path to check
 * 
 * @return 1 if path is normal, 0 if not, and -1 on error
 */
int path_is_normal(const char *path);

/** 
 * Detect if a path is regular, this really is too fuzzy a def, and it
 * needs fixing
 * 
 * @param path path to check
 * 
 * @return 1 on regular, 0 on not
 */
int path_is_regular(const char *path);


#endif
