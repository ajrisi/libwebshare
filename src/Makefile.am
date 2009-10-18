METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src $(GLIB_CFLAGS)

lib_LIBRARIES = libwebshare.a
libwebshare_adir = $(includedir)/webshare
libwebshare_a_HEADERS = wspagetable.h \
			http.h \
			hash.h \
			trie.h \
			mime.h \
			wserror.h \
			wsfs.h \
			wsinet.h \
			webshare.h
libwebshare_a_SOURCES = $(libwebshare_a_HEADERS) \
			wspagetable.c \
			http.c \
			hash.c \
			trie.c \
			mime.c \
			wserror.c \
			wsfs.c \
			wsinet.c \
			webshare.c
