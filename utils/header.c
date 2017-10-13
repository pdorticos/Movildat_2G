#include <time.h>
#include <stdio.h>
int main(int args,char * argv[]){
	time_t ti = time(NULL);
	printf("%04lx",ti);
}
