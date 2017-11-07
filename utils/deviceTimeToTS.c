#include <time.h>
#include <stdio.h>

//parse a string with a format like 17/11/07,12:10:53+04
int main(int args, char * argv[]){
	int sec,min,hour,day,month,year,tz;
	struct tm tstamp;
	sscanf(argv[1],"%d/%d/%d,%d:%d:%d+%d",&year,&month,&day,
		&hour,&min,&sec,&tz);
	printf("Day\t:%d Month\t:%d Year\t:%d\n",day,month,year);
	printf("Hour\t:%d Minute\t:%d Seccond\t:%d\ TZ\t:%d\n"
		,hour,min,sec,tz);
	tstamp.tm_sec=sec;
	tstamp.tm_min=min;
	tstamp.tm_hour=hour;
	tstamp.tm_year=1900+year;
	tstamp.tm_mon=month;
	tstamp.tm_mday=day;
	printf("Timestamp:\t %s\n",asctime(&tstamp));
	printf("timestamp:\t%04lx\n",mktime(&tstamp));
}
