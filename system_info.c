#include <stdio.h>
#include <sys/utsname.h>
#include "system_info.h"

void print_system_info(void){
    struct utsname info;
    if (uname(&info) != -1){
        printf("*** NEMONITOR ***\n\n");
        printf("%s\n", info.machine);
        printf("%s\n", info.sysname);
        printf("%s\n\n", info.release);
    }
    else{
        perror("uname");
        return;
    }
}

void print_memory_info(void){
    FILE *pFile = fopen("/proc/meminfo", "r");
	if (pFile == NULL){
		perror("fopen");
		return;
	}
	
    char line[256];
	unsigned long totalRam = 0;
	unsigned long availableRam = 0;
	int foundTotal = 0;
	int foundAvailable = 0;
	while(fgets(line, sizeof(line), pFile) != NULL){
		if (sscanf(line, "MemTotal: %lu kB", &totalRam) == 1){
			foundTotal = 1;
		}
		if (sscanf(line, "MemAvailable: %lu kB", &availableRam) == 1){
			foundAvailable = 1;
		}
	}
	
	if (foundTotal == 1 && foundAvailable == 1){
		double totalRamGiB = totalRam / 1024.0 / 1024.0;
		printf("Total Ram : %.2f GiB\n", totalRamGiB);

		unsigned long usedRam = totalRam - availableRam;
		double usedRamGiB = usedRam / 1024.0 / 1024.0;
		printf("Used Ram : %.2f GiB\n", usedRamGiB);

		double availableRamGiB = availableRam / 1024.0 / 1024.0;
		printf("Available Ram: %.2f GiB\n", availableRamGiB);
	}
	else{
		printf("Error: couldn't find the necessary information.\n");
	}
	fclose(pFile);
}