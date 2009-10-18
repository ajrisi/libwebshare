#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "wsfs.h"

int path_is_directory(const char *path) 
{
  struct stat filestat = {0};
  int getstat = -1;
  int isfolder = 0;
  getstat = stat(path, &filestat);
  if(getstat == 0)
    if(S_ISDIR(filestat.st_mode))
      isfolder = 1;
    else
      isfolder = 0;
  else
    //isfolder = -1;
    isfolder = 0;

  return isfolder;
}

int path_is_regular(const char *path) 
{
  struct stat filestat = {0};
  int getstat = -1;
  int isreg = 0;
  getstat = stat(path, &filestat);
  if(getstat == 0)
    if(S_ISREG(filestat.st_mode))
      isreg = 1;
    else
      isreg = 0;
  else
    //isreg = -1;
    isreg = 0;

  return isreg;
}

void list_folder(char* folder_name) {
  DIR *dp;
  struct dirent *entry;
  dp = opendir(folder_name);
  int res = 0;
  char* tmpbuf;
  while((entry = readdir(dp)) != NULL) {
    printf("%s/%s\n", folder_name , entry->d_name);
  }
  closedir(dp);
}

#ifdef TESTWSFS

int main(int argc, char **argv) {
  list_folder(argv[1]);
  return 0;
}

#endif
