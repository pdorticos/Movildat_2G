#include "parseCoords.h"

//ddmm.mmmmN/S

int parseLat(char *latStr,latitude_t * lat){
	sscanf(latStr,"%02d%8f%c",&lat->degrees,&lat->minutes,&lat->hemisphere);
	return 0;
}
int parseLong(char *lonStr,longitude_t * lon){
	sscanf(lonStr,"%03d%8f%c",&lon->degrees,&lon->minutes,&lon->side);
	return 0;
}



