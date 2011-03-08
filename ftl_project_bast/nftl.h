#define FTL_SUCCESS 0
#define FTL_OPENERR 1
#define FTL_MEMORYERR   2
#define FTL_FORMATERR   3
#define FTL_TOOMANYBLOCK    4
#define FTL_MANYBADBLOCK    5
#define FTL_INVALIDSECTOR   6
#define FTL_MEDIAERR    6

int ftl_format();
int ftl_open();
int ftl_close();
int ftl_read(char *, unsigned int);
int ftl_write(char *, unsigned int);
int ftl_query_num_sectors();
