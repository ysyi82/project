int FM_Open();
int FM_Close();
int FM_Read_Data(char *buffer, unsigned int, unsigned int);
int FM_Read_Spare(char *buffer, unsigned int, unsigned int);
int FM_Read_Data_Spare(char *buffer, unsigned int, unsigned int);
int FM_Write_Data(char *buffer, unsigned int, unsigned int);
int FM_Write_Spare(char *buffer, unsigned int, unsigned int);
int FM_Write_Data_Spare(char *buffer, unsigned int, unsigned int);
int FM_Erase(unsigned int);
int FM_isBadBlock(unsigned int);

#define FM_SUCCESS  0
#define FM_BAD_DEVICE_ID    1
#define FM_OPEN_FAIL    2
#define FM_CLOSE_FAIL   3
#define FM_NO_SEMAPHORE 4
#define FM_ILLEGAL_ACCESS   5
#define FM_DEVICE_NOT_OPEN  6
#define FM_DEVICE_BUSY  7
#define FM_MEMORY_FAIL  8

/*          Set your flash memory specifications            */
#define DATASIZE    512
#define SPARESIZE   16
#define PAGESIZE (DATASIZE+SPARESIZE)

#if 0
#define PAGES_PER_BLOCK 3
#define NO_OF_BLOCKS    6        // 16 Mbytes Flash Chip
#else
#define PAGES_PER_BLOCK 3
#define NO_OF_BLOCKS    7        // 16 Mbytes Flash Chip
#endif

#define ERASE_BYTE  0xFF
#define CLEAR_BYTE  0x00

#define MAX_DATA_WRITE  2
#define MAX_SPARE_WRITE 3

