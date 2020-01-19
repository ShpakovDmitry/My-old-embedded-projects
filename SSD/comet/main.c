#ifndef _STDIO_H_
 #include <stdio.h>
 #define _STDIO_H_
#endif

#ifndef _MATH_H_
 #include <math.h>
 #define _MATH_H_
#endif

#include "MySSD.c"

char *planetEphemerisFile = "./PlanetEphemeris.dat";
char *cometEphemerisFile = "./CometEphemeris.dat";

int main(void)
{
	printf("Here ");
	
	FILE *planetEphemeris = fopen(planetEphemerisFile, "r");
	if(planetEphemeris == NULL)
	{
		printf("Error. Can not open file \"%s\"\n", planetEphemerisFile);
		return 1;
	}

	FILE *cometEphemeris = fopen(cometEphemerisFile, "r");
	if(cometEphemeris == NULL)
	{
		printf("Error. Can not open file \"%s\"\n", cometEphemerisFile);
		fclose(planetEphemeris);
		return 1;
	}

	char line[MAX_LINE_LENGTH];
	planet SolSystemPlanets[9];
	comet SolSystemComets[1];
	unsigned char read_res, get_res;
	unsigned int i = 0;

	do
	{
		read_res = ReadLineFromFile(planetEphemeris, line);
		//get_res = GetPlanetEphemerisFromLine(line, &(SolSystemPlanets[i]));
		if(get_res == 0)
			i++;
	}while(read_res != 1);
	printf("%d %s\n", i, line);

	get_res = GetPlanetEphemerisFromLine(line, &(SolSystemPlanets[i]));
	printf("%d\n", get_res);

/*
	i = 0;
	do
	{
		read_res = ReadLineFromFile(cometEphemeris, line);
		get_res = GetCometEphemerisFromLine(line, &SolSystemComets[i]);
		if(get_res != 1)
			i++;
	}while(read_res != 1);

	printf("%d\n", i);
*/
	fclose(planetEphemeris);
	fclose(cometEphemeris);

	return 0;



}

