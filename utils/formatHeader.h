#include <stdint.h>

typedef struct header{
	char timeStamp[8];
	char timeStampExtension[2];
	char priority[2];
	uint16_t longitude[4];
	uint16_t latitude[4];
	uint16_t altitude[2];
	uint16_t angle[2];
	uint16_t satelites;
	uint16_t speed[2];
	uint16_t hdop;
	uint16_t eventID;
} header_t;
