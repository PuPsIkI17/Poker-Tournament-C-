/*Pislari Vadim */

#include "info.h"

int ins_lista(ALista L, void* cel, size_t d);

void printare(TLista L,FILE *out);

int noroc(Sala sala,char mas_in[Max],char numejuc[Max],int grad ,FILE *out);

int ghinion(Sala sala,char mas_in[Max],char numejuc[Max],int grad, FILE *out);

void tura(Sala sala,char mas_in[Max] , FILE *out);

int inchide(Sala sala,char mas_in[Max], FILE *out);

void tura_completa(Sala sala, FILE *out);

void clasament(Sala sala,char mas_in[Max], FILE *out);

int main(int argc, char *argv[])
{
    //deschiderea fisierelor de citire
    FILE *in = fopen(argv[1],"rt");
    if(!in){
    printf("Fisierul nu s-a deschis\n");
    return 0;
    }
    FILE *op = fopen(argv[2],"rt");
    if(!op){
    printf("Fisierul nu s-a deschis\n");
    return 0;
    }
    FILE *out = fopen(argv[3],"wt");
    if(!out){
    printf("Fisierul nu s-a deschis\n");
    return 0;
    }

    int nr_mese,i = 0;	
    fscanf(in,"%d",&nr_mese);
    Sala sala = malloc(sizeof(struct sala));
    //setarea valorilor pentru sala
    sala->nrMese = nr_mese;
    sala->nrLocCrt = 0;
    sala->nrLocMax = 0;
    TLista L = NULL;
    while(i != nr_mese){
	Masa M = NULL;

	//alocarea continua astfel ca ele sa se reinoiasca
  	M = (Masa) malloc(sizeof(struct masa));
	if(!M)return 0;	
	M->numeMasa = calloc(Max,sizeof(char));
	if(!M->numeMasa){
           free(M);
	   return 0;	
	}

	//citire datelor despre masa
	fscanf(in ,"%s%d%d", M->numeMasa, &M->nrCrtJucatori, &M->nrMaxJucatori);

	//resetarea datelor pentru sala
	sala->nrLocCrt += M->nrCrtJucatori;
	sala->nrLocMax += M->nrMaxJucatori;

	//inserarea in lista
	ins_lista(&L, M, sizeof(M));	
	int j = 0;
	
	//crearea listei de jucatori
 	TLista LJ = NULL;
	while(j != M->nrCrtJucatori){
	      Jucator J = NULL;
	      J = malloc(sizeof(struct jucator));
	      if(!J)return 0;	
              J->nume = calloc(Max, sizeof(char));
	      if(!J->nume){
	          free(J);
	          return 0;	
	      }
	      //citirea datelor despre jucator
	      fscanf(in, "%s%d", J->nume, &J->nrMaini);
	      ins_lista(&LJ, J, sizeof(J));
	      j++;		 	
	}

	//crearea santinelei
	TLista santinel = malloc(sizeof(Celula));
	if(!santinel) return 0;
	santinel->info = NULL;
	santinel->urm = LJ;
	M->jucatori = santinel;

	//crearea listei ciclice
	TLista sant_aux = LJ;
	while(sant_aux->urm) sant_aux = sant_aux->urm;
	sant_aux->urm = santinel;
    	i++;
	sala->masa = L;
    }
   
    //citirea functiilor din al 2 fisier
    char func[Max];
    while(fscanf(op, "%s", func) == 1){
	    if(strcmp(func,"noroc") == 0){
		char mas_in[Max], numejuc[Max];
		int grad;
	    	fscanf(op,"%s%s%d", mas_in, numejuc, &grad);
		noroc(sala, mas_in, numejuc, grad, out);
	    }
	     if(strcmp(func, "ghinion") == 0){
		char mas_in[Max], numejuc[Max];
		int grad;
	    	fscanf(op, "%s%s%d", mas_in, numejuc, &grad);
		ghinion(sala, mas_in, numejuc, grad, out);
	    }
	     if(strcmp(func, "inchide") == 0){
		char mas_in[Max];
	    	fscanf(op, "%s", mas_in);
		inchide(sala, mas_in, out);
	    }
	     if(strcmp(func, "tura") == 0){
		char mas_in[Max];
	    	fscanf(op, "%s", mas_in);
		tura(sala, mas_in, out);
	    }
	     if(strcmp(func, "tura_completa") == 0){
		tura_completa(sala, out);
	    }
	     if(strcmp(func, "clasament") == 0){
		char mas_in[Max];
	    	fscanf(op, "%s", mas_in);
		clasament(sala, mas_in, out);
	    }
	    if(strcmp(func, "print") == 0) printare(sala->masa, out);
    }

while(fscanf(op, "%s", func) == 1){
	    if(strcmp(func,"noroc") == 0){
		char mas_in[Max], numejuc[Max];
		int grad;
	    	fscanf(op,"%s%s%d", mas_in, numejuc, &grad);
		noroc(sala, mas_in, numejuc, grad, out);
	    }

	    if(strcmp(func, "print") == 0) printare(sala->masa, out);
    }

    //eliberare memorie 
    TLista A = sala->masa;
    TLista ant = NULL; 
    for(; A != NULL; ant = A, A = A->urm){
	free(ant);
	Masa M = (Masa)A->info;
    	TLista J = M->jucatori;
	if(J!=NULL && J->urm!=NULL)J = J->urm;
	TLista antJ = NULL;
	for(; ;antJ = J, J = J->urm){
		if(J->info == NULL){
			free(J);		
			break;
		}
	  	free(antJ);
	  	Jucator joc = (Jucator)J->info;
	 	free(joc->nume);
	 	free(joc);
	}
	//eliberarea santinelei
	free(antJ);
	free(M->numeMasa);
	free(M);
    }	
    free(ant);
    free(sala);
    fclose(in);
    fclose(out);
    return 1;
}

