#include <stdlib.h>
#include "dt2ts.h"
#include "formatHeader.h"

//usage:./dtme2TS2 17/11/10,11:21:53+04 3 7
int main(int args,char * argv[]){
	header_t head;
        dt2tsvec(argv[1],head.timeStamp);
	printf("Timestamp:\t\t%s\n",head.timeStamp);
	head.timeStampExtension=(uint8_t) atoi(argv[2]);
	printf("TimestampExtension:\t%u\n",head.timeStampExtension);
	head.priority=(uint8_t) atoi(argv[3]);
	printf("Priority:\t\t%u\n",head.priority);
	return 0;
}
