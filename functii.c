/*Pislari Vadim */

#include "info.h"

//inserarea la sfarsit de lista generic
int ins_lista(ALista L, void* cel, size_t d)
{
	TLista aux = (TLista)malloc(sizeof(Celula)); 
	if(!aux) return 0;
	aux->info = cel; 
	aux->urm = NULL;
	TLista temp = *L;
	if(!temp)*L = aux;	
	else {
		while(temp->urm)temp = temp->urm;
		temp->urm = aux;
   	 }
    	return 1;
}

void printnumeMasa(void *n, FILE *out)
{
	Masa v = (Masa)n;
	fprintf(out,"%s:",v->numeMasa);
}

void printjucator(void *n, FILE *out)
{
	Jucator J = (Jucator)n;
	fprintf(out, " %s - %d", J->nume, J->nrMaini);
}

//printarea generica
void print(TLista L, void (*fptr)(void *,FILE *),FILE *out)
{
	fptr(L->info, out);
}

//functia de printare din prob
void printare(TLista L, FILE *out)
{
   for( ; L != NULL ; L = L->urm){
   	print(L, printnumeMasa, out);
	Masa M = L->info;
	TLista J = M->jucatori;
	if(J != NULL && J->urm != NULL)J = J->urm;
	for(;J != NULL ; J=J->urm){
	  //se termina printarea daca s-a ajuns santinela
	  if(J->info == NULL)break;
	  print(J, printjucator, out);
	  //nu se printeaza ";" inainte de punct de la sfirsit
	  if(J->urm->info != NULL) fprintf(out,";");
	}
	fprintf(out, ".\n");
   }
}

//se cauta masa
int cauta_masa(void *a , void *b)
{
	TLista M = (TLista)a;
	Masa Ma = (Masa)M->info;
	char *mas_in = (char*)b;
	if(strcmp(Ma->numeMasa, mas_in) == 0) return 1;
	return 0;
}
//se cauta jucatorul
int cauta_jucator (void *a , void *b)
{
	TLista M = (TLista)a;
	Jucator Ma = (Jucator)M->info;
	char *mas_in = (char*)b;
	if(strcmp(Ma->nume,mas_in) == 0) return 1;
	return 0;
}

//cautarea functie generica in lista circulara de jucatori de la o masa "aux->info!= NULL"
TLista cauta(TLista M, ALista Precedent,void *mas_in, int (*fp)(void *,void *))
{
	TLista aux = M;	
	for( ; aux->info != NULL ; *Precedent = aux,aux = aux->urm){
		if(fp(aux, mas_in) == 1)return aux;	
	}
	return NULL;
}

//cautarea functie generica  in lista de mese "aux!= NULL"
TLista cauta_m(TLista M, ALista Precedent, void *mas_in, int (*fp)(void *,void *))
{
	TLista aux  =M;	
	for( ; aux != NULL ; *Precedent = aux,aux = aux->urm){
		if(fp(aux, mas_in) == 1) return aux;	
	}
	return NULL;
}

Masa body_cauta_masa(Sala sala,char mas_in[Max],FILE *out)
{
	TLista M = (TLista)sala->masa,aux=M;
	//se cauta masa indicata
	TLista Mgas = cauta_m(M,&aux,mas_in,cauta_masa);
	if(!Mgas){
		fprintf(out,"Masa %s nu exista!\n",mas_in);
		return 0;	
	}
	return (Masa)Mgas->info;	
}

//partea comuna de la functia de ghinion si noroc
TLista body(Sala sala,char mas_in[Max], char numejuc[Max],int grad,FILE *out)
{
	Masa Mga = body_cauta_masa(sala, mas_in,out);
	if(!Mga)return NULL;
	TLista Jgas = Mga->jucatori;
	TLista Juc_urm = Jgas->urm;
	//se cauta jucatorul indicat
	TLista J = cauta(Juc_urm,&Jgas,numejuc,cauta_jucator);
	if(!J){
		fprintf(out,"Jucatorul %s nu exista la masa %s!\n",numejuc,mas_in);
		return NULL;	
	}
	//se reintoarce cu o pozitie mai inainte de jucatorul necesar pentru a se putea face stergerea in caz de ceva 
	return Jgas;
}

//functia de noroc
int noroc(Sala sala,char mas_in[Max], char numejuc[Max],int grad,FILE *out)
{
	TLista l = body(sala,mas_in,numejuc,grad,out);
	if(!l) return 0;
	Jucator fin = (Jucator)l->urm->info;
	fin->nrMaini += grad;
	return 1;
}

//functia care verifica daca exista mes goale
void verificare_mese_goale(Sala sala, FILE *out)
{
	TLista M = sala->masa,ant = NULL;
	for(;M != NULL; ){	
		Masa Z = (Masa)M->info;
		TLista LJ = Z->jucatori;
		if(LJ == LJ->urm){
			//daca este primul termen
			sala->nrMese--;
			TLista aux = NULL;
			if(!ant){
				aux = sala->masa;
				sala->masa = sala->masa->urm;
			
			}
			else {
				aux = M;
				ant->urm = M->urm;				

			}
			Masa ps = (Masa)aux->info;
			M = M->urm;
			//eliberarea mesei 
			free (ps->numeMasa);
			free (ps->jucatori->info);
			free (ps->jucatori);
			free (aux->info);	
			free(aux);
			//se verifica daca mai exista sala
			M=sala->masa;
			if(!M)fprintf(out,"Sala este inchisa!\n");
			continue;
		}
		ant = M; M=M->urm;
	}	
}

//functia de ghinion
int ghinion(Sala sala,char mas_in[Max], char numejuc[Max],int grad,FILE *out)
{
	TLista l = body(sala,mas_in,numejuc,grad,out);
	if(!l) return 0;
	Jucator fin = (Jucator)l->urm->info;
	fin->nrMaini -= grad;
	// daca are nr de maini mai mic de 0
	if(fin->nrMaini <= 0){
		Masa Mga = body_cauta_masa(sala, mas_in,out);	
		if(!Mga) return 0;
		Mga->nrCrtJucatori--;
		sala->nrLocCrt--;
		TLista aux = l->urm;
		l->urm = l->urm->urm;
		//eliberarea memoriei
		Jucator ps = (Jucator)aux->info;
		free(ps->nume);
		free(aux->info);
		free(aux);
	}
	verificare_mese_goale(sala,out);
	return 1;
}


void redistribuire(TLista M,TLista N)
{
	Masa m_ind = (Masa)M->info,ACT = (Masa)N->info;
	int nr_juc = m_ind->nrCrtJucatori, i = 0;
	TLista L = m_ind->jucatori->urm;
	for(; N != NULL ; N = N->urm){	
		//se trece de aceiasi masa pentru a nu se insera in ea 
		if(N == M)continue;
		ACT = (Masa)N->info;
		while(i < nr_juc && ACT->nrCrtJucatori < ACT->nrMaxJucatori){
			TLista Final = ACT->jucatori;
			Final = Final->urm;
			// se cauta ultimul element din lista pentru a se insera dupa el 
			while(Final->urm && Final->urm->info)Final=Final->urm;
			TLista aux = L;
			Final->urm = aux;
			L = L->urm;
			aux->urm = ACT->jucatori;			
			ACT->nrCrtJucatori++;
			i++;
		}
	}
}

//functie care calculeaza locurile libere pentru a verifica daca la 
//inchidere mai sunt locuri libere

int calcul_locuri_libere(Sala sala,TLista Mgas)
{
	TLista l = sala->masa;
	int sum_lc_crt = 0, sum_lc_max = 0;
	for( ; l != NULL ; l = l->urm){
		if(l != Mgas){
		Masa M = (Masa)l->info;
		sum_lc_crt += M->nrCrtJucatori;
		sum_lc_max += M->nrMaxJucatori;
		}
	}
	return sum_lc_max - sum_lc_crt;
}

int inchide(Sala sala,char mas_in[Max],FILE *out)
{
	TLista M = sala->masa,actual=M, urm=actual->urm;
	TLista aux = NULL;
	//daca masa indicata este prima
	Masa prima_masa = (Masa)M->info;
	if(strcmp(prima_masa->numeMasa, mas_in) == 0)
		{
		int n = calcul_locuri_libere(sala,M);
		if(prima_masa->nrCrtJucatori > n){
			fprintf(out,"Nu exista suficiente locuri in sala!\n");
			return 1;
		}
		redistribuire(sala->masa,sala->masa->urm);
		aux = sala->masa;
		sala->masa = sala->masa->urm;
		}
	else{
		////se cauta masa indicata
		TLista Mgas = cauta_m(urm,&actual,mas_in,cauta_masa);
		if(!Mgas){
			fprintf(out,"Masa %s nu exista!\n", mas_in);
			return 0;	
		}
		int n=calcul_locuri_libere(sala,Mgas);
		prima_masa = (Masa) Mgas->info;
		if(prima_masa->nrCrtJucatori > n){
			fprintf(out,"Nu exista suficiente locuri in sala!\n");
			return 1;
		}
		redistribuire(Mgas,sala->masa);
		aux = actual->urm;
		actual->urm = Mgas->urm;
	}
	//eliberarea memoriei de la mesele sterse
	Masa ps=(Masa)aux->info;
	free (ps->numeMasa);
	free (ps->jucatori->info);
	free (ps->jucatori);
	free (aux->info);	
	free(aux);
	return 1;
}

//comun de la ambele tipuri de ture
void tura_baza(Masa Mga, Sala sala)
{
	TLista J = Mga->jucatori->urm,JK = J,ant=Mga->jucatori;

	//se verifice sa nu fie doar un element in multimie
	if(JK->urm->info){
		//se face shimbarea de pozitie a primului element care trece la urma
		Mga->jucatori->urm =JK->urm;
	 	for(;J->urm->info != NULL ; J = J->urm);
		J->urm = JK;
		JK->urm = Mga->jucatori;	
	}
	J = Mga->jucatori->urm;
	JK = J;
	//se scade cate -1 de la fiecare jucator
	while(J->info){
		Jucator juc = (Jucator)J->info;
		juc->nrMaini--;	
		J = J->urm;
	}
	J = J->urm;
	//se elimina jucatorii cu o sau mai putine maini de joc
	while(J->info){
		Jucator juc = (Jucator)J->info;
		if(juc->nrMaini <= 0){
			ant->urm = J->urm;
			TLista aux = J;
			J = J->urm;
			Jucator ps=(Jucator)aux->info;
			Mga->nrCrtJucatori--;
			free(ps->nume);
			free(aux->info);
			free(aux);
			continue;
		}
		ant = J;
		J = J->urm;
	}
	
}
//functia de tura
void tura(Sala sala,char mas_in[Max],FILE *out)
{
	Masa Mga=body_cauta_masa(sala, mas_in, out);	
	if (!Mga)return;
	tura_baza(Mga, sala);
	verificare_mese_goale(sala, out);
} 

//functie de tura completa
void tura_completa(Sala sala,FILE *out)
{
	TLista L = sala->masa;
	for(;L != NULL; L = L -> urm){
	 Masa M = (Masa) L->info;
	 tura_baza(M,sala);	
	}
	verificare_mese_goale(sala,out);
}

//functie de compararea care ajuta la inserarea ordonata
int comparare(Jucator a,Jucator b)
{
	if(a->nrMaini > b->nrMaini) return 1;
	else if( a->nrMaini == b->nrMaini && strcmp(b->nume,a->nume)<0) return 1;
	return 0;
}

//functia de inserare ordonata folosita pentru crearea listei pentru afisare
int InsLG(ALista aL, void* ae)
{
  TLista cel = malloc(sizeof(Celula));
  if(!cel) return 0;
  cel->info = ae;
  TLista aux = *aL, ant = NULL;
	// se gasesc celulele si din dreapa si din stanga
	while(aux && comparare(aux->info, cel->info)){
		ant = aux;
		aux = aux->urm;
	}
	//se verifica daca este primul element
	if(ant == NULL){
	  cel->urm = *aL;
	  *aL = cel;
	}
	else{
	 cel->urm = ant->urm;
	 ant->urm = cel;
	}

 return 1;
}

//printarea clasamentului
void clas_print(TLista L , FILE *out)
{
	for( ; L != NULL ; L = L->urm){
		Jucator J = (Jucator)L->info;
		fprintf(out,"%s %d\n",J->nume, J->nrMaini);
	}
}

//eliberarea memoriei din lista creata pentru afisarea clasamentului
void eliberare(ALista L)
{
TLista aux;
while(*L){
	aux = *L;
	*L = aux->urm;
	free(aux);
}
}

void clasament(Sala sala,char mas_in[Max], FILE *out)
{
	Masa Mga=body_cauta_masa(sala, mas_in,out);	
	if (!Mga)return;
	TLista J = Mga->jucatori->urm, lista_noua = NULL;
	for(;J->info != NULL; J = J->urm){
		InsLG(&lista_noua,  J->info);
	}
	fprintf(out,"Clasament %s:\n", Mga->numeMasa);
	clas_print(lista_noua,out);
	eliberare(&lista_noua);
} 




