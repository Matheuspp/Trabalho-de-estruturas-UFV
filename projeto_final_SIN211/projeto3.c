#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

/*
    Matheus Vieira   6002
    Everton Henrique 6000

*/


// caracteristicas do evento
typedef struct sEvento{
    char nome[41];
    int data;
}Evento;

// definicao de celula
typedef struct sCell{
    Evento info;
    struct sCell *prox;

}Celula;

// poteiros apontando para o inicio e fim
typedef struct sFila{
    Celula *inicio;
    Celula *fim;
}Fila;


// inicializando a fila
void inicializar_fila(Fila *F){
    F->fim = NULL;
    F->inicio = NULL;
}

//inicializando a lista auxiliar
void inicializar_lista(Celula **lista){
	(*lista) = NULL;
}

// criando novo no
Celula* getNode(){
    Celula *nova = (Celula*)malloc(sizeof(Celula));
    return nova;
}

// verificar se a lista auxiliar está vazia ou não
int empty(Celula *lista){
	if((lista) == NULL){
		return 1;
	}
	return 0;
}

// verificando se a fila esta vazia
int vazia(Fila *F){
    if(F->fim == NULL){
        return 1;
    }
    return 0;
}

Celula* pesquisar(Celula *lista, char *str){
    Celula *aux;
    if(!empty(lista)){
        aux = (lista);
        while(aux != NULL){
            if(strcmp(aux->info.nome, str) == 0){
                return aux;
            }
            aux = aux->prox;
        }
    }
    return NULL;
}

// a funcao getDay() vai pegar a data do sistema operacional e converter para int

int convercao(){
    char aux[9];
    int data;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);

    aux[0] = '2';
    aux[1] = '0';
    aux[2] = s[6];
    aux[3] = s[7];
    aux[4] = s[0];
    aux[5] = s[1];
    aux[6] = s[3];
    aux[7] = s[4];

    data = atoi(aux);

    return data;


}
// funcao para validar os dados do usuario
int validar(int data, Celula *lista){
    Celula *aux = lista;
    if(convercao() >= data){
        return 0;
    }
    if(aux != NULL){
        while(aux != NULL){
            if(aux->info.data == data){
                return 0;
            }
            aux = aux->prox;
        }
    }
    return 1;

}
int validar_nome(char nome[], Celula *lista){
    Celula *aux = lista;

    if(aux != NULL){
        while(aux!=NULL){
            if(strcmp(nome,(aux->info).nome)==0){
      			return 0;
			}
			aux=aux->prox;
		}
    }
    return 1;

}

// inserir o evento na lista
int ins_lista(Celula **lista, Evento elem){
    Celula *no;
    Celula *aux = (*lista);
    Celula *aux2=NULL;
    no = getNode();
    if(no != NULL){
        no->info = elem;
        no->prox = NULL;
        while((aux!=NULL)&&((aux->info).data<=(no->info).data)){
        	aux2=aux;
			aux=aux->prox;
        }
        if(aux==(*lista)){
        	no->prox=(*lista);
        	(*lista)=no;
		}else{
			aux2->prox=no;
			no->prox=aux;
		}
        return 1;
    }else{
        printf("Erro de alocação de nó\n");
        return 0;
    }
}

// desenfileirar a fila da maneira classica jogando os dados na lista auxiliar
int desenfileirar_2(Fila *F){
	Celula *q;
	if(vazia(F)){
        return 0;
	}else{
        while(F->inicio != NULL){
            q = (F->inicio)->prox;
            free(F->inicio);
            F->inicio = q;
            if(F->inicio == NULL){
                F->fim=NULL;
            }
        }

	}
	return 1;


}

int desenfileirar3(Fila *F){
	Celula *q;

	while(F->inicio!=NULL){
		q=(F->inicio)->prox;
		free(F->inicio);
		F->inicio=q;
	}
	if(F->inicio==NULL){
		F->fim==NULL;
		return 1;
	}
	return 0;
}

int desenfileirar(Fila *F){
   Celula *q;
   int i = 1;
	if(vazia(F)){
        return 0;
	}else{
        while(F->inicio != NULL){
            q = (F->inicio)->prox;

            printf("%d-%s\n", i++, (F->inicio)->info.nome);

            free(F->inicio);
            F->inicio = q;
            if(F->inicio == NULL){
                F->fim=NULL;
            }
        }

	}
	return 1;


}




// comparar os eventos e reoordenar a fila de acordo com a prioridade


//joga os eventos da lista auxiliar na fila ordenadamente de acordo com a prioridade
int enfileirar(Fila *F, Evento elem){
    Celula *q = getNode();
	if(q != NULL){
        q->info = elem;
        q->prox = NULL;
        if(vazia(F)){
            F->fim = q;
            F->inicio = q;
        }else{
            (F->fim)->prox = q;
             F->fim = q ;
        }

        return 1;
    }else{
        printf("erro!\n");
        return 0;
    }
}

int cadastrar(Fila *F, Celula **lista){
                Celula *aux;
                Evento elemento;
                int val;

				do{
	                printf("Digite o nome do evento\n");
	                scanf("%s",&elemento.nome);
	                fflush(stdin);
	                val= validar_nome(elemento.nome,*lista);
	                if(val==0){
	                	printf("Evento já existente, favor digitar novamente\n");
					}
				}while(val==0);

            //verificação se data colocada pelo usuário ja passou
                do{
                    printf("Digite a data do evento (aaaa-mm-dd)\n");
                    scanf("%d",&elemento.data);
                    fflush(stdin);
                    val = validar(elemento.data, *lista);
                    if(val == 0){
                        printf("Erro! Data inválida.\n");
                    }
                }while(val == 0);

                ins_lista(&(*lista), elemento);
                aux = (*lista);
                desenfileirar_2(F);
                while(aux != NULL){
                    enfileirar(F, aux->info);
                    aux = aux->prox;
                }
}

int cancelar_evento(Fila *F, Celula **lista){
    Celula *p;
    Celula *aux;
    char procura[20];
    printf("INSIRA UM EVENTO PARA CANCELAR\n");
    scanf("%s",&procura);
    p = pesquisar((*lista), procura);
    if(p != NULL){
        aux = (*lista);
        if(aux == p){
            (*lista) = aux->prox;
            free(aux);
        }else{
            while(aux->prox != p){
                aux = aux->prox;
            }

            aux->prox = p->prox;
            free(p);
        }
        printf("CANCELADO!\n");
    }else{
        printf("EVENTO NÃO CADASTRADO!\n");
        return 0;
    }

    desenfileirar_2(F);
    aux = (*lista);
    while(aux != NULL){
        enfileirar(F, aux->info);
        aux = aux->prox;
    }

    return 1;

}


int remarcar_evento(Fila *F, Celula **lista){
    Celula *p;
    Celula *aux;
    Celula *ant;
    Evento elem;
    int data;
    char procura[20];

    printf("INSIRA UM EVENTO CADASTRADO\n");
    scanf("%s",&procura);
    printf("DIGITE NOVA DATA (aaaa-mm-dd)\n");
    scanf("%d",&data);
    fflush(stdin);
    while(validar(data, *lista) == 0){
        printf("DATA INVÁLIDA! INSIRA OUTRA (aaaa-mm-dd)\n");
        scanf("%d",&data);
    }

    p = pesquisar((*lista), procura);
    if(p != NULL){
        elem.data = data;
        strcpy(elem.nome, procura);

        aux = (*lista);
        if(aux == p){
            (*lista) = aux->prox;
            free(aux);
        }else{
            while(aux->prox != p){
                aux = aux->prox;
            }

            aux->prox = p->prox;
            free(p);

        }
        printf("EVENTO REMARCADO!\n");
        ins_lista(&(*lista), elem);
        desenfileirar_2(F);
        aux = (*lista);
        while(aux != NULL){
            enfileirar(F, aux->info);
            aux = aux->prox;
        }

        return 1;




    }else{
        printf("EVENTO NÃO ENCONTRADO!\n");
        return 0;
    }
}

int mostrar_eventos(Fila *F, Celula **lista){
    Celula *aux;
    if(vazia(F)){
        return 0;
    }
    desenfileirar(F);
    F->fim = NULL;
    F->inicio = NULL;
    aux = (*lista);
    while(aux != NULL){
        enfileirar(F, aux->info);
        aux = aux->prox;
    }
    return 1;

}


// função recursiva para contar numero de eventos
int num_eventos(Fila *F, Celula **lista){
    Fila *q = F;
    Celula *p;
    if(F->inicio != NULL){
        p = F->inicio;
        F->inicio = p->prox;
        free(p);
        return 1 + num_eventos(q, &(*lista));
    }
    F->fim = NULL;
    p = (*lista);
    while(p != NULL){
        enfileirar(F, p->info);
        p = p->prox;
    }
    return 0;

}

int remover_tudo(Celula **lista){
	Celula *q;
	Celula *aux = (*lista);
	if(empty((*lista))){
		return 1;
	}
	while(aux != NULL){
        q = aux;
		free(q);
		aux = aux->prox;
	}

	if((aux) == NULL){
        (*lista) = NULL;
		return 1;
	}
	return 0;

}



// logo marca que ficara no cabecario da interface
void logo_marca(){
    printf("------------------------------------------------------------------------------\n");
    printf("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");
    printf("                                                   #                          \n");
    printf("                                                  #                           \n");
	printf("##     ##      ###       ######      ###      ##     ##  ######       ###     \n");
    printf("###   ###     ## ##     ##          ## ##     ##     ##  ##   ##     ## ##    \n");
    printf("## # # ##    ##   ##    ##         ##   ##    ##     ##  #######    ##   ##   \n");
    printf("##  #  ##   #########   ##        #########   ##     ##  ##   ##   #########  \n");
    printf("##     ##  ##       ##   ######  ##       ##    #####    ######   ##       ## \n");
    printf("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");
    printf("------------------------------------------------------------------------------\n");
}

int menu(){

    system("cls");
    logo_marca();

    printf(" [1] PARA CADASTRAR EVENTO \n");
    printf(" [2] PARA CANCELAR EVENTO   \n");
    printf(" [3] PARA REMARCAR EVENTO   \n");
    printf(" [4] PARA MOSTRAR EVENTOS  \n");
    printf(" [5] PARA QUANTIDADE DE EVENTOS \n");
    printf(" [0] PARA SAIR\n");
    printf("------------------------------------------------------------------------------\n");
    printf("DIGITE A OPÇÃO DESEJADA\n");
    return 0;
}

int main(){
    Fila ptr;
    Celula *lista;
    int val;
    int escolha;
	int final1;
	int final2;

    setlocale(2,"portuguese"); // definindo idioma nativo PT-BR
    inicializar_fila(&ptr);
	inicializar_lista(&lista);
//---------------------------------------------------------------------------
    menu();
    scanf("%d",&escolha);
	while(escolha != 0){
        switch(escolha){
            case 1:
                //--------------------------------------------------------------
                cadastrar(&ptr, &lista);
                break;
            case 2:
                cancelar_evento(&ptr, &lista);
                break;
            case 3:
                remarcar_evento(&ptr, &lista);
                break;
            case 4:
                mostrar_eventos(&ptr, &lista);
                break;
            case 5:
                val = num_eventos(&ptr, &lista);
                if(val != 0){
                    printf("NÚMERO DE EVENTOS CADASTRADOS: %d", val);
                }
                break;

	}
        if(escolha == 1 ){
            printf("\nOPERAÇÃO REALIZADA COM SUCESSO!\n");
        }
        Sleep(2000);
        menu();
        scanf("%d",&escolha);
    }
//------------------------------------------------------------------------
      final1 = desenfileirar3(&ptr);
      final2 = remover_tudo(&lista);
      if((final1==1)&&(final2==1)){
            printf("Todas as estruturas utililzadas foram desalocadas da memória\n");
       }
    return 0;
}
