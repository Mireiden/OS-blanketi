//april 2023, 4. zadatak.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>

#define MAX_LEVEL 5
#define MAX_PATH 4096
#define MAX_PATH_LENGTH 257

void findLargestFile(char* path, int level, off_t* maxsize, char* maxfile){
    if (level >= MAX_LEVEL) return;

    DIR* dp = opendir(path);
    struct dirent* dirp;
    struct stat stats;
    char fullPath[MAX_PATH_LENGTH];

    if (!dp){
        perror("opendir");
        return;
    }

    while ((dirp = readdir(dp)) != NULL){
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) continue;
        snprintf(fullPath, MAX_PATH_LENGTH, "%s/%s", path, dirp->d_name);
        stat(fullPath, &stats);

        if (S_ISDIR(stats.st_mode)){
            findLargestFile(fullPath, level + 1, maxsize, maxfile);
            continue;
        }

        if (!S_ISREG(stats.st_mode)) continue;

        if (stats.st_size > *maxsize){
            *maxsize = stats.st_size;
            strcpy(maxfile, dirp->d_name);
        }
    }
    closedir(dp);
}

int main(int argc, char* argv[]){
    int level = 0;
    off_t size = 0;
    char* fileName = malloc(MAX_PATH);

    findLargestFile(argv[1], 0, &size, fileName);
    printf("Najveci fajl: %s\n", fileName);

    free(fileName);
    return 0;
}