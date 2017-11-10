#include <time.h>
#include <stdio.h>


int dt2ts(char * devDate){
	int sec,min,hour,day,month,year,tz;
	struct tm tstamp;
	sscanf(devDate,"%d/%d/%d,%d:%d:%d+%d",&year,&month,&day,
		&hour,&min,&sec,&tz);
	printf("Day\t:%d Month\t:%d Year\t:%d\n",day,month,year);
	printf("Hour\t:%d Minute\t:%d Seccond\t:%d TZ\t:%d\n"
		,hour,min,sec,tz);
	tstamp.tm_sec=sec;
	tstamp.tm_min=min;
	tstamp.tm_hour=hour;
	tstamp.tm_year=100+year;
	tstamp.tm_mon=month-1;
	tstamp.tm_mday=day;
//	printf("timestamp arg:\t%04lx\n",mktime(&tstamp));
	time_t aux=mktime(&tstamp);
	printf("timestamp arg:\t%04lx\t",aux);
	printf("ctime arg:\t%s\n",ctime(&aux));
	time_t ti = time(NULL);
	printf("timestamp loc:\t%04lx\t",ti);
	printf("ctime loc:\t%s\n",ctime(&ti));
	return 0;
}

//usage: ./dt2ts 17/11/07,12:10:53+04
int main(int args,char * argv[]){
	return dt2ts(argv[1]);
}

