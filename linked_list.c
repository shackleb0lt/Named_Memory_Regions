#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "linked_list.h"

reg_node * insert()
{
    reg_node * new_reg = (reg_node *)malloc(sizeof(reg_node));
    
    if (new_reg == NULL) return NULL;
    
    new_reg->next = reg_head;
    reg_head = new_reg;
    return new_reg;
}

Boolean search_region(const char * name)
{
    reg_node * itr = reg_head;
    if (name == NULL) return FALSE;
    while(itr != NULL && itr->name != NULL)
    {
        if (strcmp(name,itr->name) == 0) return TRUE;
        itr = itr->next;
    }
    return FALSE;
}

Boolean delete_region(const char * name)
{
    if (name == NULL) return FALSE;

    reg_node *itr = reg_head;
    reg_node *prev_itr = NULL;

    while(itr!=NULL && itr->name!=NULL)
    {
        if (strcmp(itr->name,name) == 0) break;

        prev_itr = itr;
        itr = itr->next;
    }

    if (itr != NULL && itr->name!=NULL && strcmp(itr->name,name) == 0)
    {
        if (prev_itr == NULL) reg_head = itr->next;

        else prev_itr->next = itr->next;

        free(itr->name);
        free(itr->data);
        free(itr);

        return TRUE;
    }

    return FALSE;
}

reg_node * get_region(const char * name)
{
    if (name == NULL) return NULL;
    reg_node * itr = reg_head;
    while(itr!=NULL && itr->name!=NULL)
    {
        if (strcmp(itr->name,name)==0) return itr;
        itr = itr->next;
    }
    return NULL;
}
    
block_node * add_block(rsize_t block_size, reg_node * curr_reg)
{
    if ((block_size%BLOCK_ALIGN != 0) || (curr_reg->data==NULL)) return NULL;

    
    block_node * new_block = (block_node *)malloc(sizeof(block_node));
    if (new_block == NULL) return NULL;

    new_block->size = block_size;
    new_block->next = NULL;
    

    if (curr_reg->block_list_head == NULL && curr_reg->size >= block_size)
    {
        curr_reg->block_list_head = new_block;
        new_block->block_start = curr_reg->data;
        return new_block;
    }

    block_node * itr = curr_reg->block_list_head->next;
    block_node * prev_itr = curr_reg->block_list_head;

    if(prev_itr->block_start!=curr_reg->data)
    {
        
        uint64_t l = (uint64_t) curr_reg->data;
        uint64_t r = (uint64_t) prev_itr->block_start;
        
        if((r-l)>=new_block->size)
        {
            new_block->block_start = curr_reg->data;
            new_block->next = curr_reg->block_list_head;
            curr_reg->block_list_head = new_block;
            return new_block;
        }
    }
    
    while(prev_itr!=NULL)
    {
        uint64_t l = (uint64_t)prev_itr->block_start + prev_itr->size;
        uint64_t r;
        if (itr != NULL) r = (uint64_t)itr->block_start;

        else r = (uint64_t)curr_reg->data + curr_reg->size;
        
        if ((r-l)>=new_block->size)
        {
            new_block->block_start = prev_itr->block_start + prev_itr->size;
            prev_itr->next = new_block;
            new_block->next = itr;
            return new_block;
        }
        prev_itr = itr;

        if(itr == NULL) break;
        itr = itr->next;
    }

    free(new_block);
    return NULL;

}

block_node * find_block(void * block_start, block_node * head)
{
    if (block_start==NULL || head==NULL) return NULL;

    block_node * itr = head;

    while(itr!=NULL && itr->block_start!=block_start) itr = itr->next;
    
    return itr;
}

Boolean delete_block(block_node * block, reg_node * curr_reg)
{
    if(block==NULL || curr_reg==NULL) return FALSE;

    block_node * itr = curr_reg->block_list_head;
    block_node * prev_itr = NULL;
    while(itr!=NULL)
    {
        if(itr==block)
        {
            if(prev_itr==NULL)
            {
                curr_reg->block_list_head = itr->next;
            }
            else
            {
                prev_itr->next = itr->next;
            }
            free(block);
            return TRUE;
        }
        prev_itr = itr;
        itr = itr->next;
    }
    return FALSE;
}

Boolean destroy_block_list(reg_node * curr_reg)
{
    if(curr_reg->block_list_head==NULL) return TRUE;

    block_node * itr = curr_reg->block_list_head;
    block_node * temp;

    curr_reg->block_list_head = NULL;

    while(itr!=NULL)
    {
        temp = itr;
        itr = itr->next;
        free(temp);
    }
    return TRUE;
}