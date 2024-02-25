#ifndef SYSSTAT_HEADER
#define SYSSTAT_HEADER
#define TOTALSTORAGE "total"
#define AVAILABLESTORAGE "available"
int cpuTemperature(int *temp);
int getSystemMemoryInformation(int *mem, int memtype);
// void getTotalProcesses();
int systemStorageSpace(int *storage, char *storageType);
int getProcessesData(char *topLine, char*command, int maxOutputLength);
int getSystemName(char *machineName, int maxLength);
int getIp(char *ip, int maxLength);
int getSystemKernelInfo(char *kernelInfo, int maxLength);
int getSystemKernelInfo(char *kernelInfo, int maxLength);
int getPm2Data(char *pm2Data, char *command, int maxOutputLength);
#endif