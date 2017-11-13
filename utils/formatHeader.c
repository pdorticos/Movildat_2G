#include <stdlib.h>
#include "dt2ts.h"
#include "formatHeader.h"

//usage:./dtme2TS2 17/11/10,11:21:53+04 3 7
int main(int args,char * argv[]){
	header_t head;
        dt2tsvec(argv[1],head.timeStamp);
	printf("Timestamp:\t\t%s\n",head.timeStamp);
	sprintf(head.timeStampExtension,"%02x",atoi(argv[2]));
	printf("TimestampExtension:\t%s\n",head.timeStampExtension);
	sprintf(head.priority,"%02x",atoi(argv[3]));
	printf("Priority:\t\t%s\n",head.priority);
	return 0;
}
