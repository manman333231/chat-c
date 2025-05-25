#ifndef SERVER_MACROS_H
#define SERVER_MACROS_H

#define MAX_NAME 32
#define MAX_CLIENTS 64
#define MAX_MESG 96
#define MAX_RECV 128
#define MAX_RESP 129

// error mesgs

#define USAGE_ERROR_NONE "commands are: m (receiver name) (mesg), b (mesg), l, h"
#define USAGE_ERROR_MESG "for message command: m (receiver name) (mesg)"
#define USAGE_ERROR_CNF "client not found! use l command to view active clients"
#define USAGE_ERROR_BRDCST "for broadcast command: b (mesg)"
#define USAGE_ERROR_LIST "for list command: l"
#define USAGE_ERROR_HELP "for help command: h"

// macros for serving

#define LIST "lst"
#define ALL -2

#endif
