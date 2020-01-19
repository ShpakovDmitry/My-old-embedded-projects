unsigned char ReadLineFromFile(FILE *f,	// чтение годной строчки из файла возвращает 1 если конец файла 
				   char line[MAX_LINE_LENGTH]);	
unsigned char GetPlanetEphemerisFromLine(char *line,	// выделить параметры эфемерид планеты из строки
					 planet *body);	// возвращает 1 если строка не годна, иначе 0
unsigned char GetCometEphemerisFromLine(char *line,	// выделить параметры эфемерид кометы из строки
					comet *body);	// возвращает 1 если строка не годна, иначе 0

unsigned char ReadLineFromFile(FILE *f, char line[MAX_LINE_LENGTH])
{
	char c;
	char str[MAX_LINE_LENGTH];
	unsigned int i;

	i = 0;
	do
	{
		str[i] = getc(f);
		i++;
	}while( (i < (MAX_LINE_LENGTH - 1)) && (str[i - 1] != '\n') && (str[i - 1] != EOF) && (str[i - 1] != '#'));

	if(str[i - 1] == '#')
	{
		do
		{
			c = getc(f);
		}while( (i < (MAX_LINE_LENGTH - 1)) && (c != '\n') && (c != EOF) );
	}

	if((str[i - 1] == EOF) && (i <= 1) )
	{
		str[i - 1] = '\0';
		return 1;
	}
	else
	{
		str[i - 1] = '\0';
		strcpy(line, str);
		return 0;
	}
}

unsigned char GetPlanetEphemerisFromLine(char *line, planet *body)
{
	int get_res;
	
	//15 parameters
	get_res = sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", \
		(*body).SemiMajorAxis.value,		\
		(*body).Eccentricity.value,		\
		(*body).Inclination.value,		\
		(*body).LongitudeOfPericenter.value,	\
		(*body).AscendingNode.value,		\
		(*body).MeanLongitude.value,		\
		(*body).SemiMajorAxis.change,		\
		(*body).Eccentricity.change,		\
		(*body).Inclination.change,		\
		(*body).LongitudeOfPericenter.change,	\
		(*body).AscendingNode.change,		\
		(*body).MeanLongitude.change,		\
		(*body).N_r.value,			\
		(*body).Epoch.value,			\
		(*body).Mass.value			\
		);
	if(get_res != 15)
	{
		return 1;
	}

	return 0;
}

unsigned char GetCometEphemerisFromLine(char *line, comet *body)
{
	char *str;
	unsigned char i = 0;
	
	return 0;

}

