#ifndef _REGIONS_H
#define _REGIONS_H

#include "global.h"


Boolean rinit(const char *region_name, rsize_t region_size);
Boolean rchoose(const char *region_name);
const char *rchosen();
void *ralloc(rsize_t block_size);
rsize_t rsize(void *block_ptr);
Boolean rfree(void *block_ptr);
void rdestroy(const char *region_name);
void rdump();

#endif
