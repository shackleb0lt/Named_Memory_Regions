#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "global.h"

typedef struct block_node
{
	rsize_t size;
	void * block_start;
	struct block_node * next;
} block_node;

typedef struct reg_node{
    char * name;
    rsize_t size;
    rsize_t bytes_used;
    void * data;
    struct block_node * block_list_head;
    struct reg_node * next;
} reg_node;

static reg_node * reg_head = NULL;

reg_node *insert();
Boolean delete_region(const char* name);
Boolean search_region(const char* name);
reg_node * get_region(const char* name);


block_node * add_block(rsize_t block_size, reg_node * curr_reg);
block_node * find_block(void * block_start, block_node * head);
Boolean delete_block(block_node * block, reg_node * curr_reg);
Boolean destroy_block_list(reg_node * curr_reg);

#endif