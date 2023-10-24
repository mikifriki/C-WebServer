#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "systemstats.h"
// Value of 1024*1024
#define MB 1048576;

static void returnStorageSize(char **pointerchar, char *storageType)
{
	int readLine = 0;
	while (*pointerchar != NULL)
	{
		(readLine)++;
		// If the I value is 1 then we are reading total section.
		if (strncasecmp(storageType, TOTALSTORAGE, strlen(TOTALSTORAGE)) == 0 && readLine == 1)
		{
			*pointerchar = strtok(NULL, " ");
			break;
		}
		// If the I value is 1 then we are reading available section.
		if (strncasecmp(storageType, AVAILABLESTORAGE, strlen(AVAILABLESTORAGE)) == 0 && readLine == 3)
		{
			*pointerchar = strtok(NULL, " ");
			break;
		}
		*pointerchar = strtok(NULL, " ");
	}
}

void cpuTemperature(int *temp)
{
	FILE *fptr;
	fptr = fopen("/sys/class/thermal/thermal_zone3/temp", "r");
	if (fptr == NULL)
	{
		printf("Failed to read cpu temp\n");
		fclose(fptr);
		return;
	}
	fscanf(fptr, "%i", temp);
	*temp = *temp / 1000;
	fclose(fptr);
}

//  Returns available memory of system in kb as given my meminfo.
void getSystemMemoryInformation(int *mem, int memtype)
{
	FILE *fp;
	char *cmd = "cat /proc/meminfo | grep -i 'memtotal' | grep -o '[[:digit:]]*'";
	if (memtype == 1)
	{
		cmd = "cat /proc/meminfo | grep -i 'memavailable' | grep -o '[[:digit:]]*'";
	}
	/* Open the command for reading. */
	fp = popen(cmd, "r");
	// If the FilePointer is Null then no need to resume as no data will be fetched.
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		pclose(fp);
		return;
	}
	fscanf(fp, "%i", mem);
	/* close */
	pclose(fp);
}

// Reads the current total running processes on the machine
// Needs to be implemented
// void getTotalProcesses()
// {
// 	FILE *fp;
// 	char *cmd = "top -b -n 1";
// 	// char *cmd = "top -l 1";
// 	fp = popen(cmd, "r");
// 	int a = 0;
// 	if (fp == NULL)
// 	{
// 		printf("Failed to run command\n");
// 		pclose(fp);
// 		return;
// 	}
// 	char string[150];
// 	char *pointerchar;
// 	while (fgets(string, 150, fp))
// 	{
// 		pointerchar = strtok(string, " \t");
// 		while (pointerchar != NULL)
// 		{
// 			if (strcasecmp(pointerchar, "Processes:") == 0 || strcasecmp(pointerchar, "Tasks:") == 0)
// 			{
// 				// As we know we want the next value then just print it.
// 				pointerchar = strtok(NULL, " \t");
// 				printf("%s", pointerchar);
// 				return;
// 			}
// 			// Read rest of the tokeized string.
// 			pointerchar = strtok(NULL, " \t");
// 		}
// 	}
// 	pclose(fp);
// }

// Returns the top line of the given command.
// this data includes cpu and ram usage. It also incldues uptime of the proccess.
void getProcessesData(char *topLine, char *command, int maxOutputLength)
{
	FILE *fp;
	char baseCmd[100] = "top -u steam -bc -n 1  | grep ";
	strcat(baseCmd, command);
	// char *cmd = "top -l 1";
	fp = popen(baseCmd, "r");
	int a = 0;
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		pclose(fp);
		return;
	}
	fgets(topLine, maxOutputLength, fp);
	strcat(topLine, "\n");
	pclose(fp);
}

// Returns storage size based off of storageType.
// gets available or total.
// returns nothing if none of these strings are given.
void systemStorageSpace(int *storage, char *storageType)
{
	if (strcasecmp(storageType, TOTALSTORAGE) != 0 && strcasecmp(storageType, AVAILABLESTORAGE) != 0)
	{
		return;
	}
	FILE *fp;
	char *cmd = "df -H /";
	/* Open the command for reading. */
	fp = popen(cmd, "r");
	// If the FilePointer is Null then no need to resume as no data will be fetched.
	if (fp == NULL)
	{
		printf("Failed to read disk space command\n");
		fclose(fp);
		return;
	}

	char tokenizedString[150];
	char *pointerchar;
	int i = 0;
	while (fgets(tokenizedString, 150, fp))
	{
		// Skip the initial row as we dont need to read it.
		i++;
		if (i == 1)
		{
			continue;
		}
		pointerchar = strtok(tokenizedString, " ");
		returnStorageSize(&pointerchar, storageType);
		break;
	}
	// Remove last character from string
	pointerchar[strlen(pointerchar) - 1] = '\0';
	*storage = atoi(pointerchar);
	/* close */
	pclose(fp);
}

void getSystemName(char *machineName, int maxLength)
{
	FILE *fp;
	char *cmd = "hostname -s";
	/* Open the command for reading. */
	fp = popen(cmd, "r");
	// If the FilePointer is Null then no need to resume as no data will be fetched.
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		pclose(fp);
		return;
	}
	fgets(machineName, maxLength, fp);
	/* close */
	pclose(fp);
}
