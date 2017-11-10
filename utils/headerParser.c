#include <stdio.h>
#include <string.h>
#include <time.h>

int chr2HX(char * chrStr,char * hexStr,int len){
    int i;
    if(chrStr[len-1]=='\n')
        chrStr[--len] = '\0';

    for(i = 0; i<len; i++){
        sprintf(hexStr+i*2, "%02X", chrStr[i]);
    }
    hexStr[len]='\0';
    return 0;
}

int copyArray(char * orig, char * cpy, int ini,int fin){
	int i;
	for(i=ini;i<=fin;i++){
		cpy[i]=orig[i];
	}
	cpy[fin+1]='\0';
	return 0;
}
int printLinuxLocalTimeStamp(){
	time_t ti = time(NULL);
	printf("timestamp:\t%04lx\n",ti);
	return 0;
}

int main(int args,char * argv[]){
	char hexStr[9];
	char chrStr[4];
	printf("Timestamp:\t");
	copyArray(argv[1],chrStr,0,4);
	chr2HX(chrStr,hexStr,4);
	printf("%s\n",hexStr);
	
	printf("Next field..\n");
}
