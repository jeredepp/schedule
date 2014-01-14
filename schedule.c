/*****************************************
 * Scheduler (Exam 20140701)
 * Autor: Stefan Bakocs
 *
 ****************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHAR_LEN 255

const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const char* errorString[] = {"OK", "Warning"};
const char* shiftStrinf[] = {"F", "S", "N"};


// Definition of the Scheduler struct
typedef struct {
	int  day;
	char sCaretaker[20];
	char sQualification[20];
	int  sShift;
	int  sError;
	int nightshift;
} entry;

//External Variable for a maximum of one week
entry entryArray[63];

// Predefinitions of functions
void mainMenu();
void createPlan();
int  loadFile();
int  saveFile(int typ);
void printSchedule();
void checkSchedule();
void setError(int oneDay, int shift);
void mostNightshift();
int chooseOutputType();

int main()
{
	int output = 0;
	
	printf("SchedulePlanMaster 2000 is starting up...");
	fflush(stdout);
	sleep(1);
	system("clear");
	
	mainMenu();

	checkSchedule();

	printSchedule();

	mostNightshift();
	output = chooseOutputType();

	saveFile(output);

    system( "read -n 1 -s -p \"Press any Key...\"" );
    
	return 0;
}


void mainMenu()
{
	char choice[3] = "";
	
    system("clear");
    
	printf("Select mode: \n");

	printf("Manual\t(1) \n");
	printf("Load\t(2)\n");
	printf("Exit\t(3) \n");
	
	fgets(choice, 2, stdin);

	if ((strlen(choice)>0) && (choice[strlen (choice) - 1] == '\n'))
		choice[strlen (choice) - 1] = '\0';

    fpurge(stdin);
    
    system("clear");
    
	switch (choice[0]) {
	case '1':
		createPlan();
		break;
	case '2':
		loadFile();
		break;
	case '3':
		exit(0);
	}
	
	system("clear");
}

int loadFile()
{
	/* FileStream for the Scheduler File */
	FILE *inputFile;

	/* allocation of the buffer for every line in the File */
	char *buf = malloc(MAX_CHAR_LEN);
	char *tmp;

	/* if the space could not be allocaed, return an error */
	if (buf == NULL) {
		printf ("Memory Error\n");
		return 1;
	}

	if ( ( inputFile = fopen( "input.dat", "r" ) ) == NULL ) { //Reading a file
		printf( "File Error.\n" );
		return 1;
	}

	int i = 0;
	while (fgets(buf, 255, inputFile) != NULL) {
		if(ferror(inputFile)) {
			break;
		}
		if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
			buf[strlen (buf) - 1] = '\0';


		tmp = strtok(buf, ";");
		strcpy( entryArray[i].sCaretaker, tmp);

		tmp = strtok(NULL, ";");
		strcpy(  entryArray[i].sQualification, tmp);

		tmp = strtok(NULL, ";");
		entryArray[i].sShift = atoi(tmp);

		tmp = strtok(NULL, ";");
		entryArray[i].day = atoi(tmp);

		tmp = strtok(NULL, ";");
		entryArray[i].sError = atoi(tmp);

		i++;

	}

	free(buf);
	fclose(inputFile);
	
	system("clear");
	return 0;
}


void createPlan()
{
	int counter = 0;
	int abbruch = 0;
	
	while(abbruch != 1) {
		char tmp[MAX_CHAR_LEN];
		// Eingabe Pfleger
		printf("Employees name: \n");
		fgets(tmp, 20, stdin);

		if ((strlen(tmp)>0) && (tmp[strlen (tmp) - 1] == '\n'))
			tmp[strlen (tmp) - 1] = '\0';

		strcpy(entryArray[counter].sCaretaker, tmp);

        	fpurge(stdin);
		
		system("clear");
        
        //Eingabe Qualification
		printf("Qualification:\n");
		printf("Trainee\t\t(1)\n");
		printf("Assistant\t(2)\n");
		printf("Degree\t\t(3)\n");
		fgets(tmp, 20, stdin);

		if ((strlen(tmp)>0) && (tmp[strlen(tmp) - 1] == '\n'))
			tmp[strlen(tmp) - 1] = '\0';

		switch (tmp[0]) {
		case '1':
			strcpy(tmp, "Trainee");
			break;
		case '2':
			strcpy(tmp, "Assistant");
			break;
		case '3':
			strcpy(tmp, "Degree");
			break;
		default:
			strcpy(tmp, "Trainee");
		}

		strcpy(entryArray[counter].sQualification, tmp);

        	fpurge(stdin);
        
		system("clear");
		
		//Eingabe Shift
		printf("Shift:\n");
		printf("Early\t\t(1)\n");
		printf("Late\t\t(2)\n");
		printf("Nightshift\t(3)\n");
		fgets(tmp, 3, stdin);

		if ((strlen(tmp)>0) && (tmp[strlen (tmp) - 1] == '\n'))
			tmp[strlen (tmp) - 1] = '\0';

		switch (tmp[0]) {
		case '1':
			strcpy(tmp, "0");
			break;
		case '2':
			strcpy(tmp, "1");
			break;
		case '3':
			strcpy(tmp, "2");
			break;
		default:
			strcpy(tmp, "0");
		}

		entryArray[counter].sShift= atoi(tmp);

        	fpurge(stdin);
        
		system("clear");
        	
		// Eingabe Weekday
		printf("Weekday:\n");
		printf("Monday\t\t(1)\n");
		printf("Tuesday\t\t(2)\n");
		printf("Wednesday\t(3)\n");
		printf("Thursday\t(4)\n");
		printf("Friday\t\t(5)\n");
		printf("Saturday\t(6)\n");
		printf("Sunday\t\t(7)\n");

		fgets(tmp, 3, stdin);

		if ((strlen(tmp)>0) && (tmp[strlen (tmp) - 1] == '\n'))
			tmp[strlen (tmp) - 1] = '\0';
		
		if(tmp[0]>=1 && tmp[0]<=7){
			strcpy(tmp-1, "0");
		}else{
			strcpy(tmp, "0");
		}

		entryArray[counter].day =  atoi(tmp);

		fpurge(stdin);
        
		printf("Add another Entry? (Y/N)\n");
		fgets(tmp, 2, stdin);

		if ((strlen(tmp)>0) && (tmp[strlen (tmp) - 1] == '\n'))
			tmp[strlen (tmp) - 1] = '\0';
			
		switch(tmp[0])
		{
			case 'Y':
			case 'y':
				abbruch = 0; 
				counter++;
			break;
			case 'N': 
			case 'n':
				abbruch = 1; 
			break;
			default: 
				abbruch = 0;
				counter++;
		}

        fpurge(stdin);
        
		system("clear");
	}
}

void printSchedule()
{
	int i;

	//Print Titlerow
	printf("Employee name: \t Qualification: \t Shift: \tWeekday: \tStatus: \n");


	for (i = 0; i <= sizeof(entryArray); i++) {
		if (strlen(entryArray[i].sCaretaker) != 0)
			printf("%s\t\t%s\t%s\t%s\t%s \n", entryArray[i].sCaretaker , entryArray[i].sQualification, shiftStrinf[entryArray[i].sShift] , dayNames[entryArray[i].day], errorString[entryArray[i].sError]);
		else
			break;
	}

    system("clear");
}

void checkSchedule()
{
	printf("Checking plan. Please wait! ... \n");
	/*
	 * Here we should check if for any day and shift there must be at least one "Degree" person.
	*/
	//int size = 0;
	int i, j, k;
	int dplVorhanden = 0;

	//Check per day
	for( j = 0; j<= 6; j++) {
		//Check per shift
		for( k = 0; k<= 2; k++) {
			for (i = 0; i <= sizeof(entryArray); i++) {
				if (strlen(entryArray[i].sCaretaker) != 0) {
					if(entryArray[i].day == j && entryArray[i].sShift == k) {

						if(strcmp(entryArray[i].sQualification, "Degree") == 0) {
							dplVorhanden = 1;
						}

					}
				}

				else
					break;
			}

			if (dplVorhanden == 0) {
				setError(j, k);
			}

			dplVorhanden = 0;
		}

	}
	system("clear");
}

void setError(int oneDay, int shift)
{
	int i;

	for (i = 0; i <= sizeof(entryArray); i++) {
		if (strlen(entryArray[i].sCaretaker) != 0) {
			if(entryArray[i].day == oneDay && entryArray[i].sShift == shift) {
				entryArray[i].sError = 1;
			}
		} else
			break;
	}
}

int  saveFile(int typ)
{
	/* FileStream for the Library File */
	FILE *outFile;
	char filename[20];

	switch (typ) {
	case 1:
		strcpy( filename, "output.txt");
		break;
	case 2:
		strcpy( filename, "output.html");
		break;
	default:
		strcpy( filename, "output.dat");
	}


	if ( ( outFile = fopen(filename , "w" ) ) == NULL ) {
		printf( "File could not be opened.\n" );
		return 1;
	}

	// Wenn ein HTML File generiert wird
	if(typ == 2) {
		fprintf(outFile, "<!DOCTYPE html><html><body><h1>Shiftenplan Web</h1><table border='1'>");
		fprintf(outFile, "<tr><td>Name des Pflegepersonals:</td><td>Qualification:</td><td>Shift:</td><td>Weekday:</td><td>Warning:</td></tr>");
	}


	int i;
	for(i = 0; i < 64; i++) {
		//printf("%lu",strlen(entryArray[i].sCaretaker));
		if(strlen(entryArray[i].sCaretaker) != 0) {
			switch (typ) {
				//TXT
			case 1:
				fprintf(outFile, "%s\t\t%s\t%s\t%s\t%s\n", entryArray[i].sCaretaker , entryArray[i].sQualification, shiftStrinf[entryArray[i].sShift] , dayNames[entryArray[i].day], errorString[entryArray[i].sError]);
				break;

				//HTML
			case 2: {
				fprintf(outFile, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n", entryArray[i].sCaretaker , entryArray[i].sQualification, shiftStrinf[entryArray[i].sShift] , dayNames[entryArray[i].day], errorString[entryArray[i].sError]);
				break;
			}

			//DAT (analog TXT)
			default:
				fprintf(outFile, "%s\t\t%s\t%s\t%s\t%s\n", entryArray[i].sCaretaker , entryArray[i].sQualification, shiftStrinf[entryArray[i].sShift] , dayNames[entryArray[i].day], errorString[entryArray[i].sError]);
			}
		}

	}
	
	if(typ == 2) {
		fprintf(outFile, "</table></body></html>");
	}
	
	fclose(outFile);
	return 0;
}

void mostNightshift()
{
	char *sMost[20];
	int countMost = 0;
	
	int i,j,k,l;
	
	for (i = 0; i <= sizeof(entryArray); i++) {
		if (strlen(entryArray[i].sCaretaker) != 0)
		{
			if (entryArray[i].sShift == 2)
			{
				entryArray[i].nightshift++;
			}
		}
		else
			break;
	}
	
	for (j = 0; j <= sizeof(entryArray); j++) {
		if (strlen(entryArray[j].sCaretaker) != 0)
		{
			for(k = 0; k <= sizeof(entryArray); k++)
			{
				if (strlen(entryArray[k].sCaretaker) != 0)
				{
					if ((strcmp(entryArray[j].sCaretaker, entryArray[k].sCaretaker) == 0) && entryArray[k].nightshift > 0)
					{
						entryArray[k].nightshift++;
					}
					
				}
				else
					break;
			}
		}
		else
			break;
			
	
	}
	
	
	for (l = 0; l <= sizeof(entryArray); l++) {
		if (strlen(entryArray[l].sCaretaker) != 0)
		{
			if(countMost == 0)
			{
			*sMost = entryArray[l].sCaretaker;
			countMost = entryArray[l].nightshift;
			}
			else if(countMost < entryArray[l].nightshift)
			{
				*sMost = entryArray[l].sCaretaker;
				countMost = entryArray[l].nightshift	;
			}
			
		}
		else
			break;
	}
	countMost--;
	printf("The most Nightshifts, has %s with %i times.\n", *sMost, countMost );
	
}

int chooseOutputType()
{
	char choice[3] = "";

	// Enter the Data (Hand or File)
	printf("Please choose the output type: \n");

	printf("txt  (1) \n");
	printf("html (2)\n");
	printf("Quit (3) \n");

	printf("Auswahl: ");
	fgets(choice, 2, stdin);

	if ((strlen(choice)>0) && (choice[strlen (choice) - 1] == '\n'))
		choice[strlen (choice) - 1] = '\0';

	//fflush(stdin);
    fpurge(stdin);
    
	switch (choice[0]) {
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		exit(0);
	default:
		return 99;
	}

}
