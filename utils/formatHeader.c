#include <stdlib.h>
#include "dt2ts.h"
#include "formatHeader.h"
#include "parseCoords.h"

//usage:./dtme2TS2 17/11/10,11:21:53+04 3 7 4542.8106N
//		   YY/MM/DD,HH:MM:SS+ZZ tsXtension prio latitude
int main(int args,char * argv[]){
	header_t head;
        dt2tsvec(argv[1],head.timeStamp);
	longitude_t longitude;
	latitude_t latitude;
	printf("Timestamp:\t\t%s\n",head.timeStamp);
	sprintf(head.timeStampExtension,"%02x",atoi(argv[2]));
	printf("TimestampExtension:\t%s\n",head.timeStampExtension);
	sprintf(head.priority,"%02x",atoi(argv[3]));
	printf("Priority:\t\t%s\n",head.priority);
	parseLong(argv[4],&longitude);
	printf("Longitude:\t");
	printf("degrees:\t%i\t",longitude.degrees);
	printf("minutes:\t%f\t",longitude.minutes);
	printf("side:\t\t%c\n",longitude.side);
	parseLat(argv[5],&latitude);
	printf("Latitude:\t");
	printf("degrees:\t%i\t",latitude.degrees);
	printf("minutes:\t%f\t",latitude.minutes);
	printf("hemisphere:\t\t%c\n",latitude.hemisphere);
	return 0;
}
