#ifndef SYSSTAT_HEADER
#define SYSSTAT_HEADER
#define BUFFSIZE 1024
#define PORT 8080
void cpuTemperature(int *temp);
void getSystemMemoryInformation(int *mem, int memtype);
// void getTotalProcesses();
void systemStorageSpace(int *storage, char *storageType);
void getProcessesData(char *topLine, char*command, int maxOutputLength);
#endif