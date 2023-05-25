#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cpuTemperature(FILE *fp, double *temp)
{
	char output[1024];
	char *eptr;
	// We want to ignore any errors from the sensors command.
	char *cmd = "sensors  2>/dev/null | grep 'Package id 0:' | grep -o -P '(?<=\\+).*?(?=°)' | head -1";
	/* Run the command given. */
	fp = popen(cmd, "r");
	// If the FilePointer is Null then no need to resume as no data will be fetched.
	if (fp == NULL)
	{
		printf("Failed to run command\n");
		pclose(fp);
		return;
	}

	/* Read the output a line at a time - output it. */
	// Should have a if statement.
	if (fgets(output, sizeof(output), fp) != NULL)
	{
		*temp = strtod(output, &eptr);
	}
	// debug print.
	// printf("%s", output);
	pclose(fp);
}

//  Returns available memory of system in kb as given my meminfo. By default shows Free memory.
void getSystemMemoryInformation(FILE *fp, int *mem, int memtype)
{
	char output[1024];
	char *cmd = "grep 'MemFree: ' /proc/meminfo | grep -o -P '(?<= ).*?(?= )'";
	if (memtype == 0)
	{
		cmd = "grep 'MemAvailable: ' /proc/meminfo | grep -o -P '(?<= ).*?(?= )'";
	}
	if (memtype == 1)
	{
		cmd = "grep 'MemTotal: ' /proc/meminfo | grep -o -P '(?<= ).*?(?= )'";
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
	/* Read the output a line at a time - output it. */
	// Should have a if statement.
	if (fgets(output, sizeof(output), fp) != NULL)
	{
		// Convert the string into a int.
		*mem = atoi(output);
	}
	/* close */
	pclose(fp);
}

// int main()
// {
// 	FILE *fp;
// 	int mem;
// 	double cpuTemp;
// 	cpuTemperature(fp, &cpuTemp);
// 	printf("%f  \n", cpuTemp);
// 	getSystemMemoryInformation(fp, &mem, 0);
// 	printf("%i \n", mem);
// 	return 0;
// }