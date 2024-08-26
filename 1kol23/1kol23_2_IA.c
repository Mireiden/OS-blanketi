//prvi kolokvijum 2023. 2. zadatak grupa I-a
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>

int main(int argc, char* argv[]){
    FILE* ulaz;

    char* ulazniFajl = argv[1];
    char* izlazniFajl = argv[2];

    int pajp[2];
    pipe(pajp);

    if ((ulaz = fopen("data.txt", "r")) == NULL){
        printf("Greska pri otvaranju fajla!\n");
        exit(1);
    }

    if (fork() != 0){
        close(pajp[0]);
        ulaz = fopen(ulazniFajl, "r");
        char linija[100];
        while (fgets(linija, 100, ulaz))
            write(pajp[1], linija, strlen(linija));
        close(pajp[1]);
        fclose(ulaz);
    }

    else{
        close(pajp[1]);
        char primljenaLinija[100];

        int redirekcija = open(izlazniFajl, O_TRUNC | O_CREAT | O_RDWR, 0777);
        dup2(redirekcija, 1);

        while (read(pajp[0], &primljenaLinija, sizeof(primljenaLinija)) > 0){
            for (int i = 0; i < strlen(primljenaLinija); i++){
                if (primljenaLinija[i] == 'A')
                    primljenaLinija[i] = 'Z';

                else if (primljenaLinija[i] == 'a')
                    primljenaLinija[i] = 'z';

                else
                    primljenaLinija[i] -= 1;
            }
            printf("%s", primljenaLinija);
        }
        close(pajp[0]);
        close(redirekcija);
    }
    return 0;
}