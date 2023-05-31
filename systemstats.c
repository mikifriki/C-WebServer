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
