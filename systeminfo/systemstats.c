#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "systemstats.h"
// Value of 1024*1024
#define MB 1048576;

// Read common CLI command with popen and fill the passed string if sucessfull
int readCommonComand(char *cmd, char* string,int maxLength)
{
	FILE *fp;
	/* Open the command for reading. */
	fp = popen(cmd, "r");
	// If the FilePointer is Null then no need to resume as no data will be fetched.
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		pclose(fp);
		return -1;
	}
	fgets(string, maxLength, fp);
	/* close */
	pclose(fp);
	return 0;
}

int returnStorageSize(char **pointerchar, char *storageType)
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
	return 0;
}

int cpuTemperature(int *temp)
{
	FILE *fptr;
	fptr = fopen("/sys/class/thermal/thermal_zone3/temp", "r");
	if (fptr == NULL)
	{
		printf("Failed to read cpu temp\n");
		fclose(fptr);
		*temp = -1;
		return -1;
	}
	fscanf(fptr, "%i", temp);
	*temp = *temp / 1000;
	fclose(fptr);
	return 0;
}

//  Returns available memory of system in kb as given my meminfo.
int getSystemMemoryInformation(int *mem, int memtype)
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
		return -1;
	}
	fscanf(fp, "%i", mem);
	/* close */
	pclose(fp);
	return 0;
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
int getProcessesData(char *topLine, char *command, int maxOutputLength)
{
	char baseCmd[100] = "top -u steam -bc -n 1  | grep ";
	strncat(baseCmd, command, maxOutputLength);
	int returnCode = readCommonComand(command, topLine, maxOutputLength);
	strncat(topLine, "\n", maxOutputLength);
	return returnCode;
}

// Returns storage size based off of storageType.
// gets available or total.
// returns nothing if none of these strings are given.
int systemStorageSpace(int *storage, char *storageType)
{
	if (strcasecmp(storageType, TOTALSTORAGE) != 0 && strcasecmp(storageType, AVAILABLESTORAGE) != 0)
	{
		return -1;
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
		return -1;
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
	return 0;
}

int getSystemName(char *machineName, int maxLength)
{
	char *cmd = "hostname -s";
	return readCommonComand(cmd, machineName, maxLength);
}

int getIp(char *ip, int maxLength)
{	
	char *cmd = "hostname -I | awk '{print $1}'";
	return readCommonComand(cmd, ip, maxLength);
}

int getSystemKernelInfo(char *kernelInfo, int maxLength)
{
	char *cmd = "uname -r -m";
	return readCommonComand(cmd, kernelInfo, maxLength);
}

int getPm2Data(char *pm2Data, char *command, int maxOutputLength)
{
	FILE *fp;
	char baseCmd[150];
	snprintf(baseCmd, 150, "pm2 show %s | grep -o \"online\\|offline\"", command);
	fp = popen(baseCmd, "r");
	int a = 0;
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		pclose(fp);
		return -1;
	}
	fgets(pm2Data, maxOutputLength, fp);
	char result[maxOutputLength];
	snprintf(result, maxOutputLength, "%s: %s\n", command, pm2Data);
	strncpy(pm2Data, result, maxOutputLength);
	pclose(fp);
	return 0;
}