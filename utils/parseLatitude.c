#include <stdio.h>
#include <stdlib.h>

//ddmm.mmmm N/S
int main(int args,char * argv[]){
	char lat[10];
	int dd;
	float m;
	char nOs;
	sscanf(argv[1],"%02d%f %c",&dd,&m,&nOs);
	printf("degrees:\t%i\n",dd);
	printf("minutes:\t%f\n",m);
	printf("n/s:\t%c\n",nOs);
	printf("%s\n",lat);
}
