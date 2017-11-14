#include <stdio.h>
#include <stdlib.h>


typedef struct latitude{
	char hemisphere;
	int degrees;
	float minutes;
} latitude_t;

typedef struct longitude{
	char side;
	int degrees;
	float minutes;
} longitude_t;


int parseLat(char *latStr,latitude_t * lat);
int parseLong(char *lonStr,longitude_t * lon);
