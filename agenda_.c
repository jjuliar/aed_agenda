#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define nome (sizeof(char) * 30)
#define idade (sizeof(int))
#define telefone (sizeof(long int))
#define ant (sizeof(char) * 30 + sizeof(int) + sizeof(long int))
#define prox (sizeof(char) * 30 + sizeof(int) + sizeof(long int) + sizeof(void **))
#define pri (sizeof(int) + sizeof(void **))
#define ult (sizeof(int))

void inserirPessoa(void *pBuffer, void *pAddress){
    system("clear");
    void *nodo = (void *)malloc(nome + idade + telefone + sizeof(void **) * 2);
    void *temp;

    if(!nodo){
        printf("Erro de memória!\n");
        return -1;
    }

    printf("\nInsira o nome: "); 
    scanf("%s", (char *)nodo);
    printf("Insira a idade: "); 
    scanf("%d", &*(int *)(nodo + nome));
    printf("Insira o telefone: "); 
    scanf("%ld", &*(long int *)(nodo + nome + idade));

    *(void **)(nodo + ant) = NULL; 
    *(void **)(nodo + prox) = NULL; 

    if(*(int *)pAddress == 0){
        *(void **)(pAddress + ult) = nodo;
        *(void **)(pAddress + pri) = nodo;
        *(int *)pAddress += 1; 

        return;
    }

    pBuffer = *(void **)(pAddress + pri); 

    do{
        if(strcmp((char *)nodo, (char *)pBuffer) < 0){
            
            *(void **)(nodo + ant) = *(void **)(pBuffer + ant); 
            *(void **)(nodo + prox) = pBuffer;
            if(*(void **)(pBuffer + ant) != NULL && *(int *)pAddress > 1){
                temp = *(void **)(pBuffer + ant);
                *(void **)(temp + prox) = nodo;
            }

            *(void **)(pBuffer + ant) = nodo; 
            
            if(*(void **)(nodo + ant) == NULL){
                *(void **)(pAddress + pri) = nodo; 
            }
            *(int *)pAddress += 1; 
            return;
        }

       
        if(*(void **)(pBuffer + prox) == NULL){
            break;
        }

        pBuffer = *(void **)(pBuffer + prox); 
    }while(pBuffer != NULL);

    *(void **)(nodo + ant) = pBuffer;
    *(void **)(pBuffer + prox) = nodo;
    *(void **)(pAddress + ult) = nodo;

    *(int *)pAddress += 1; 
    return;
}

void procurar(void *pBuffer, void *pAddress){
    system("clear");
    char *nomeBusca = calloc(1, nome);

    printf("Insira nome da pessoa: ");
    scanf("%s", nomeBusca);

    pBuffer = *(void **)(pAddress + pri); 

    while(pBuffer != NULL){
        if(strcmp((char *)pBuffer, (char *)nomeBusca) == 0){
            printf("\n\n%s, ", (char*)pBuffer);
            printf("%d anos, ", *(int*)(pBuffer + nome));
            printf("contato: %d \n\n",  *(long int*)(pBuffer + nome + idade));
            free(nomeBusca);
            continuar();
            return;
        } 

        pBuffer = *(void **)(pBuffer + prox);
    }
    
    printf("\n\nPessoa não encontrada.\n");
    continuar();    
    free(nomeBusca);
}

void listarTodos(void *pBuffer, void *pAddress){
    system("clear");
    pBuffer = *(void **)(pAddress + pri);

    while(pBuffer != NULL) {
        printf("\n%s, ", (char*)pBuffer);
        printf("%d anos, ", *(int*)(pBuffer + nome));
        printf("contato: %d \n",  *(long int*)(pBuffer + nome + idade));
        pBuffer = *(void**)(pBuffer + prox);
    }

    if(*(int *)pAddress == 0){
        printf("Nenhuma pessoa cadastrada na agenda.");
    }

    continuar();
}

void removerPessoa(void *pBuffer, void *pAddress){
    system("clear");
    void *temp;
    char *nomeExcluir = calloc(1, nome);

    printf("Insira nome da pessoa para remover: ");
    scanf("%s", nomeExcluir);    

    pBuffer = *(void **)(pAddress + pri); 

    while (pBuffer != NULL){
        if(strcmp((char *)pBuffer, (char *)nomeExcluir) == 0){
            if(*(void **)(pBuffer + prox) != NULL){
                temp = *(void **)(pBuffer + prox); 
                *(void **)(temp + ant) = *(void **)(pBuffer + ant);
            }else{
                *(void **)(pAddress + ult) = *(void **)(pBuffer + ant);
            }

            if(*(void **)(pBuffer + ant) != NULL){
                temp = *(void **)(pBuffer + ant); 
                *(void **)(temp + prox) = *(void **)(pBuffer + prox); 
            }else{
                *(void **)(pAddress + pri) = *(void **)(pBuffer + prox);
            }
   
            *(int *)pAddress -= 1;
            free(pBuffer);
            free(nomeExcluir);
            printf("\n\nPessoa removida com sucesso!\n");
            continuar();
            return;
        } 

        pBuffer = *(void **)(pBuffer + prox);
    }

    free(nomeExcluir);
    printf("\n\nPessoa não encontrada.\n");
    continuar();
    return;
}

void removerTudo(void *pBuffer, void *pAddress){
    system("clear");
    pBuffer = *(void **)(pAddress + pri); 
    void *temp;

    while (pBuffer != NULL){
        if(*(void **)(pBuffer + prox) != NULL){
            temp = *(void **)(pBuffer + prox);
            free(pBuffer);
            pBuffer = temp;
        }else{
            free(pBuffer);
            return;
        }
    }

    return;
}

void continuar(){
  printf("\nPressione enter para continuar....");
  while(getchar()!='\n');
  getchar();
}

int main(){
    void * pBuffer;
    void * pAddress;
    pBuffer = (void *)malloc(sizeof(int) + sizeof(void **) + sizeof(int));
    pAddress = (void *)malloc((sizeof(int) + sizeof(void **) * 2));
    int * response = pBuffer + sizeof(int);

    *(void **)pBuffer = NULL; 
    *(int *)pAddress = 0; 
    *(void **)(pAddress + sizeof(int)) = NULL; 
    *(void **)(pAddress + pri) = NULL; 
    

    if(!pBuffer || !pAddress){
        printf("Erro de memória!\n");
        return -1;
    }

    printf("\t\tOlá!");

    while( *response != 5){
        system("clear");
        printf("\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        printf("\n\n\tSelecione uma opção:\n\n\t1. Inserir uma nova pessoa na agenda\n\t2. Procurar pessoas\n\t3. Listar todas as pessoas\n\t4. Remover pessoas\n\t5. Sair \n\n");
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        scanf("%d", &*response);

        switch (*response)
        {
            case 1:
                inserirPessoa(pBuffer, pAddress);
            break;
            
            case 2:
                procurar(pBuffer, pAddress);
            break;

            case 3:
                listarTodos(pBuffer, pAddress);
            break;

            case 4:
                removerPessoa(pBuffer, pAddress);
            break;

            case 5:
                removerTudo(pBuffer, pAddress);
                free(pAddress);
                printf("\nVolte sempre! :)\n");
                exit(0);
            break;

            default:
            break;
        }
    }
}

