/*
Al: VINICIUS PATRICIO MEDEIROS DA SILVA
Nº: 21951799
Curso: EGENHARIA DA COMPUTAÇÃO
*/

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "float.h"


#define true 1
#define false 0
#define MAX 3   // vai ter no máximo 3 chaves cada nó da árvore
#define MIN 1   // vai ter no mínimo 1 chave cada nó da árvore

// ***************************** ESTRUTURA DA ÁRVORE E SUA CRIAÇÃO *****************************//

// estrutura de cada nó multiway 2-3-4
typedef struct no ArvMW; 
struct no{
    int num_chaves;  // numero de chaves que nó tem
    int chaves[MAX]; // vetor que armazena as chaves 
    ArvMW *filhos[MAX+1];  // vetor que guarda os endereços dos filhos
};

// a funçao criarArv irá alocar espaço na memória para estrutura da árvore
ArvMW *criarArv(){  
    ArvMW *raiz = (ArvMW*)malloc(sizeof(ArvMW));
    raiz->num_chaves = 0;
    
    for(int i = 0; i<=MAX;i++){  // como é um nova raiz/nó, então ele não possui filhos
        raiz->filhos[i] = NULL;
    }
    return raiz;
}

// ***************************** FUNÇÕES AUXILIARES *****************************//


// Função buscaBinaria irá dizer onde o elemento irá ser inserido caso ele não esteja, caso esteja saberemos sua posição
int buscaBinaria(ArvMW *raiz, int entrada){
    int inicio, meio, fim;
    if(raiz->num_chaves == 0){  // caso ele não tenha elemento, ele vai ser inserido no inicio
        return 0;
    }
    inicio = 0;
    fim = raiz->num_chaves - 1;

    while(inicio <= fim)
    {
        meio = (inicio + fim)/2;
        if(entrada == raiz->chaves[meio]) return meio;
        else if(entrada<raiz->chaves[meio]){
          fim = meio - 1;
        }
        else inicio = meio + 1;       
    }
    return inicio;
}

// verifica se nó/raiz possui filhos
int raizNaoTemFilhos(ArvMW *raiz){
   if(raiz->filhos[0]==NULL) return 1;
   return 0;
}

void imprimirArvMW(ArvMW *raiz, int nivel){
    if(raiz==NULL) return;
    for(int i = 0; i < raiz->num_chaves;i++){
        imprimirArvMW(raiz->filhos[i],nivel +1);
        printf("%d|",nivel);
        printf("%d|%d ",i,raiz->chaves[i]);
        printf("\n");
    }
    
    imprimirArvMW(raiz->filhos[raiz->num_chaves],nivel+1);
}


// ***************************** FUNÇÕES DE INSERÇÃO *****************************//

/* 
caso ocarro a inserção e o numero de chaves está no máximo, então é necessário elevar o 
numero do meio do nó e fazer a divisão em dois novos filhos;
*/
void split(ArvMW *raiz, int indice){ // indice mostra em que posição vai ocorrer o split
    ArvMW *filho1 = criarArv();
    ArvMW *filho2 = raiz->filhos[indice];

    filho1->num_chaves = MIN;

    for(int i = 0; i < MIN; i++){
        filho1->chaves[i] = filho2->chaves[MIN+i+1];
        filho1->filhos[i] = filho2->filhos[MIN+i+1];
    }
    filho1->filhos[MIN] = filho2->filhos[MAX];
    
    for(int i = raiz->num_chaves; i>indice;i--){
        raiz->chaves[i] = raiz->chaves[i-1];
        raiz->filhos[i+1] = raiz->filhos[i];
    }

    raiz->filhos[indice+1] = filho1;
    raiz->chaves[indice] = filho2->chaves[MIN];
    raiz->num_chaves++;
    filho2->num_chaves = MIN;
}

void inserirArvoreParcialmente(ArvMW *raiz, int chave_nova){
    int num = raiz->num_chaves;
    if(raizNaoTemFilhos(raiz)){ // caso ele não tenha filhos então devemos inserir a nova chave na raiz/nó
        while( (num>0) && (raiz->chaves[num-1]>chave_nova)){
            raiz->chaves[num] = raiz->chaves[num-1];
            num--;
        }
        raiz->chaves[num] = chave_nova;
        raiz->num_chaves++;
    }else{
        int indice = buscaBinaria(raiz,chave_nova); // buscará em filho a chave vai ter que ser inserida
        if(raiz->filhos[indice]->num_chaves == MAX){ // caso a o filho da raiz onde o numero deve ser inserido estiver cheio
            split(raiz,indice);
            if(chave_nova > raiz->chaves[indice]) indice++; // caso o numero que subiu para o nó pai seja o maior chave do pai
        }
        inserirArvoreParcialmente(raiz->filhos[indice],chave_nova);
    }
}

void inserirArvore(ArvMW **raiz, int chave_nova){
    if((*raiz)->num_chaves == MAX){
        ArvMW *aux = criarArv();
        aux->filhos[0] = *raiz;
        *raiz = aux;
        
        split(aux,0);
        inserirArvoreParcialmente(aux, chave_nova);
    }else{
        inserirArvoreParcialmente(*raiz,chave_nova);
    }
}


int main(){
  srand(time(NULL)); // colocado para não repetir a sequência dos números aleatórios
  ArvMW *raiz=criarArv();

  for(int i = 0; i<300; i++){
      int num = rand()%1000;
      printf("%d ",num);
      inserirArvore(&raiz,num);
    }
  imprimirArvMW(raiz, 0);
  

    return 0;
}