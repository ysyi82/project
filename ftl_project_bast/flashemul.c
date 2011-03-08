#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "fm_def.h"

#define FALSE   0
#define TRUE    1

typedef struct {
    char    data[DATASIZE];
    char    spare[SPARESIZE];
    short   data_wcount;    // debugging only
    short   spare_wcount;   // debugging only
} PAGE;

static PAGE flashmemory[NO_OF_BLOCKS*PAGES_PER_BLOCK];

int init_page(int i);

int FM_Open()
{
    return(FM_SUCCESS);
}

int FM_Close()
{
    return(FM_SUCCESS);
}

int FM_Read_Data(char *buffer, unsigned int block, unsigned int page)
{
    if ((block >= NO_OF_BLOCKS) || (page >= PAGES_PER_BLOCK)) {
        return(FM_ILLEGAL_ACCESS);
    }
    memcpy(buffer, flashmemory[block * PAGES_PER_BLOCK + page].data, DATASIZE);
    return(FM_SUCCESS);
}

int FM_Read_Spare(char *buffer, unsigned int block, unsigned int page)
{
    if ((block >= NO_OF_BLOCKS) || (page >= PAGES_PER_BLOCK)) {
        return(FM_ILLEGAL_ACCESS);
    }
    memcpy(buffer, flashmemory[block * PAGES_PER_BLOCK + page].spare, SPARESIZE);
    return(FM_SUCCESS);
}

int FM_Read_Data_Spare(char *buffer, unsigned int block, unsigned int page)
{
    int ret;

    ret = FM_Read_Data(buffer, block, page);
    if (ret != FM_SUCCESS)
        return ret;
    return FM_Read_Spare(buffer+DATASIZE, block, page);
}


int FM_Write_Data(char *buffer, unsigned int block, unsigned int page)
{
    if ((block >= NO_OF_BLOCKS) || (page >= PAGES_PER_BLOCK)) {
        return(FM_ILLEGAL_ACCESS);
    }
    memcpy(flashmemory[block * PAGES_PER_BLOCK + page].data, buffer, DATASIZE);
    flashmemory[block * PAGES_PER_BLOCK + page].data_wcount++;
    return(FM_SUCCESS);
}


int FM_Write_Spare(char *buffer, unsigned int block, unsigned int page)
{
    if ((block >= NO_OF_BLOCKS) || (page >= PAGES_PER_BLOCK)) {
        return(FM_ILLEGAL_ACCESS);
    }
    memcpy(flashmemory[block * PAGES_PER_BLOCK + page].spare, buffer, SPARESIZE);
    flashmemory[block * PAGES_PER_BLOCK + page].spare_wcount++;
    return(FM_SUCCESS);
}

int FM_Write_Data_Spare(char *buffer, unsigned int block, unsigned int page)
{
    int ret;

    ret = FM_Write_Data(buffer, block, page);
    if (ret != FM_SUCCESS)
        return ret;
    return FM_Write_Spare(buffer+DATASIZE, block, page);
}

int FM_Erase(unsigned int block)
{

    unsigned int i;

    if (block >= NO_OF_BLOCKS) {
            return(FM_ILLEGAL_ACCESS);
    }

    for (i = 0; i < PAGES_PER_BLOCK; i++)
        init_page(block * PAGES_PER_BLOCK + i);

    return(FM_SUCCESS);
}

int FM_isBadBlock(unsigned int block)
{
    return(FALSE);
}

int init_page(int i)
{
    memset(flashmemory[i].data, ~0, DATASIZE);
    memset(flashmemory[i].spare, ~0, SPARESIZE);
    flashmemory[i].data_wcount = flashmemory[i].spare_wcount = 0;

	return 0;
}

