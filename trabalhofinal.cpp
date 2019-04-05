
// Inclusão de bibliotecas e elementos de pré-processador
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define TAM 33
#define HEADS 3

// Definição e declaração da struct que armazenará os nós
struct node{
    int active;
    int previous;
    char nick[26];
    char city[16];
    int age;
    int next;
};
typedef struct node Node;

// Definição das funções
// Função que imprime o menu da aplicação
void printMenu();
// Função que imprime a lista de status
void printList();
// Função que adiciona nós à lista
int includeNode(Node*, int&, Node*);
// Função que retira nós da lista
int removeNode(Node*, int&, char*);
// Função que apresenta os nós de uma lista
int showList(Node*, int);
// Função que altera o status de um nó
int exchangeStatus(Node*, int&, int&, Node*);
// Função que apresenta os nós disponíveis e as cabeças de lista
void freeNodes(Node*, int*);
// Função que consulta um nó à partir do nick
Node* consultNode(Node*, int, int&, char*);

// Função principal do programa
int main(){
    Node list[TAM];
    int heads[HEADS];

    for(int i = 0; i < TAM; i++)
        list[i].active = 0;
    for(int i = 0; i < HEADS; i++)
        heads[i] = -1;

    while(true){
        char op;

        printMenu();
        fflush(stdin);
        scanf("%c", &op);

        // Tratamento de inclusão na lista
        if((op == 'i') || (op == 'I')){
            char nick[26], city[16];
            int age, ls;

            while(true){
                printList();
                scanf("%d", &ls);

                if((ls == 1) || (ls == 2) || (ls == 3))
                    break;
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            printf("\n Para incluir, digite:\n");

            while(true){
                fflush(stdin);
                printf("  NICK: ");
                gets(nick);

                if(nick[0] != ' ')
                    break;
                printf("\t!-- Informe um nick valido.\n");
                system("pause");
            }

            while(true){
                fflush(stdin);
                printf("  CIDADE: ");
                gets(city);

                if(city[0] != ' ')
                    break;
                printf("\t!-- Informe um nome de cidade valido.\n");
                system("pause");
            }

            while(true){
                fflush(stdin);
                printf("  IDADE: ");
                scanf("%d", &age);

                if((age > 0) && (age < 131))
                    break;
                printf("\t!-- Informe uma idade valida.\n");
                system("pause");
            }

            Node *newNode = (Node*) calloc(1, sizeof(Node));
            strcpy(newNode->nick, nick);
            strcpy(newNode->city, city);
            newNode->age = age;

            int *included = (int*) calloc(1, sizeof(int));
            *included = includeNode(list, heads[ls-1], newNode);
            if(*included != -1)
                printf("\n Incluido no indice: %d.\n", *included);
            else
                printf(" A lista excedeu sua capacidade maxima, para prosseguir eh necessario remover um registro.\n");
            free(newNode);
            free(included);
            system("pause");
        }
        // Tratamento de remoção da lista
        else if((op == 'r') || (op == 'R')){
            int ls, printed;
            char pn;

            while(true){
                printList();
                scanf("%d", &ls);

                if((ls == 1) || (ls == 2) || (ls == 3))
                    break;
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            while(true){
                printf(" Deseja imprimir os registros da lista (s - sim | n - nao): ");
                fflush(stdin);
                scanf("%c", &pn);

                if((pn == 's') || (pn == 'S')){
                    printed = showList(list, heads[ls-1]);
                    break;
                }
                else if((pn == 'n') || (pn == 'N')){
                    if(heads[ls - 1] == -1)
                        printed = false;
                    else
                        printed = true;
                    break;
                }
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            if(!printed)
                printf("\n\t!-- Nao ha registros na lista de status selecionada.\n\n");
            else{
                char tempNick[26];
                int removed;
                printf("\n Informe o nick do registro a ser removido: ");
                fflush(stdin);
                gets(tempNick);

                removed = removeNode(list, heads[ls - 1], tempNick);

                if(!removed)
                    printf("\n\t!-- Nick nao encontrado na lista.\n\n");
                else
                    printf("\n\tRegistro removido com sucesso!\n\n");
            }
            system("pause");

        }
        // Tratamento de exibição da lista
        else if((op == 'm') || (op == 'M')){
            int ls, printed;

            while(true){
                printList();
                scanf("%d", &ls);

                if((ls == 1) || (ls == 2) || (ls == 3))
                    break;
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            printed = showList(list, heads[ls-1]);
            if(!printed)
                printf("\n\t!-- Nao ha registros na lista de status selecionada.\n\n");
            system("pause");
        }
        // Tratamento de troca de status do nó
        else if((op == 't') || (op == 'O')){
            int ls, printed;

            while(true){
                printList();
                scanf("%d", &ls);

                if((ls == 1) || (ls == 2) || (ls == 3))
                    break;
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            while(true){
                char pn;
                printf(" Deseja imprimir os registros da lista (s - sim | n - nao): ");
                fflush(stdin);
                scanf("%c", &pn);

                if((pn == 's') || (pn == 'S')){
                    printed = showList(list, heads[ls-1]);
                    break;
                }
                else if((pn == 'n') || (pn == 'N')){
                    if(heads[ls - 1] == -1)
                        printed = false;
                    else
                        printed = true;
                    break;
                }
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            if(!printed)
                printf("\n\t!-- Nao ha registros na lista de status selecionada.\n\n");
            else{
                char tempNick[26];
                printf("\n Informe o nick do registro que deseja alterar o status: ");
                fflush(stdin);
                gets(tempNick);

                Node* tempNode = (Node*) calloc(1, sizeof(Node));
                int ind = -1, trade, traded;
                tempNode = consultNode(list, heads[ls-1], ind, tempNick);

                if(!strcmp(tempNode->nick, tempNick)){
                    printf("\n\n Selecione uma opcao de troca: ");
                    while(true){
                        switch(ls){
                            case 1:
                                printf("\n\t1: Online -> Offline");
                                printf("\n\t2: Online -> Ignorado");
                            break;
                            case 2:
                                printf("\n\t1: Offline -> Online");
                                printf("\n\t2: Offline -> Ignorado");
                            break;
                            case 3:
                                printf("\n\t1: Ignorado -> Online");
                                printf("\n\t2: Ignorado -> Offline");
                            break;
                        }
                        fflush(stdin);
                        printf("\n > ");
                        scanf("%d", &trade);

                        if((trade == 1) || (trade == 2))
                            break;
                        printf("\n\t!-- Opcao de troca invalida.\n");
                        system("pause");
                    }

                    if((ls == 1) && (trade == 1))
                        traded = exchangeStatus(list, heads[ls-1], heads[trade], tempNode);
                    else if((ls == 1) && (trade == 2))
                        traded = exchangeStatus(list, heads[ls-1], heads[trade], tempNode);
                    else if((ls == 2) && (trade == 1))
                        traded = exchangeStatus(list, heads[ls-1], heads[trade-1], tempNode);
                    else if((ls == 2) && (trade == 2))
                        traded = exchangeStatus(list, heads[ls-1], heads[trade], tempNode);
                    else if((ls == 3) && (trade == 1))
                        traded = exchangeStatus(list, heads[ls-1], heads[trade-1], tempNode);
                    else if((ls == 3) && (trade == 2))
                        traded = exchangeStatus(list, heads[ls-1], heads[trade-1], tempNode);

                    if(traded)
                        printf("\n\tAlteracao de status realizada com sucesso!\n\n");
                    else
                        printf("\n\t!-- Ocorre uma falha durante a alteracao de status.\n\n");
                }
                else
                    printf("\n\t!-- Registro nao encontrado.\n\n");
                system("pause");
                free(tempNode);
            }
        }
        // Tratamento de exibição de nós disponíveis
        else if((op == 'd') || (op == 'M')){
            freeNodes(list, heads);
            system("pause");
        }
        // Tratamento de consulta à um nó da lista
        else if((op == 'c') || (op == 'C')){
            int ls;

            while(true){
                printList();
                scanf("%d", &ls);

                if((ls == 1) || (ls == 2) || (ls == 3))
                    break;
                printf("\t!-- Opcao invalida.\n");
                system("pause");
            }

            char tempNick[26];
            printf(" Digite o nick: ");
            fflush(stdin);
            gets(tempNick);

            Node* tempNode = (Node*) calloc(1, sizeof(Node));
            int ind = -1;
            tempNode = consultNode(list, heads[ls-1], ind, tempNick);

            if(!strcmp(tempNode->nick, tempNick)){
                printf("\n Indice |    Nick    |   Cidade   |  Idade\n");
                printf("   %d    | %s | %s |  %d\n", ind, tempNode->nick, tempNode->city, tempNode->age);
            }
            else
                printf("\n\t!-- Registro nao encontrado.\n\n");
            system("pause");
            free(tempNode);
        }
        // Tratamento de saída de execução
        else if((op == 'f') || (op == 'F'))
            break;
    }

    return 0;
}

void printMenu(){
    system("cls");
    printf("\n +----------------------------------------+\n");
    printf(" |                  MENU                  |\n");
    printf(" +----------------------------------------+\n");
    printf(" | I - Inclusao        | D - Nodos dispo. |\n");
    printf(" | R - Retirada        | C - Consulta     |\n");
    printf(" | M - Mostra Lista    | O - Ordenar      |\n");
    printf(" | T - Troca de Status | F - Fim          |\n");
    printf(" +----------------------------------------+\n");
    printf("   Digite sua opcao: ");
}

void printList(){
    printf("\n Escolha uma opcao:\n");
    printf("\t1 - Online\n");
    printf("\t2 - Offline\n");
    printf("\t3 - Ignorado\n");
    printf("Opcao: ");
}

int includeNode(Node *list, int &head, Node *newNode){
    int included = false, ind = -1;
    if(head == -1){
        int i;
        for(i = 0; i < TAM; i++){
            if(!list[i].active){
                list[i].active = 1;
                strcpy(list[i].nick, newNode->nick);
                strcpy(list[i].city, newNode->city);
                list[i].age = newNode->age;
                list[i].previous = i;
                head = i;
                list[i].next = head;
                included = true;
                ind = i;
                break;
            }
            else
                included = false;
        }
    }
    else if(head != -1){
        int j;
        for(int i = head; ; i = list[i].next){
            if(list[i].next == head){
                for(j = 0; j < TAM; j++){
                    if(!list[j].active){
                        list[j].active = 1;
                        strcpy(list[j].nick, newNode->nick);
                        strcpy(list[j].city, newNode->city);
                        list[j].age = newNode->age;
                        list[j].previous = i;
                        list[i].next = j;
                        list[j].next = head;
                        included = true;
                        ind = j;
                        return ind;
                    }
                    else
                        included = false;
                }
            }
            else
                included = false;
        }
    }
    if(included)
        return ind;
    else
        return -1;
}

int removeNode(Node *list, int &head, char *nick){
    for(int i = head, j = 0; ; i = list[i].next, j++){
        if(!strcmp(list[i].nick, nick)){
            if((i == head) && (list[i].next == head)){
                list[i].active = 0;
                head = -1;
                return 1;
            }
            else if((i == head) && (list[i].next != head)){
                list[i].active = 0;
                head = list[i].next;
                list[list[i].next].previous = head;
                list[list[i].next].next = head;
                return 1;
            }
            else if((i != head) && (list[i].next == head)){
                list[list[i].previous].next = head;
                list[i].active = 0;
                return 1;
            }
            else if((i != head) && (list[i].next != head)){
                list[list[i].previous].next = list[i].next;
                list[list[i].next].previous = list[i].previous;
                list[i].active = 0;
                return 1;
            }
        }
        if((i == head) && (j > 0))
            break;
    }

    return 0;
}

int showList(Node *list, int head){
    if(head == -1)
        return 0;
    for(int i = head, j = 0; ; i = list[i].next, j++){
        if((i == head) && (j > 0))
            break;
        if(i == head)
            printf("\n Indice | elo_A |    Nick    |    Cidade    |  Idade  |  elo_P\n");
        printf("   %d    |   %d   | %s | %s |   %d   |  %d\n", i, list[i].previous, list[i].nick, list[i].city, list[i].age, list[i].next);
        if(list[i].next == head)
            break;
    }
    return 1;
}

int exchangeStatus(Node* list, int& sourceHead, int& destinyHead, Node* realocNode){
    int confirmTrade = -1;
    removeNode(list, sourceHead, realocNode->nick);
    confirmTrade = includeNode(list, destinyHead, realocNode);

    if(!strcmp(list[confirmTrade].nick, realocNode->nick))
        return 1;
    else
        return 0;
}

void freeNodes(Node* list, int* heads){
    int print = false;

    if(heads[0] != -1)
        printf("\n O Nodo Cabeca online encontra-se no indice: %d\n", heads[0]);
    else
        printf("\n Nao ha registros da lista online.\n");
    if(heads[1] != -1)
        printf(" O Nodo Cabeca offline encontra-se no indice: %d\n", heads[1]);
    else
        printf(" Nao ha registros da lista offline.\n");
    if(heads[2] != -1)
        printf(" O Nodo Cabeca ignorado encontra-se no indice: %d\n", heads[2]);
    else
        printf(" Nao ha registros da lista ignorado.\n");

    for(int i = 0; i < TAM; i++)
        if(list[i].active == 0){
            print = true;
            break;
        }

    if(print){
        printf("\n Os nodos vazios sao:\n\t");
        for(int i = 0, j = 0; i < TAM; i++){
            if((list[i].active == 0) && (i < 10)){
                printf("[%d]  ", i);
                j++;
            }
            else if((list[i].active == 0) && (i >= 10)){
                printf("[%d] ", i);
                j++;
            }
            if((j == 11) || (j == 22))
                printf("\n\t");
        }
        printf("\n");
    }
    else
        printf("\t!-- Nao ha nodos vazios.\n\n");
}

Node* consultNode(Node* list, int head, int &ind, char* tempNick){
    Node* returnNode = (Node*) calloc(1, sizeof(Node));

    if(head != -1)
        for(int i = head, j = 0; ; i = list[i].next, j++){
            if((i == head) && j > 0)
                break;
            if(!strcmp(tempNick, list[i].nick)){
                returnNode->active = list[i].active;
                returnNode->age = list[i].age;
                strcpy(returnNode->city, list[i].city);
                returnNode->next = list[i].next;
                strcpy(returnNode->nick, list[i].nick);
                returnNode->previous = list[i].previous;
                ind = i;
                break;
            }
        }

    return returnNode;
}
