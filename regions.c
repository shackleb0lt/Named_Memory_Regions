#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "regions.h"
#include "linked_list.h"

static reg_node * active_reg = NULL;

rsize_t round_block(rsize_t size)
{
    // if(size < 0) return -1;
    if(size > RSIZE_MAX) return RSIZE_MAX;

    else if((size > 0) && (size % BLOCK_ALIGN !=0))
    {
        return (size + BLOCK_ALIGN - (size % BLOCK_ALIGN));
    }

    return size;
}

void set_zero(block_node * temp)
{
    if(temp==NULL) return;
    unsigned char * ptr;
    for(ptr = temp->block_start;(void *)ptr < temp->block_start + temp->size; ptr++) *ptr = 0;
}

Boolean rinit(const char * region_name, rsize_t region_size)
{
    if(region_name==NULL || search_region(region_name)!=FALSE || region_size > RSIZE_MAX) return FALSE;
    
    active_reg = insert();

    if(active_reg != NULL)
    {
        active_reg->name = strdup(region_name);
    
        if(active_reg->name != NULL){

            active_reg->size = round_block(region_size);
            active_reg->bytes_used = 0;
            active_reg->data = malloc(active_reg->size);

            if(active_reg->data != NULL)
            {
                active_reg->block_list_head = NULL;
                active_reg->next = NULL;
                if(reg_head==NULL) reg_head = active_reg;
                else {
                    active_reg->next = reg_head;
                    reg_head = active_reg;
                }
                return TRUE;
                
            }
            free(active_reg->data);
        }
        free(active_reg->name);
    }

    free(active_reg);
    active_reg = NULL;
    return FALSE;    
}

Boolean rchoose(const char* region_name)
{
    if(region_name==NULL) return FALSE;
    
    reg_node * temp = get_region(region_name);
    
    if(temp==NULL) return FALSE;

    active_reg = temp;
    
    return TRUE;
}

const char * rchosen()
{
    if(active_reg == NULL) return NULL;
    const char * region_name = active_reg->name;
    return region_name;
}

void * ralloc(rsize_t block_size)
{
    if( block_size > active_reg->size || active_reg == NULL || active_reg->data == NULL) return NULL;
    
    rsize_t rounded = round_block(block_size);
    
    block_node * new_block = add_block(rounded,active_reg);
    
    if(new_block==NULL) return NULL;

    set_zero(new_block);

    active_reg->bytes_used += new_block->size;
    
    return new_block->block_start;
}

rsize_t rsize(void * block_ptr)
{
    if (block_ptr==NULL || active_reg==NULL) return 0;
    
    block_node * temp = find_block(block_ptr,active_reg->block_list_head);
    
    if (temp==NULL) return 0;

    return temp->size;
}

Boolean rfree(void * block_ptr)
{
    if(block_ptr==NULL || active_reg==NULL) return FALSE;

    block_node * temp = find_block(block_ptr,active_reg->block_list_head);

    if(temp==NULL) return FALSE;
    int sz = temp->size;
    
    if(delete_block(temp,active_reg)==FALSE) return FALSE;
    active_reg->bytes_used -= sz;
    return TRUE;
}

void rdestroy(const char * region_name)
{
    if(region_name==NULL) return;

    reg_node * temp = get_region(region_name);
    
    if(temp==NULL) return;

    if(destroy_block_list(temp)==FALSE) return;

    if(active_reg==temp && active_reg!=reg_head) active_reg=reg_head;
    else if(active_reg==temp && active_reg==reg_head)
    {
        reg_head=reg_head->next;
        active_reg=reg_head;
    }

    if(delete_region(region_name)==FALSE) return;
}

void rdump()
{
    reg_node * reg_itr = reg_head;
    block_node * block_itr;
    float percent_free;
    while(reg_itr!=NULL)
    {
        printf("Region Name: %s\n",reg_itr->name);
        printf("Size (Bytes): %d\n",reg_itr->size);
        
        percent_free = 100.0*(1.0 - (float)active_reg->bytes_used/(float)active_reg->size);
        printf("Free Space: %.3f\n\n",percent_free);

        block_itr = reg_itr->block_list_head;

        if(block_itr==NULL)
        {
            printf("No Block in Usage\n");
        }
        while (block_itr!=NULL )
        {
            printf("\t%p\n", block_itr->block_start);
			printf("\t%d bytes\n\n", block_itr->size);
            block_itr = block_itr->next;
        }
        printf("\n");
        
        reg_itr = reg_itr->next;
    }
}