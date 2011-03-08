unsigned int FM_Open();
unsigned int FM_Close();
unsigned int FM_Read_Data(char *buffer, unsigned int, unsigned int);
unsigned int FM_Read_Spare(char *buffer, unsigned int, unsigned int);
unsigned int FM_Write_Data(char *buffer, unsigned int, unsigned int);
unsigned int FM_Write_Spare(char *buffer, unsigned int, unsigned int);
unsigned int FM_Erase(unsigned int);

#define FM_SUCCESS  0
#define FM_BAD_DEVICE_ID    1
#define FM_OPEN_FAIL    2
#define FM_CLOSE_FAIL   3
#define FM_NO_SEMAPHORE 4
#define FM_ILLEGAL_ACCESS   5
#define FM_DEVICE_NOT_OPEN  6
#define FM_DEVICE_BUSY  7
#define FM_MEMORY_FAIL  8

