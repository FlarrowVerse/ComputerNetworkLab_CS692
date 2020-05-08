#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {

	struct rusage res_usage;
	int retVal;

	retVal = getrusage(RUSAGE_SELF, &res_usage);

	if (retVal == -1) {
		perror("getrusage error\n");
		return 1;
	}

	printf("Details of getrusage:\n");
   	printf("User CPU time (seconds) is %d\n", (int)res_usage.ru_utime.tv_sec);
	printf("User CPU time (micro seconds) is %d\n", (int)res_usage.ru_utime.tv_usec);
	printf("Maximum size of resident set (kb) is %ld\n", res_usage.ru_maxrss);
	printf("Soft page faults (I/O not required) is %ld\n", res_usage.ru_minflt);
	printf("Hard page faults (I/O not required) is %ld\n", res_usage.ru_majflt);
	printf("Block input operations via file system is %ld\n", res_usage.ru_inblock);
	printf("Block output operations via file system is %ld\n", res_usage.ru_oublock);
	printf("Voluntary context switches are %ld\n", res_usage.ru_nvcsw);
	printf("Involuntary context switches are %ld\n", res_usage.ru_nivcsw);

	return 0;
}