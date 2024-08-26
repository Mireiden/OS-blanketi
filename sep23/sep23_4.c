#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LINE_LENGTH 100
#define MAX_PATH_LENGTH 257

int main(int argc, char* argv[]){
    struct dirent* dirp;
    DIR* dp;
    struct stat fstats;
    int n, m, fd;
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    char fullPath[MAX_PATH_LENGTH];

    if (argc != 4){
        fprintf(stderr, "Usage: %s <directory> <n> <m>\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[2]);
    m = atoi(argv[3]);

    if ((dp = opendir(argv[1])) == NULL){
        perror("opendir");
        exit(1);
    }

    while ((dirp = readdir(dp)) != NULL && count < n){
        snprintf(fullPath, MAX_PATH_LENGTH, "%s/%s", argv[1], dirp->d_name);

        stat(fullPath, &fstats);

        if (!S_ISREG(fstats.st_mode)) continue;

        if ((fd = open(fullPath, O_RDONLY)) < 0){
            perror("open");
            continue;
        }

        FILE* file = fdopen(fd, "r");
        if (!file){
            perror("fdopen");
            close(fd);
            continue;
        }

        printf("File: %s\n", fullPath);
        for (int i = 0; i < m; i++){
            if (fgets(buffer, MAX_LINE_LENGTH, file) == NULL) break;
            printf("%s", buffer);
        }
        printf("\n");

        fclose(file);  // fclose takodje zatvara i deskriptor
        count++;
    }

    closedir(dp);
    return 0;
}
