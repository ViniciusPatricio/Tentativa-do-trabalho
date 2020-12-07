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
   if(raiz->filhos[0]==NULL) return true;
   return false;
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

int buscaArvore( ArvMW *raiz, int chave, int nivel){
    int i = 0;
    if(raiz==NULL){
        return 0;
    }else{
        while( i < raiz->num_chaves && chave > raiz->chaves[i]) i++;
        if(raiz->chaves[i]==chave){
            printf("chave %d estava no nivel %d da arvore e na posicao %d do no\n",chave,nivel,i);
            return 1;
        }
        if(chave<raiz->chaves[i]){
            buscaArvore(raiz->filhos[i],chave,nivel+1);
        }else{
            buscaArvore(raiz->filhos[i+1],chave,nivel+1);
        }
    }
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



// ***************************** FUNÇÕES DE REMOÇÃO *****************************//


int maiorChave(ArvMW *raiz){
    if(raizNaoTemFilhos(raiz)==true){
        return raiz->chaves[raiz->num_chaves-1];
    }else{
        maiorChave(raiz->filhos[raiz->num_chaves]);
    }
}
int menorChave(ArvMW *raiz){
    if(raizNaoTemFilhos(raiz)==true) return raiz->chaves[0];
    else return menorChave(raiz->filhos[0]);
    
}

void removerChave(ArvMW *raiz, int posicao){
    int k;
    for(k=posicao;k<raiz->num_chaves;k++){
        raiz->chaves[k] = raiz->chaves[k+1];
        raiz->filhos[k+1] = raiz->filhos[k+2];
    }
    raiz->num_chaves--;
}

// essa função irá fazer a junção de dois filhos, fazendo a redução do numero de chaves do pai

void join(ArvMW *filho_esq, ArvMW *filho_dir, int chave){
    filho_esq->chaves[filho_esq->num_chaves] = chave;
    for(int i = 0; i<filho_dir->num_chaves;i++){
        filho_esq->chaves[filho_esq->num_chaves+i+1]=filho_dir->chaves[i];
        filho_esq->filhos[filho_esq->num_chaves+1+i]=filho_dir->filhos[i];
    }
    filho_esq->filhos[filho_esq->num_chaves+filho_dir->num_chaves+1] = filho_dir->filhos[filho_dir->num_chaves];
    filho_esq->num_chaves=(filho_esq->num_chaves)+(filho_dir->num_chaves+1);
    free(filho_dir);

}


void remocao(ArvMW *raiz, int chave){
    int posicao = buscaBinaria(raiz, chave);

    if(raizNaoTemFilhos(raiz)==true){ // verifica se o no é folha
        if(raiz->chaves[posicao]==chave){
            removerChave(raiz,posicao);
        }
    }

    else{  // caso o no tenha filhos

        // caso 2, a remoção irá ocorrer no nó mais interno
        if(raiz->chaves[posicao]==chave){
            ArvMW *filho_esq=raiz->filhos[posicao];
            ArvMW *filho_dir=raiz->filhos[posicao+1];

            if(filho_esq->num_chaves>MIN){
                int predecessor=maiorChave(filho_esq);
                remocao(filho_esq, predecessor);
                raiz->chaves[posicao]=predecessor;
            }
            else if(filho_dir->num_chaves>MIN){
                int sucessor=menorChave(filho_dir);
                remocao(filho_dir, sucessor);
                raiz->chaves[posicao]=sucessor;
            }
            else{
                join(filho_esq,filho_dir,chave);
                for(int i=posicao+1;i<raiz->num_chaves;i++){
                    raiz->chaves[i-1]=raiz->chaves[i];
                    raiz->filhos[i]=raiz->filhos[i+1];
                }
                raiz->filhos[raiz->num_chaves]=NULL;
                raiz->num_chaves--;
                remocao(filho_esq,chave);
            }
        // caso a chave nao esta no nó
        }else{
            ArvMW *auxiliar = raiz->filhos[posicao];
            
            if(auxiliar->num_chaves==MIN){
                ArvMW *esq = NULL;
                ArvMW *dir = NULL;

                if(posicao > 0) esq = raiz->filhos[posicao-1];

                if(posicao<MAX) dir=raiz->filhos[posicao+1];

                if(esq != NULL && esq->num_chaves>MIN){
                    // desce uma chave do no para auxiliar e sobe uma
                    int chave_desce =  raiz->chaves[posicao-1];
                    int chave_sobe = esq->chaves[posicao];
                    ArvMW *filho_move =esq->filhos[esq->num_chaves];
                    // move a chaves e filhos de auxliar para direita
                    auxiliar->filhos[auxiliar->num_chaves+1]=auxiliar->filhos[auxiliar->num_chaves];
                    for(int i=auxiliar->num_chaves;i>0;i--){
                        auxiliar->chaves[i]=auxiliar->chaves[i-1];
                        auxiliar->filhos[i]=auxiliar->filhos[i-1];
                    }
                    auxiliar->chaves[0]=chave_desce;
                    auxiliar->filhos[0]=filho_move;
                    auxiliar->num_chaves++;
                    // sobe a chave da esquerda
                    raiz->chaves[posicao-1]=chave_sobe;
                    esq->chaves[esq->num_chaves-1]=0;
                    esq->filhos[esq->num_chaves]=NULL;
                    esq->num_chaves--;
                }
                else if(dir!=NULL && dir->num_chaves>MIN){
                    //desce uma chave do no para auxiliar e sobe uma
                int chave_desce=raiz->chaves[posicao];
                int chave_sobe=dir->chaves[0];
                ArvMW *filho_move=dir->filhos[0];

                //desce chave na ultima posicao de auxiliar

                auxiliar->chaves[auxiliar->num_chaves]=chave_desce;
                auxiliar->filhos[auxiliar->num_chaves+1]=filho_move;
                auxiliar->num_chaves++;

                //sobe a chave da direita
                raiz->chaves[posicao]=chave_sobe;

                //desloca chaves da direita
                for(int i=0;i<dir->num_chaves-1;i++){
                    dir->chaves[i]=dir->chaves[i+1];
                    dir->filhos[i]=dir->filhos[i+1];
                }
                dir->filhos[dir->num_chaves-1]=dir->filhos[dir->num_chaves];
                dir->filhos[dir->num_chaves]=NULL;
                dir->num_chaves--;

                }else{ // faz a junção (join/merge)
                    if(esq!=NULL){
                        join(esq,auxiliar,raiz->chaves[posicao-1]);
                        //shit
                        for(int i = posicao; i <raiz->num_chaves;i++){
                            raiz->chaves[i-1]=raiz->chaves[i];
                            raiz->filhos[i]=raiz->filhos[i+1];
                        }
                        raiz->filhos[raiz->num_chaves]=NULL;
                        raiz->num_chaves--;
                        auxiliar=esq;

                    }else{
                        join(auxiliar,dir,raiz->chaves[posicao]);
                        for(int i = posicao+1;i<raiz->num_chaves;i++){
                            raiz->chaves[i-1]=raiz->chaves[i];
                            raiz->filhos[i]=raiz->filhos[i+1];
                        }
                        raiz->filhos[raiz->num_chaves]=NULL;
                        raiz->num_chaves--;
                    }

                }

                
            }
           remocao(auxiliar,chave); 
        }

    }

}

void removerArvMW(ArvMW **raiz, int valor){
    remocao(*raiz, valor);
    if((*raiz)->num_chaves==0) *raiz =(*raiz)->filhos[0];
}





int main(){
    srand(time(NULL)); // colocado para não repetir a sequência dos números aleatórios
    ArvMW *raiz=criarArv();

    for(int i = 0; i<300; i++){   // inserindo 300 numeros aleatórios de 0 a 9000
        int num = rand()%1000;
        inserirArvore(&raiz,num);
    }
    imprimirArvMW(raiz, 0);
    int chave;
    int *pos;

    for(int i = 0; i<80; i++){ //gerando 80 numeros aleatórios e verificando se eles estão na árvore 2-3-4 
        int num = rand()%1000;
        buscaArvore(raiz,num,0) == true ? removerArvMW(&raiz,num) : printf("chave %d nao esta na arvore\n",num);
    }
    imprimirArvMW(raiz,0);
    return 0;
}