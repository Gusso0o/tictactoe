#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALTEZZA_CAMPO 3
#define LARGHEZZA_CAMPO 3
#define LUNGHEZZAMAX_NOME 15

//Definisco uno struct con all'interno le variabili
typedef struct Partita{
    char campoDaGioco[ALTEZZA_CAMPO][LARGHEZZA_CAMPO];

    char giocatore1[LUNGHEZZAMAX_NOME];
    int counterMosse1;
    char simbolo1;

    char giocatore2[LUNGHEZZAMAX_NOME];
    int counterMosse2;
    char simbolo2;

} partita;

//funzione per stampare il campo da gioco. Funziona basandosi sul pari/dispari della line per capire cosa stampare
void stampaCampoDaGioco(partita *gioco){
    for(int i = 1; i < ALTEZZA_CAMPO*2; i++){
        printf("\t\t\t\t");
        for(int j = 1; j < LARGHEZZA_CAMPO*2; j++){
            if(i%2 == 0){
                printf("\033[0;31m-\033[0;37m");
            } else {
                if(j%2 == 0){
                    printf("\033[0;31m|\033[0;37m");
                } else {
                    printf("%c", gioco->campoDaGioco[i/2][j/2]);
                }
            }
        }
        printf("\n");
    }
}

//controllo del tris analizzando ogni parte del campo da gioco
bool controlloTris(partita *gioco){
    int counterSegniConnessi = 0;  

    //controllo righe
    for(int i = 0; i < ALTEZZA_CAMPO; i++){
        for(int j = 1; j < LARGHEZZA_CAMPO; j++){
            if(gioco->campoDaGioco[i][j] != ' ' && gioco->campoDaGioco[i][j-1] != ' '){
                if(gioco->campoDaGioco[i][j] == gioco->campoDaGioco[i][j-1]){
                    counterSegniConnessi++;
                }
            }
        }
        if(counterSegniConnessi > 1){
            return 1;
        }
        counterSegniConnessi = 0;
    }

    //controllo colonne
    for(int i = 0; i < LARGHEZZA_CAMPO; i++){
        for(int j = 1; j < ALTEZZA_CAMPO; j++){
            if(gioco->campoDaGioco[j][i] != ' ' && gioco->campoDaGioco[j-1][i] != ' '){
                if(gioco->campoDaGioco[j][i] == gioco->campoDaGioco[j-1][i]){
                    counterSegniConnessi++;
                }
            }
        }
        if(counterSegniConnessi > 1){
            return 1;
        }
        counterSegniConnessi = 0;
    }

    //controllo tris a croce
    if((gioco->campoDaGioco[0][2] == gioco->campoDaGioco[1][1]) && (gioco->campoDaGioco[1][1] == gioco->campoDaGioco[2][0]) && (gioco->campoDaGioco[0][2] != ' ') && (gioco->campoDaGioco[1][1] != ' ') && (gioco->campoDaGioco[2][0] != ' ')){
        return 1;
    }
    
    
    if((gioco->campoDaGioco[0][0] == gioco->campoDaGioco[1][1]) && (gioco->campoDaGioco[1][1] == gioco-> campoDaGioco[2][2]) && (gioco->campoDaGioco[0][0] != ' ') && (gioco->campoDaGioco[1][1] != ' ') && (gioco->campoDaGioco[2][2] != ' ')){
        return 1;
    }
    
    return 0;
}

//questa funziona ha il compito di controllare il pareggio.
bool controlloDraw(partita *partitone){
    int numeroCasellePiene = 0;
    
    for(int i = 0; i < ALTEZZA_CAMPO; i++){
        for(int j = 0; j < LARGHEZZA_CAMPO; j++){
            if(partitone->campoDaGioco[i][j] != ' '){
                numeroCasellePiene++;      
            }
        }
    }

    if(numeroCasellePiene == 9){
        return 1;
    } else {
        return 0;
    }
}

//salvo la partita su un file con alcuni dati
void salvaPartita(partita *gioco){
    FILE *file;
    file = fopen("partita_tetris.txt", "a");
    if(file == NULL){
        printf("\nC'è stato un errore nell'apertura del file!");
        return;
    }

    fprintf(file, "\nGiocatore 1: %s", gioco->giocatore1);
    fprintf(file, "\nGiocatore 2: %s\n", gioco->giocatore2);
    for(int i = 0; i < ALTEZZA_CAMPO*2+1; i++){
        fprintf(file, "\t\t\t\t");
        for(int j = 0; j < LARGHEZZA_CAMPO*2+1; j++){
            if(i%2 == 0){
                fprintf(file, "-");
            } else {
                if(j%2 == 0){
                    fprintf(file, "|");
                } else {
                    fprintf(file, "%c", gioco->campoDaGioco[i/2][j/2]);
                }
            }
        }
        fprintf(file, "\n");
    }
    fprintf(file, "Mosse giocatore 1: %d", gioco->counterMosse1);
    fprintf(file, "\nMosse giocatore 2: %d", gioco->counterMosse2);
    fprintf(file, "\n------------------------------------------------------------------------------");
    fclose(file);
    printf("\nPartita salvata correttamente!");
}


void partitaInCorso(partita *gioco){
    bool isTris = 0;
    int isGiocatore1Turn = 1;
    int coordX1 = 0;
    int coordY1 = 0;
    int coordX2 = 0;
    int coordY2 = 0;
    //inizializzo il campo prima della partita
    for(int i = 0; i < ALTEZZA_CAMPO; i++){
        for(int j = 0; j < LARGHEZZA_CAMPO; j++){
            gioco->campoDaGioco[i][j] = ' ';
        }
    }
    gioco->counterMosse1 = 0;
    gioco->counterMosse2 = 0;
    //eseguo finche non si è fatto tris
    while(isTris != 1){

        if(isGiocatore1Turn == 1){
            gioco->counterMosse1++;

            //non metto & perchè è già referenziato
            stampaCampoDaGioco(gioco);
            printf("\n\033[0;36m[%s]\033[0;37m Inserisci la coordinata X di dove vuoi inserire il tuo simbolo: ", gioco->giocatore1);
            scanf("%d", &coordX1);
            system("cls");
            stampaCampoDaGioco(gioco);
            printf("\n\033[0;36m[%s]\033[0;37m Inserisci la coordinata Y di dove vuoi inserire il tuo simbolo: ", gioco->giocatore1);
            scanf("%d", &coordY1);
            system("cls");

            //controllo se la casella è già occupata o non esiste
            while((gioco->campoDaGioco[coordX1][coordY1] != ' ') || (coordX1 >= LARGHEZZA_CAMPO || coordY1 >= ALTEZZA_CAMPO)){
                stampaCampoDaGioco(gioco);
                printf("\n\033[0;33m[%s] Oops! Quella casella e' gia stata occupata o non esiste. Per favore, inserisci il tuo simbolo in caselle non occupate e esistenti.\033[0;37m", gioco->giocatore1);
                printf("\n\033[0;36m[%s]\033[0;37m Inserisci la coordinata X di dove vuoi inserire il tuo simbolo: ", gioco->giocatore1);
                scanf("%d", &coordX1);
                system("cls");
                stampaCampoDaGioco(gioco);
                printf("\n\033[0;36m[%s]\033[0;37m Inserisci la coordinata Y di dove vuoi inserire il tuo simbolo: ", gioco->giocatore1);
                scanf("%d", &coordY1);
                system("cls");
            }

            gioco->campoDaGioco[coordX1][coordY1] = gioco->simbolo1;
            //controllo se c'è tris
            if(controlloTris(gioco) == 1){
                stampaCampoDaGioco(gioco);
                printf("\033[0;36m[%s]\033[0;32m Congratulazioni! Hai vinto!\033[0;33m\n", gioco->giocatore1);
                isTris = 1;
            }

            isGiocatore1Turn = 0;
            //controllo se c'è draw
            if(controlloDraw(gioco) == 1){
                printf("\n\033[0;31m[%s&%s] Pareggio\033[0;37m", gioco->giocatore1, gioco->giocatore2);
                return;
            }

        } else {
            gioco->counterMosse2++;
            //non metto & perchè è già referenziato
            stampaCampoDaGioco(gioco);
            printf("\n\033[0;35m[%s]\033[0;37m Inserisci la coordinata X di dove vuoi inserire il tuo simbolo: ", gioco->giocatore2);
            scanf("%d", &coordX2);
            system("cls");
            stampaCampoDaGioco(gioco);
            printf("\n\033[0;35m[%s]\033[0;37m Inserisci la coordinata Y di dove vuoi inserire il tuo simbolo: ", gioco->giocatore2);
            scanf("%d", &coordY2);
            system("cls");

            while((gioco->campoDaGioco[coordX2][coordY2] != ' ') || (coordX2 >= LARGHEZZA_CAMPO || coordY2 >= ALTEZZA_CAMPO)){
                stampaCampoDaGioco(gioco);
                printf("\n\033[0;33m[%s] Oops! Quella casella e' gia stata occupata o non esiste. Per favore, inserisci il tuo simbolo in caselle non occupate e esistenti.\033[0;37m", gioco->giocatore2);
                printf("\n\033[0;35m[%s]\033[0;37m Inserisci la coordinata X di dove vuoi inserire il tuo simbolo: ", gioco->giocatore2);
                scanf("%d", &coordX2);
                system("cls");
                stampaCampoDaGioco(gioco);
                printf("\n\033[0;35m[%s]\033[0;37m Inserisci la coordinata Y di dove vuoi inserire il tuo simbolo: ", gioco->giocatore2);
                scanf("%d", &coordY2);
                system("cls");
            }

            gioco->campoDaGioco[coordX2][coordY2] = gioco->simbolo2;

            if(controlloTris(gioco) == 1){
                stampaCampoDaGioco(gioco);
                printf("\033[0;35m[%s]\033[0;32m Congratulazioni! Hai vinto!\033[0;37m\n", gioco->giocatore2);
                isTris = 1;
            } else {
                isGiocatore1Turn = 1;
            }
        }

        

    }
}

int main(){
    partita game;
    int replay = 1;

    system("cls");
    printf("\n\033[0;36m[Giocatore 1]\033[0;37m Benvenuto nel gioco del tris. Per iniziare, inserisci il nome del giocatore 1: ");
    scanf("%s", game.giocatore1);
    system("cls");
    printf("\n\033[0;36m[%s]\033[0;37m %s, scegli il simbolo con cui vuoi giocare: ", game.giocatore1, game.giocatore1);
    scanf("%s", &game.simbolo1);
    system("cls");
    
    printf("\n\033[0;35m[Giocatore 2]\033[0;37m Giocatore 2, inserisci il tuo nome: ");
    scanf("%s", game.giocatore2);
    system("cls");
    //controllo che il giocatore 2 non inserisca lo stesso simbolo del giocatore 1
    do{
        printf("\n\033[0;35m[%s]\033[0;37m %s, scegli il simbolo con cui vuoi giocare (per evitare confusione, non inserire '%c', il simbolo di %s): ", game.giocatore2, game.giocatore2, game.simbolo1, game.giocatore1);
        scanf("%s", &game.simbolo2);
        system("cls");
    }while(game.simbolo2 == game.simbolo1);

    while(replay == 1){
        partitaInCorso(&game);
        printf("\n\033[0;31mVuoi salvare la partita? [1] \033[0;37m");
        scanf("%d", &replay);
        if(replay == 1){
            salvaPartita(&game);
        }

        replay = 0;
        printf("\n\033[0;31mNuova partita? [1] \033[0;37m");
        scanf("%d", &replay);
        system("cls");
    }
}