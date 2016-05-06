#ifndef __LIB_H__
#define __LIB_H__
#ifdef __cplusplus 
extern "C" {
#endif
#define TYPE_USERNAME 1
#define TYPE_GROUPNAME 2
#define FILE_PASSWD "/etc/passwd"
#define FILE_GROUP  "/etc/group"

int file_read_line(int fd,char *buf/*output*/,int size) ;

int parse_line(char *line,char **output,int size);

int get_name(int type,int id,char *output ,int size);

int format_access(mode_t mode,char *buf /*output*/,int count);

int format_access_as_number(mode_t mode,char *buf /*output*/,int count);

int format_time(time_t *timep,char *buf /*output*/,int count);

#ifdef __cplusplus
}
#endif
#endif
