#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Nevalidan br. argumenata\n");
        exit(1);
    }

    char* path1 = argv[1];
    char* path2 = argv[2];

    DIR* dp1 = opendir(path1);
    DIR* dp2 = opendir(path2);

    if (!dp1 || !dp2){
        perror("opendir");
        exit(1);
    }

    struct dirent* dirp1;
    struct dirent* dirp2;
    struct stat stats1;
    struct stat stats2;
    char fullpath1[1024];
    char fullpath2[1024];

    while ((dirp1 = readdir(dp1)) != NULL){
        snprintf(fullpath1, sizeof(fullpath1), "%s/%s", path1, dirp1->d_name);
        stat(fullpath1, &stats1);
        if (S_ISREG(stats1.st_mode)){
            rewinddir(dp2);
            while ((dirp2 = readdir(dp2)) != NULL){
                snprintf(fullpath2, sizeof(fullpath2), "%s/%s", path2, dirp2->d_name);
                stat(fullpath2, &stats2);
                if (S_ISREG(stats2.st_mode)){
                    if (strcmp(dirp2->d_name, dirp1->d_name) == 0){
                        printf("Datoteka %s postoji u oba direktorijuma.\n", dirp1->d_name);
                        break;
                    }
                }
            }
        }
    }

    closedir(dp1);
    closedir(dp2);

    return 0;
}