#include <stdio.h>
#include <stdlib.h>
#include "scalar.h"
//-----------------------------------------------------------------------------
HiScore::HiScore()
{
	char DefaultNames[10][15] = {		// create default hiscore
		"MILAN", 		"TANJA",		"DULIO", 		"DJORDJE",		"CLAUS",
		"ENZO", 		"JOLAN",		"JAAP",			"MARTIN", 		"RICHARD"	};
	for (int i=0; i<10; i++)
	{
		points[i] = 100 - 10 * i;
		swaps[i] = 50 + i*10;
		sprintf(names[i], "%s", DefaultNames[i]);
	}

	// load hiscore from file (if any)
	// format: NAME#POINTS#SWAPS#
	FILE *fp = fopen("data/hiscore.dat", "r");
	if (fp)
	{
		LogFile("Reading data/hiscore.dat\n");
		char buff[80];
		int number = 0;
		while (!feof(fp) && number < 10)
		{
			fgets(buff, 80, fp);
			int i = 0, last;

			// name
			while (buff[i] != '#' && i < 9)
			{
				names[number][i] = buff[i];
				i++;
			}
			names[number][i] = '\0';

			i++;
			last = i;
			while (buff[i] != '#' && i < 40)
				i++;
			if (i >= 40)
				break;
			buff[i] = '\0';
			points[number] = atoi(buff+last);
			i++;
			last = i;
			while (buff[i] != '#' && i < 40)
				i++;
			if (i >= 40)
				break;
			buff[i] = '\0';
			swaps[number] = atoi(buff+last);

			number++;
		}
		fclose(fp);
	}
}
//-----------------------------------------------------------------------------
//! write to file
HiScore::~HiScore()
{
	// format: NAME#POINTS#SWAPS#
	FILE *fp = fopen("data/hiscore.dat", "w+");
	if (fp)
	{
		for (int i=0; i<10; i++)
			fprintf(fp, "%s#%d#%d#\n", names[i], points[i], swaps[i]);
		fclose(fp);
	}
 	else
 		printf("Unable to write global config file!\n");
}
//-----------------------------------------------------------------------------
char *HiScore::enterHiscore(int pts, int swp)
{
	int ix = 9;
	while (ix > 0 && (pts > points[ix-1] || pts == points[ix-1] && swp <= swaps[ix-1]))
	{
		points[ix] = points[ix-1];
		swaps[ix] = swaps[ix-1];
		int i=0;
		for (; names[ix-1][i]; i++)
			names[ix][i] = names[ix-1][i];
		names[ix][i] = '\0';
		ix--;
	}

	points[ix] = pts;
	swaps[ix] = swp;
	return names[ix];
}
//-----------------------------------------------------------------------------
