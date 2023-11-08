#ifndef SYSSTAT_HEADER
#define SYSSTAT_HEADER
#define TOTALSTORAGE "total"
#define AVAILABLESTORAGE "available"
void cpuTemperature(int *temp);
void getSystemMemoryInformation(int *mem, int memtype);
// void getTotalProcesses();
void systemStorageSpace(int *storage, char *storageType);
void getProcessesData(char *topLine, char*command, int maxOutputLength);
void getSystemName(char *machineName, int maxLength);
void getSystemKernelInfo(char *kernelInfo, int maxLength);
#endif