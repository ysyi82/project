#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fm_def.h"
#include "nftl.h"

#define TRUE 1
#define FALSE 0

#define SECTORSIZE  DATASIZE

int main(void)
{
#if 1
    int     NumSector;
    char    buf[SECTORSIZE];
    int     ret;

    ftl_format();

    ret = ftl_open();
    if (ret != FTL_SUCCESS) 
	{
        printf("Open Error code = %d\n", ret);
        exit(1);
    }

    NumSector = ftl_query_num_sectors();
    printf("NumSector = %d\n", NumSector);
	
	memset( buf, 'a', SECTORSIZE );
	ret = ftl_write(buf, 1);
	memset( buf, '1', SECTORSIZE );
	ftl_read(buf, 1);
//-----------------------------------
	memset( buf, 'b', SECTORSIZE );
    ret = ftl_write(buf, 4);
	memset( buf, '1', SECTORSIZE );
	ftl_read(buf, 4);
//-----------------------------------
	memset( buf, 'c', SECTORSIZE );
    ret = ftl_write(buf, 7);
	memset( buf, '1', SECTORSIZE );
	ftl_read(buf, 7);
//-----------------------------------
	return 0;
#else
    int     i, k, blkno;
    char    buf[SECTORSIZE], w_char;
    int     ret, NumSector;
    char    *sec_data;

    ftl_format();

    ret = ftl_open();
    if (ret != FTL_SUCCESS) {
        printf("Open Error code = %d\n", ret);
        exit(1);
    }

    NumSector = ftl_query_num_sectors();
    printf("NumSector = %d\n", NumSector);

    sec_data = malloc(NumSector);
    for (i = 0; i < NumSector; i++) sec_data[i] = (char) 0xFF;

    //for (i = 0; i < 10000; i++) 
	while( 1 )
	{
        blkno = rand() % NumSector;

        ret = ftl_read(buf, blkno);
        if(ret != FTL_SUCCESS) {
             printf("read error blk = %d, code = %d", blkno, ret);
            exit(1);
        }
        for (k = 0; k < SECTORSIZE; k++) {
            if (buf[k] != sec_data[blkno]) break;
        }
        if (k < SECTORSIZE) {
            printf("\nInvalid Old data, blkno = %d, sec_data = %2d, buf = %2d\n", blkno, sec_data[blkno], buf[k]);
            exit(1);
        }

        w_char = rand() % 0xFE;
        for (k = 0; k < SECTORSIZE; k++) buf[k] = w_char;

        ret = ftl_write(buf, blkno);
        if(ret != FTL_SUCCESS) {
             printf("write error blk = %d, code = %d", blkno, ret);
            exit(1);
        }

        for (k = 0; k < SECTORSIZE; k++) buf[k] = w_char - 1;

        ftl_read(buf, blkno);
        for (k = 0; k < SECTORSIZE; k++) {
            if (buf[k] != w_char) {
                printf("Read data wrong \n");
                exit(1);
            }
        }
        sec_data[blkno] = w_char;
		printf("%d ", blkno);
    }
    ftl_close();
	return 0;
#endif
}

