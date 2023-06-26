#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Value of 1024*1024
#define MB 1048576;

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
void getTotalProcesses()
{
	FILE *fp;
	// char *cmd ="top -b -n 1";
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
	int readNext = 0;
	while (fgets(string, 150, fp))
	{
		pointerchar = strtok(string, " \t");
		while (pointerchar != NULL)
		{

			if (strcasecmp(pointerchar, "Processes:") == 0 || strcasecmp(pointerchar, "Tasks:") == 0)
			{
				readNext = 1;
				// As we know we want the next value then just print it.
				pointerchar = strtok(NULL, " \t");
				printf("%s", pointerchar);
				return;
			}

			pointerchar = strtok(NULL, " \t");
		}
	}

	fclose(fp);
}

// Main is for testing the top command currently.
int main()
{
	readTopMac();
	return 0;
}
