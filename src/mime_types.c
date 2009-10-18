#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash.h"
#include "trie.h"
#include "mime_types.h"

static Trie *mime_trie;
static hshtbl *mime_table;

mime_table_item mime_table_data[] = {
  { "a", "application/octet-stream"},
  { "aab", "application/x-authorware-bin"},
  { "aam", "application/x-authorware-map"},
  { "aas", "application/x-authorware-seg"},
  { "ai", "application/postscript"},
  { "aif", "audio/x-aiff"},
  { "aifc", "audio/x-aiff"},
  { "aiff", "audio/x-aiff"},
  { "asc", "text/plain"},
  { "asf", "video/x-ms-asf"},
  { "asx", "video/x-ms-asf"},
  { "au", "audio/basic"},
  { "avi", "video/x-msvideo"},
  { "bcpio", "application/x-bcpio"},
  { "bin", "application/octet-stream"},
  { "bmp", "image/bmp"},
  { "cdf", "application/x-netcdf"},
  { "class", "application/x-java-vm"},
  { "cpio", "application/x-cpio"},
  { "cpt", "application/mac-compactpro"},
  { "crl", "application/x-pkcs7-crl"},
  { "crt", "application/x-x509-ca-cert"},
  { "csh", "application/x-csh"},
  { "css", "text/css"},
  { "dcr", "application/x-director"},
  { "dir", "application/x-director"},
  { "djv", "image/vnd.djvu"},
  { "djvu", "image/vnd.djvu"},
  { "dll", "application/octet-stream"},
  { "dms", "application/octet-stream"},
  { "doc", "application/msword"},
  { "dtd", "text/xml"},
  { "dump", "application/octet-stream"},
  { "dvi", "application/x-dvi"},
  { "dxr", "application/x-director"},
  { "eps", "application/postscript"},
  { "etx", "text/x-setext"},
  { "exe", "application/octet-stream"},
  { "ez", "application/andrew-inset"},
  { "fgd", "application/x-director"},
  { "fh", "image/x-freehand"},
  { "fh4", "image/x-freehand"},
  { "fh5", "image/x-freehand"},
  { "fh7", "image/x-freehand"},
  { "fhc", "image/x-freehand"},
  { "gif", "image/gif"},
  { "gtar", "application/x-gtar"},
  { "hdf", "application/x-hdf"},
  { "hqx", "application/mac-binhex40"},
  { "htm", "text/html; charset=%s"},
  { "html", "text/html; charset=%s"},
  { "ice", "x-conference/x-cooltalk"},
  { "ief", "image/ief"},
  { "iges", "model/iges"},
  { "igs", "model/iges"},
  { "iv", "application/x-inventor"},
  { "jar", "application/x-java-archive"},
  { "jfif", "image/jpeg"},
  { "jpe", "image/jpeg"},
  { "jpeg", "image/jpeg"},
  { "jpg", "image/jpeg"},
  { "js", "application/x-javascript"},
  { "kar", "audio/midi"},
  { "latex", "application/x-latex"},
  { "lha", "application/octet-stream"},
  { "lzh", "application/octet-stream"},
  { "m3u", "audio/x-mpegurl"},
  { "man", "application/x-troff-man"},
  { "mathml", "application/mathml+xml"},
  { "me", "application/x-troff-me"},
  { "mesh", "model/mesh"},
  { "mid", "audio/midi"},
  { "midi", "audio/midi"},
  { "mif", "application/vnd.mif"},
  { "mime", "message/rfc822"},
  { "mml", "application/mathml+xml"},
  { "mov", "video/quicktime"},
  { "movie", "video/x-sgi-movie"},
  { "mp2", "audio/mpeg"},
  { "mp3", "audio/mpeg"},
  { "mp4", "video/mp4"},
  { "mpe", "video/mpeg"},
  { "mpeg", "video/mpeg"},
  { "mpg", "video/mpeg"},
  { "mpga", "audio/mpeg"},
  { "ms", "application/x-troff-ms"},
  { "msh", "model/mesh"},
  { "mv", "video/x-sgi-movie"},
  { "mxu", "video/vnd.mpegurl"},
  { "nc", "application/x-netcdf"},
  { "o", "application/octet-stream"},
  { "oda", "application/oda"},
  { "ogg", "application/x-ogg"},
  { "pac", "application/x-ns-proxy-autoconfig"},
  { "pbm", "image/x-portable-bitmap"},
  { "pdb", "chemical/x-pdb"},
  { "pdf", "application/pdf"},
  { "pgm", "image/x-portable-graymap"},
  { "pgn", "application/x-chess-pgn"},
  { "png", "image/png"},
  { "pnm", "image/x-portable-anymap"},
  { "ppm", "image/x-portable-pixmap"},
  { "ppt", "application/vnd.ms-powerpoint"},
  { "ps", "application/postscript"},
  { "qt", "video/quicktime"},
  { "ra", "audio/x-realaudio"},
  { "ram", "audio/x-pn-realaudio"},
  { "ras", "image/x-cmu-raster"},
  { "rdf", "application/rdf+xml"},
  { "rgb", "image/x-rgb"},
  { "rm", "audio/x-pn-realaudio"},
  { "roff", "application/x-troff"},
  { "rpm", "audio/x-pn-realaudio-plugin"},
  { "rss", "application/rss+xml"},
  { "rtf", "text/rtf"},
  { "rtx", "text/richtext"},
  { "sgm", "text/sgml"},
  { "sgml", "text/sgml"},
  { "sh", "application/x-sh"},
  { "shar", "application/x-shar"},
  { "silo", "model/mesh"},
  { "sit", "application/x-stuffit"},
  { "skd", "application/x-koan"},
  { "skm", "application/x-koan"},
  { "skp", "application/x-koan"},
  { "skt", "application/x-koan"},
  { "smi", "application/smil"},
  { "smil", "application/smil"},
  { "snd", "audio/basic"},
  { "so", "application/octet-stream"},
  { "spl", "application/x-futuresplash"},
  { "src", "application/x-wais-source"},
  { "stc", "application/vnd.sun.xml.calc.template"},
  { "std", "application/vnd.sun.xml.draw.template"},
  { "sti", "application/vnd.sun.xml.impress.template"},
  { "stw", "application/vnd.sun.xml.writer.template"},
  { "sv4cpio", "application/x-sv4cpio"},
  { "sv4crc", "application/x-sv4crc"},
  { "svg", "image/svg+xml"},
  { "svgz", "image/svg+xml"},
  { "swf", "application/x-shockwave-flash"},
  { "sxc", "application/vnd.sun.xml.calc"},
  { "sxd", "application/vnd.sun.xml.draw"},
  { "sxg", "application/vnd.sun.xml.writer.global"},
  { "sxi", "application/vnd.sun.xml.impress"},
  { "sxm", "application/vnd.sun.xml.math"},
  { "sxw", "application/vnd.sun.xml.writer"},
  { "t", "application/x-troff"},
  { "tar", "application/x-tar"},
  { "tcl", "application/x-tcl"},
  { "tex", "application/x-tex"},
  { "texi", "application/x-texinfo"},
  { "texinfo", "application/x-texinfo"},
  { "tif", "image/tiff"},
  { "tiff", "image/tiff"},
  { "torrent", "application/x-bittorrent"},
  { "tr", "application/x-troff"},
  { "tsp", "application/dsptype"},
  { "tsv", "text/tab-separated-values"},
  { "txt", "text/plain"},
  { "ustar", "application/x-ustar"},
  { "vcd", "application/x-cdlink"},
  { "vrml", "model/vrml"},
  { "vx", "video/x-rad-screenplay"},
  { "wav", "audio/x-wav"},
  { "wax", "audio/x-ms-wax"},
  { "wbmp", "image/vnd.wap.wbmp"},
  { "wbxml", "application/vnd.wap.wbxml"},
  { "wm", "video/x-ms-wm"},
  { "wma", "audio/x-ms-wma"},
  { "wmd", "application/x-ms-wmd"},
  { "wml", "text/vnd.wap.wml"},
  { "wmlc", "application/vnd.wap.wmlc"},
  { "wmls", "text/vnd.wap.wmlscript"},
  { "wmlsc", "application/vnd.wap.wmlscriptc"},
  { "wmv", "video/x-ms-wmv"},
  { "wmx", "video/x-ms-wmx"},
  { "wmz", "application/x-ms-wmz"},
  { "wrl", "model/vrml"},
  { "wsrc", "application/x-wais-source"},
  { "wvx", "video/x-ms-wvx"},
  { "xbm", "image/x-xbitmap"},
  { "xht", "application/xhtml+xml"},
  { "xhtml", "application/xhtml+xml"},
  { "xls", "application/vnd.ms-excel"},
  { "xml", "text/xml"},
  { "xpm", "image/x-xpixmap"},
  { "xsl", "text/xml"},
  { "xwd", "image/x-xwindowdump"},
  { "xyz", "chemical/x-xyz"},
  { "zip", "application/zip"},
  {NULL, NULL} /* indicates the end of the list */
};



int mime_table_item_compare(void *lt, void *rt) {
  mime_table_item *left = lt;
  mime_table_item *right = rt;
  return strcmp(left->key, right->key);
}

void *mime_table_item_dup(void *it) {
  mime_table_item *old_item = it;
  mime_table_item *new_item = malloc(sizeof(mime_table_item));
  if(new_item == NULL) return NULL;

  new_item->key = malloc(strlen(old_item->key)+1);
  if(new_item->key == NULL) {
    free(new_item);
    return NULL;
  }
  strcpy(new_item->key, old_item->key);

  new_item->value = malloc(strlen(old_item->value)+1);
  if(new_item->value == NULL) {
    free(new_item->key);
    free(new_item);
    return NULL;
  }
  strcpy(new_item->value, old_item->value);

  return new_item;
}

void mime_table_item_free(void *it) {
  mime_table_item *item = it;
  free(item->key);
  free(item->value);
  free(item);
}

unsigned long mime_table_item_hash(void *it) {
  mime_table_item *myitem = it;
  return hshstrhash_sdbm(myitem->key);
}

unsigned long mime_table_item_rehash(void *it) {
  mime_table_item *myitem = it;
  return hshstrhash_rs(myitem->key);
}


int mime_table_init() {
  mime_table = hshinit(mime_table_item_hash, mime_table_item_rehash,
		       mime_table_item_compare, mime_table_item_dup, 
		       mime_table_item_free, 0);

  if(mime_table == NULL) return 1;

  /* now load the table with mime types */
  mime_table_item *it = &mime_table_data[0];
  while(it->key != NULL && it->value != NULL)
    if(hshinsert(mime_table, it++) == NULL) return 1;

  return 0;
}

void mime_table_free() {
  hshkill(mime_table);
}

char *wsserver_mime_lookup(char *extension) {
  /* create an item to search for */
  mime_table_item lookfor = {0};
  lookfor.key = extension;

  mime_table_item *it = hshfind(mime_table, &lookfor);
  if(it == NULL) return UNKNOWN_MIME;
  else return it->value;
}

char *wsserver_mime_auto(char *sharepath) {
  /* determine the extension of the file being shared, then do a mime lookup */
  char  *extension = sharepath;
  while(strchr(extension, '.') != NULL) extension = strchr(extension, '.')+1;
  return wsserver_mime_lookup(extension);
}

#ifdef MIMETYPESTEST

int main(int argc, char **argv) {
  if(mime_table_init() != 0) {
    fprintf(stderr, "Could not initialize the mime table\n");
  } else {
    printf("mime table all set up\n");
  }
  
  printf("a .torrent file has the mime type %s\n", wsserver_mime_auto("/myshare/my.file.torrent"));
  printf("a .torrent file has the mime type %s\n", wsserver_mime_lookup("torrent"));
  mime_table_free();
  return 0;
}

#endif
