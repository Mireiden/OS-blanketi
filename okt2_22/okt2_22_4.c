//oktobar 2 2022. 4. zadatak
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <malloc.h>

#define MAX_PATH_LENGTH 257

struct dirent dirp;

int findFile(char* path, char* fileName, char* fileLocation, off_t* fileSize){
    DIR* dp = opendir(path);
    int found = 0;
    struct dirent* dirp;
    if (!dp) return 0;

    while (dirp = readdir(dp)){
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) continue;
        char fullPath[MAX_PATH_LENGTH];
        snprintf(fullPath, MAX_PATH_LENGTH, "%s/%s", path, dirp->d_name);
        struct stat stats;
        stat(fullPath, &stats);
        if (strcmp(dirp->d_name, fileName) == 0){
            //ovaj if je dodat kako bi vracao 1 samo za fajlove a ne i za foldere
            if (S_ISREG(stats.st_mode)){
                strcpy(fileLocation, fullPath);
                *fileSize = stats.st_size;
                closedir(dp);
                return 1;
            }
        }
        if (S_ISDIR(stats.st_mode)) found = findFile(fullPath, fileName, fileLocation, fileSize);
    }
    return found;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: %s <directory> <filename>\n", argv[0]);
        return 1;
    }

    char* fileLocation = malloc(MAX_PATH_LENGTH * sizeof(char));
    off_t fileSize = 0;

    int found = findFile(argv[1], argv[2], fileLocation, &fileSize);

    if (found) printf("Fajl je nadjen na lokaciji %s\nVelicina fajla je: %d \n", fileLocation, (int)fileSize);
    else printf("Fajl nije nadjen.\n");

    return 0;
}