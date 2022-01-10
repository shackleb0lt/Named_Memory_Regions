#ifndef _GLOBALS_H
#define _GLOBALS_H

#define BLOCK_ALIGN 8
#define RSIZE_MAX ((1<<16)-8)

typedef enum
{
    FALSE,
    TRUE
} Boolean;

typedef unsigned short rsize_t;
typedef unsigned long int uint64_t;


#endif