#ifndef __FCGI_H__
#define __FCGI_H__

#define FCGI_VERSION_1             1
// Packet types
#define FCGI_BEGIN_REQUEST         1
#define FCGI_ABORT_REQUEST         2
#define FCGI_END_REQUEST           3
#define FCGI_PARAMS                4
#define FCGI_STDIN                 5
#define FCGI_STDOUT                6
#define FCGI_STDERR                7
#define FCGI_DATA                  8
#define FCGI_GET_VALUES            9
#define FCGI_GET_VALUES_RESULT     10
#define FCGI_UNKNOWN_TYPE          11
#define FCGI_RESPONDER             1
#define FCGI_AUTHORIZER            2
#define FCGI_FILTER                3
// Response codes
#define FCGI_REQUEST_COMPLETE      0
#define FCGI_CANT_MPX_CONN         1
#define FCGI_OVERLOADED            2
#define FCGI_UNKNOWN_ROLE          3
// Number of bytes used in FastCGI header packet
#define FCGI_HEADER_LEN            8

typedef struct _fcgi_header {
	unsigned char version;
	unsigned char type;
	unsigned char requestIdB1;
	unsigned char requestIdB0;
	unsigned char contentLengthB1;
	unsigned char contentLengthB0;
	unsigned char paddingLength;
	unsigned char reserved;
} fcgi_header;

typedef struct _fcgi_params{
	char key[64];
	char value[512];
}fcgi_params;

typedef enum requestMethod
{
	GET,
	POST
}RequestMethod;
int buildPacket(char type, char *content /*in*/, int content_len, unsigned short requestId, char *retval /*out*/, int *retval_len/*out*/);

int buildNvpair(char *name /*in*/, char *value /*in*/, char *retval /*out*/, int *retval_len /*out*/);

int handle_php(char *host, int port, RequestMethod method, char *path, char *queryString, char *postData, char *output, int *output_len);

#endif