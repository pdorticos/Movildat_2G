#include <time.h>
#include <stdio.h>
int main(int args,char * argv[]){
	time_t ti = time(NULL);
	printf("timestamp:\t%04lx\n",ti);
}
