#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Value of 1024*1024
#define MB 1048576;

static void returnStorageSize(char **pointerchar, char **storageType, int *i)
{
	while (*pointerchar != NULL)
	{
		(*i)++;
		// Reuse the I variable from above
		// It is ugly but works for the moment
		// If the I is 5 then we can read avaiable disk space.
		// if I is 3 then we get total size
		if (strcasecmp(*storageType, "total") == 0 && *i == 1)
		{

			// printf("%i", *i);
			*pointerchar = strtok(NULL, " ");
			break;
		}
		if (strcasecmp(*storageType, "available") == 0 && *i == 3)
		{
			// printf("%i - here __  ", *i);
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
		fclose(fp);
		return;
	}
	fscanf(fp, "%i", mem);
	/* close */
	fclose(fp);
}

// Reads the current total running processes on the machine
// Needs to be implemented
void getTotalProcesses()
{
	FILE *fp;
	// char *cmd = "top -b -n 1";
	char *cmd = "top -l 1";
	fp = popen(cmd, "r");
	int a = 0;
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		fclose(fp);
		return;
	}
	char string[150];
	char *pointerchar;
	while (fgets(string, 150, fp))
	{
		pointerchar = strtok(string, " \t");
		while (pointerchar != NULL)
		{
			if (strcasecmp(pointerchar, "Processes:") == 0 || strcasecmp(pointerchar, "Tasks:") == 0)
			{
				// As we know we want the next value then just print it.
				pointerchar = strtok(NULL, " \t");
				printf("%s", pointerchar);
				return;
			}
			// Read rest of the tokeized string.
			pointerchar = strtok(NULL, " \t");
		}
	}
	fclose(fp);
}

// Returns storage size based off of storageType.
// gets available or total.
// returns nothing if none of these strings are given.
void systemStorageSpace(int *storage, char *storageType)
{
	printf("%s", "system Storage  ");
	if (strcasecmp(storageType, "total") != 0 && strcasecmp(storageType, "available") != 0)
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
	int readLine = 0;
	while (fgets(tokenizedString, 150, fp))
	{
		// Skip the initial row as we dont need to read it.
		i++;
		if (i == 1)
		{
			continue;
		}
		pointerchar = strtok(tokenizedString, " ");
		returnStorageSize(&pointerchar, &storageType, &readLine);
		break;
	}
	// Remove last character from string
	pointerchar[strlen(pointerchar) - 1] = '\0';
	*storage = atoi(pointerchar);
	/* close */
	fclose(fp);
}

// Main is for testing the top command currently.
// int main()
// {
// 	int i = 0;
// 	// getTotalProcesses();
// 	systemStorageSpace(&i, "total");
// 	printf("%i", i);
// 	return 0;
// }
