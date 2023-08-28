#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

void intToPaddedString(int number, char *num_str, int width) {
    sprintf(num_str, "%d", number);

    int length = strlen(num_str);
    int padding = width - length;

    if (padding > 0) {
        memmove(num_str + padding, num_str, length + 1);
        for (int i = 0; i < padding; i++) {
            num_str[i] = '0';
        }
    }
}

int main(int argc, char **argv) {
    int fd = open("/proc/Flag-Checker", O_RDWR);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    // set layer to true
    unsigned long request = 0x7000;
    int result = ioctl(fd, request, "SEKAI{BRUH}");
        if (result != 0)
            printf("The result is : %d\n", result);
        else
            printf("Wrong !");

    request = 0x7001;
    int i = 0;
    while (i < 9999999)
    {
        char num_str[8];
        intToPaddedString(i, num_str, 7);
        int result = ioctl(fd, request, num_str);
        if (result != 0)
        {
            printf("The num is : %d\n", i);
            break;
        }
        else
            printf("%s\n", num_str);
        i++;
    }

    close(fd);
    return 0;
}
