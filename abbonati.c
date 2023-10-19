#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 256
#define NOM 32
#define FILENAME "elenco.txt"

typedef struct elem_{
  char nome[NOM+1];
  int pack;
  struct elem_ *next;
  int mesi;
}elem;


elem* esegui(char *filename);
void calcolaPack(elem *tmp);
elem* aggiorna(elem *lista, int n_mesi, char *filename);
void visualizza(elem* lista);
void scriviFile(elem *lista, char *filename);


int main(int argc, char const *argv[]) {

  elem* lista;
  int scelta;
  int flag=0;

  printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   \n BENVENUTO NEL PROGRAMMA di controllo e aggiornamento degli abbonati PER IL DISCORD: \n");
  printf(" \n\tINIZIAMO \n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - -\n");
  printf(" [Sto utilizzando il FILE]: %s\n \n", FILENAME);



  do{
    sleep(2);
    printf("\n Inserisci un valore: \n\n 1) Vedi lista Membri/Mesi/Packs \n 2) Aggiorna Mese \n  (-1 per uscire) --> ");
    scanf("%d", &scelta);
    printf("\n");

      switch (scelta) {
        case 1:
            lista = esegui(FILENAME);
            sleep(1);
            visualizza(lista);
            flag=1;
            break;
        case 2:
            if(flag == 1){
              printf("\n Quanti mesi sono passati? -> ");
              int n_mesi;
              scanf("%d", &n_mesi);
              printf("\n");
              aggiorna(lista,n_mesi, FILENAME);
              scriviFile(lista, FILENAME); // Aggiornamento del file
              sleep(1);
              visualizza(lista);
            }
            else{
              printf("devi prima creare la lista, premi 1");
            }
            break;
      }
      sleep(3);
      printf("\n - - - - - - - - - - - - - - - \n");
  }while(scelta != -1);

  return 0;
}


elem* esegui(char *filename){
  char line[SIZE];

  elem *tmp;
  elem *lista = NULL;
  elem *prec = NULL;

  FILE *fd = fopen (filename, "r");
  if(fd == NULL)
    perror("err fopen");

  while(fgets(line, SIZE, fd)){
    tmp = (elem*) malloc(sizeof(elem));
    if(tmp == NULL)
      perror("Errore nell'allocazione di memoria per tmp");

    // Inizializza i campi di tmp
    tmp->next = NULL;
    tmp->pack = 0;
    tmp->mesi = 0;

    // Parsing della riga
    int num_read = sscanf(line, "%32[^,], %d", tmp->nome, &tmp->mesi);


    if (num_read != 2) {
      // Gestione degli errori o righe non valide
      free(tmp);
      continue;
    }

    // Aggiungi tmp alla lista
    if(lista == NULL){
      lista = tmp;
      prec = tmp;
    }
    else{
      prec->next = tmp;
      prec = tmp;
    }
    calcolaPack(tmp);
  }

  fclose(fd); // Chiudi il file quando hai finito di leggere

  return lista;
}

void calcolaPack(elem *tmp) {
    for (int i = 0; i < 10; i++) {
        if (tmp->mesi == i) {
            tmp->pack = i / 2;
        }
    }
}

elem* aggiorna(elem* lista, int n_mesi, char *filename){
    elem *tmp;
    char line[SIZE];

    while(lista != NULL){
        tmp = lista;
        tmp->mesi += n_mesi;
        if(tmp->mesi >12)
          tmp->mesi = tmp->mesi -12;
        calcolaPack(tmp);
        lista = lista->next;
    }
    return lista;
}

void scriviFile(elem *lista, char *filename) {
  FILE *fd = fopen(filename, "w");
  if (fd == NULL) {
    perror("err fopen");
    return;
  }

  while (lista != NULL) {
    fprintf(fd, "%s, %d\n", lista->nome, lista->mesi);
    lista = lista->next;
  }

  fclose(fd);
}

void visualizza(elem* lista){
  while(lista!= NULL){
      printf("\n Nome: %s \n Mesi: %d \n Pack: %d \n\n", lista->nome, lista->mesi, lista->pack);
      lista = lista->next;
  }
}
