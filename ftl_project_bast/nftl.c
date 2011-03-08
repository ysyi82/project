#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fm_def.h"
#include "nftl.h"

#define FALSE   0
#define TRUE    1

#define INVAILD  0
#define VAILD    1

#define FTL_SIGNATURE   0x83404945

#define PAGE_WRITING    0x7FFF
#define PAGE_VALID      0x3FFF
#define PAGE_DELETED    0x1FFF

//#define UTILIZATION_DATA_RATE    5/10
//#define UTILIZATION_LOG_RATE	 (1 - UTILIZATION_DATA_RATE)

//#define UTILIZATION_DATA_RATE    59/100
//#define UTILIZATION_LOG_RATE	 34/100

#define UTILIZATION_DATA_RATE    5/10
#define UTILIZATION_LOG_RATE	 4/10

#if 0
#define NO_OF_SECTORS		(((NO_OF_BLOCKS) * PAGES_PER_BLOCK) * UTILIZATION_DATA_RATE)
#define NO_OF_DATA_BLOCKS	(NO_OF_BLOCKS * UTILIZATION_DATA_RATE)
#define NO_OF_LOG_BLOCKS	(NO_OF_BLOCKS * UTILIZATION_LOG_RATE)
#else
#define NO_OF_SECTORS		9
#define NO_OF_DATA_BLOCKS	3
#define NO_OF_LOG_BLOCKS	3
#endif
#define SIZE_OF_MAP			(NO_OF_BLOCKS * PAGES_PER_BLOCK)

#if 0
#define NO_OF_DATA_SECTORS	  (((NO_OF_BLOCKS-1) * PAGES_PER_BLOCK) * UTILIZATION_DATA_RATE)
#define NO_OF_LOG_SECTORS	  (((NO_OF_BLOCKS-1) * PAGES_PER_BLOCK) * UTILIZATION_LOG_RATE)
#define NO_OF_SECTORS		  NO_OF_DATA_SECTORS + NO_OF_LOG_SECTORS
#endif

typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct {
    ushort  Flag;   // 2bytes
    ushort  dummy;  // 2bytes
    int LogicalSectorNo;        /* 4 byte */
    int dummy2;     /* 4 byte */
    char    ecc[4];         /* 4 byte */
} SPARE_T;      /* Total 16 byte */

typedef struct {
    unsigned short  Flag;
    int		NumBlock;
    uint    NumSector;
    unsigned int    Signature;
} MASTERHEADER_T;

typedef struct
{
	uint PBN;
	uint LBN;
	int  USED;
	char pIndex[PAGES_PER_BLOCK];
} LOG_BLOCK;

MASTERHEADER_T  master_header;
uint l2p_block_map[NO_OF_DATA_BLOCKS];
LOG_BLOCK	 log_map[NO_OF_LOG_BLOCKS];
char		 glob_buf[PAGESIZE];

/* 나중에는 큐에 넣고 관리 하자 */
//char    free_log_blocks[NO_OF_LOG_BLOCKS];
char	free_pages[NO_OF_BLOCKS];
uint	vid_map[SIZE_OF_MAP];
int     invaild_pages[NO_OF_LOG_BLOCKS];
int     num_free_blocks;

uint find_free_logblock();
uint getAllocatedLogMapByPBN(uint iPBN);
uint getAllocatedLogMap(uint iLBN);

int ftl_format()
{
    int i, j;
    MASTERHEADER_T  master_header;

    if (FM_Open() != FM_SUCCESS) 
	{
        return(FTL_MEDIAERR);
    }

    for (i = 0; i < NO_OF_BLOCKS; i++) 
	{
        FM_Erase(i);
    }

    for (i = 0; i < NO_OF_DATA_BLOCKS + 1; i++) 
	{
		//+1은 마스터 블럭에 대한 보정
        l2p_block_map[i] = i + 1;
    }

    free_pages[0] = (char) 0; //마스터 블럭
	//printf("%d\n", NO_OF_DATA_BLOCKS);
    for (i = 1; i < NO_OF_DATA_BLOCKS + 1; i++) 
	{
        free_pages[i] = (char) 0;
    }
	//printf("NO_OF_LOG_BLOCKS[%d]\n", NO_OF_LOG_BLOCKS);
	j = i;
	for( ; i < NO_OF_BLOCKS; i++) 
	{
        free_pages[i] = (char) PAGES_PER_BLOCK;
    }

	for( i = 0; i < NO_OF_LOG_BLOCKS; i++, j++)
	{
		log_map[i].PBN  = j;
		log_map[i].LBN  = 0xFF;
		log_map[i].USED = -1;
		memset( log_map[i].pIndex, 0xFF, PAGES_PER_BLOCK * sizeof(char) );
	}
	
	memset( invaild_pages, 0, PAGES_PER_BLOCK * sizeof(int) );
#if 0
	//memset( vid_map, ~0, SIZE_OF_MAP * sizeof(uint) );
#else
	for( i = 0; i < (NO_OF_DATA_BLOCKS+1) * PAGES_PER_BLOCK; i++)
	{
		vid_map[i] = VAILD;
	}

	for(j = 0; j < NO_OF_LOG_BLOCKS * PAGES_PER_BLOCK; j++, i++)
	{
		vid_map[i] = INVAILD;
	}
#endif
    num_free_blocks = NO_OF_LOG_BLOCKS;

    master_header.Flag = PAGE_VALID;
    master_header.NumBlock = NO_OF_BLOCKS;
    master_header.NumSector = NO_OF_SECTORS;
    master_header.Signature = FTL_SIGNATURE;

    memset(glob_buf, ~0, PAGESIZE);
    memcpy(glob_buf, (char *) &master_header, sizeof(MASTERHEADER_T));

    FM_Write_Data((char *) glob_buf, 0, 0);

    FM_Close();
    return(FTL_SUCCESS);
}

int ftl_query_num_sectors()
{
    return(NO_OF_SECTORS);
}

int ftl_open()
{
    if (FM_Open() != FM_SUCCESS) {
        return(FTL_MEDIAERR);
    }

    FM_Read_Data((char *) glob_buf, 0, 0);

    memcpy((char *) &master_header, glob_buf, sizeof(MASTERHEADER_T));

    if (master_header.Signature != FTL_SIGNATURE) {
        return (FTL_FORMATERR);
    }

    return(FTL_SUCCESS);
}

int ftl_close()
{
    return(FTL_SUCCESS);
}

int ftl_read(char *buf, unsigned int LogSector)
{
	uint iPPN;
	if (LogSector >= NO_OF_SECTORS) 
	{
		return(FTL_INVALIDSECTOR);
	}

	iPPN = (l2p_block_map[LogSector / PAGES_PER_BLOCK] * PAGES_PER_BLOCK) + (LogSector % PAGES_PER_BLOCK);
	/*
	if( vid_map[iPPN] == ~0)
	{
		memset(buf, ~0, DATASIZE);
	}
	else*/
	{
		uint idx;
		if( vid_map[ iPPN ] == VAILD )
		{
			FM_Read_Data(buf, l2p_block_map[LogSector / PAGES_PER_BLOCK], LogSector % PAGES_PER_BLOCK);
		} else
		{
			uint iLogBlock = getAllocatedLogMap( LogSector / PAGES_PER_BLOCK );

			// pIndex에서 유효한 논리 주소를 찾느다.
			for( idx = 0; idx < PAGES_PER_BLOCK; idx++ )
			{
				uint nCurPPN = ( (log_map[ iLogBlock ].PBN) * PAGES_PER_BLOCK) + idx;
				if( vid_map[ nCurPPN ] == VAILD )
				{
					if( log_map[iLogBlock].pIndex[idx] == LogSector )
					{
						//Read data from Log Block
						//이 부분 고치자
						FM_Read_Data(buf, log_map[iLogBlock].PBN, idx);
						break;
					}
				}
			}
		}
	}

	return(FTL_SUCCESS);
}

uint getVictimBlock()
{
	uint i;
	uint minIndex = 0;
	int min = invaild_pages[minIndex];
	
	for( i = 1; i < NO_OF_LOG_BLOCKS; i++ )
	{
		if( min < invaild_pages[i] )
		{
			minIndex = i;
			min = invaild_pages[i];
		}
	}

	return minIndex;
}
SPARE_T Spare = {PAGE_VALID, ~0, ~0, ~0, '\377', '\377', '\377', '\377'};

int do_merge()
{
	if( 0 )/*로그 블럭에 업데이트 기록이 있는지 확인*/
	{
		//Switch Merge

	} else
	{
		//Full Merge
		uint i, j;
		uint emptyPBN = find_free_logblock();
		uint iVictim  = getVictimBlock();
		uint dataPBN = l2p_block_map[ log_map[iVictim].LBN ];
		uint write_block, write_page;
		uint iEmptyLogmap;

		for( i = 0; i < PAGES_PER_BLOCK; i++ )
		{
			// 찾을 논리 주소를 구한다.
			uint nCurLPN = ((log_map[iVictim].LBN) * PAGES_PER_BLOCK) + i;
			uint nCurPPN = (dataPBN * PAGES_PER_BLOCK) + i;
			if( vid_map[nCurPPN] == VAILD || vid_map[nCurPPN] == ~0)
			{
				//Copy to Empty Log Block
				write_block = emptyPBN;
				write_page  = i;
				//맵정리
				vid_map[nCurPPN] = INVAILD;
				vid_map[write_block*PAGES_PER_BLOCK + write_page] = VAILD;
								
			} else
			{
				// pIndex에서 유효한 논리 주소를 찾느다.
				for( j = 0; j < PAGES_PER_BLOCK; j++ )
				{
					nCurPPN = ( (log_map[ iVictim ].PBN) * PAGES_PER_BLOCK) + j;
					if( vid_map[ nCurPPN ] == VAILD )
					{
						if( log_map[iVictim].pIndex[j] == nCurLPN )
						{
							//Copy to Empty Log Block
							write_block = emptyPBN;
							write_page  = i;
							//맵정리
							vid_map[ nCurPPN ] = INVAILD;
							vid_map[write_block*PAGES_PER_BLOCK + write_page] = VAILD;
							break;
						}
					}
				}
			}
			//freePages
			free_pages[write_block]--;
			//스페어 영역 읽기 & 기록
			FM_Read_Spare(  (char *)&glob_buf, nCurPPN / PAGES_PER_BLOCK, nCurPPN % PAGES_PER_BLOCK );
			FM_Write_Spare( (char *)&glob_buf, write_block, write_page );

			//데이터 영역 읽기 & 기록
			FM_Read_Data( (char *)&glob_buf, nCurPPN / PAGES_PER_BLOCK, nCurPPN % PAGES_PER_BLOCK );
			FM_Write_Data((char *)&glob_buf, write_block, write_page );
		}
		//데이터 블럭과 로그 블럭 교환
		l2p_block_map[ log_map[iVictim].LBN ] = emptyPBN;
		iEmptyLogmap = getAllocatedLogMapByPBN(emptyPBN);

		//로그맵 초기화
		log_map[iEmptyLogmap].PBN  = dataPBN;
		log_map[iEmptyLogmap].LBN  = 0xFF;
		log_map[iEmptyLogmap].USED = -1;
		memset( log_map[iEmptyLogmap].pIndex, 0xFF, PAGES_PER_BLOCK * sizeof(char) );

		//log_map[iVictim].PBN  = log_map[iVictim].PBN;
		log_map[iVictim].LBN  = 0xFF;
		log_map[iVictim].USED = -1;
		memset( log_map[iVictim].pIndex, 0xFF, PAGES_PER_BLOCK * sizeof(char) );

		//free_pages
		free_pages[dataPBN] = PAGES_PER_BLOCK;
		free_pages[log_map[iVictim].PBN] = PAGES_PER_BLOCK;

		//invaild_pages
		invaild_pages[iEmptyLogmap] = 0;
		invaild_pages[iVictim] = 0;

		//Erase
		FM_Erase( dataPBN );
		FM_Erase( log_map[iVictim].PBN );

		num_free_blocks++;
	}
	return 0;
}

uint getAllocatedLogMap(uint iLBN)
{
	uint i = 0;
	while( i < NO_OF_LOG_BLOCKS )
	{
		if( log_map[i].LBN == iLBN )
			break;
		i++;
	}
	return (i < NO_OF_LOG_BLOCKS) ? i : -1;
}

uint getAllocatedLogMapByPBN(uint iPBN)
{
	uint i = 0;
	while( i < NO_OF_LOG_BLOCKS )
	{
		if( log_map[i].PBN == iPBN )
			break;
		i++;
	}
	return (i < NO_OF_LOG_BLOCKS) ? i : -1;
}

int getEmptyLogMap()
{
	uint i = 0;
	while( i < NO_OF_LOG_BLOCKS )
	{
		if( log_map[i].USED == -1 )
			break;
		i++;
	}
	return (i < NO_OF_LOG_BLOCKS) ? i : -1;
}

uint getLastLPN(uint iLogMap, uint LogSector)
{
	uint pageIndex = 0;
	while( pageIndex < PAGES_PER_BLOCK )
	{
		uint iPPN = (log_map[iLogMap].PBN * PAGES_PER_BLOCK) + pageIndex;
		if( (log_map[iLogMap].pIndex[pageIndex] == LogSector) && (vid_map[iPPN] == VAILD) )
			break;
		pageIndex++;
	}
	return (pageIndex < PAGES_PER_BLOCK) ? pageIndex : -1;
}

uint write_LogIndex(char *buf, uint iLogMap, uint LogSector ) 
{
	//pIndex에서 마지막 논리주소가 있는지 찾는다.
	//if( 기존 논리 주소이 있다면 )
	/* if문에서 만약 참이라면 IPPN을 리턴하여 후에 고치도록 바꾸자*/
	uint iPPN;
	uint pageIndex = getLastLPN(iLogMap, LogSector);
	if( pageIndex != -1 )
	{
		iPPN = (log_map[iLogMap].PBN * PAGES_PER_BLOCK) + pageIndex;
		vid_map[ iPPN ] = INVAILD;
		invaild_pages[iLogMap]++;
	}

	log_map[iLogMap].USED++;
	log_map[iLogMap].pIndex[ log_map[iLogMap].USED ] = LogSector;
	iPPN = (log_map[iLogMap].PBN * PAGES_PER_BLOCK) + log_map[iLogMap].USED;
	vid_map[ iPPN ] = VAILD;
	free_pages[ log_map[iLogMap].PBN ]--;

	//스페어 영역 기록
	Spare.Flag = PAGE_VALID;
	Spare.LogicalSectorNo = LogSector;
	FM_Write_Spare( (char *)&Spare, iPPN / PAGES_PER_BLOCK, iPPN % PAGES_PER_BLOCK );

	//데이터 영역 기록
	FM_Write_Data( buf, iPPN / PAGES_PER_BLOCK, iPPN % PAGES_PER_BLOCK );

	return log_map[iLogMap].USED;
}

int write_Logmap( char *buf, uint LogSector )
{
	//빈 로그 블럭찾기
	uint iPBN, iLBN, iLogMap;
	iPBN = find_free_logblock();
	iLBN = LogSector / PAGES_PER_BLOCK;
	if( iPBN == -1 ) return -1;

	//로그 맵 기록
	iLogMap = getEmptyLogMap();
	log_map[iLogMap].PBN = iPBN;
	log_map[iLogMap].LBN = iLBN;

	//마지막 페이지에 논리주소를 쓴다.
	write_LogIndex(buf, iLogMap, LogSector);
	num_free_blocks--;

	return iLogMap;
}

int ftl_write(char *buf, unsigned int LogSector)
{
	uint iLBN;
	uint iLogMap;
	uint iPPN;
    if( LogSector >= NO_OF_SECTORS )
        return FTL_INVALIDSECTOR;
	
	iLBN	= LogSector / PAGES_PER_BLOCK;
	iLogMap = getAllocatedLogMap( iLBN );

	//로그 블럭 할당 여부확인
	if( iLogMap != -1 )
	{
		//로그 블럭의 pIndex가 사용가능한지 확인
		if( log_map[iLogMap].USED < PAGES_PER_BLOCK - 1 )
		{
			write_LogIndex(buf, iLogMap, LogSector);
		} else
		{
			do_merge();
			write_Logmap(buf, LogSector);
		}

	} else //빈 로그 블럭이 할당되어 있지 않다면
	{
		//빈 로그 블럭이 한개 인지 확인
		if( num_free_blocks == 1 )
		{
			do_merge();
		}
		write_Logmap(buf, LogSector);
	}
	//write_LogIndex일떄 리턴값을 받는걸로 수정하자
	iPPN = (l2p_block_map[iLBN] * PAGES_PER_BLOCK) + (LogSector % PAGES_PER_BLOCK);
	vid_map[iPPN] = INVAILD;

    return(FTL_SUCCESS);
}

int do_reclaim(uint LogSector)
{
	return 0;
}

uint find_free_logblock()
{
	uint i = 0;
	while( i < NO_OF_LOG_BLOCKS )
	{
		if( log_map[i].LBN == 0xFF )
			break;
		i++;
	}
	return (i < NO_OF_LOG_BLOCKS) ? log_map[i].PBN : -1;
}
