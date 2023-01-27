/*
Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

#define MAX_LINE 1024
#define MAX_STRING 64
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _polinom{
    int koeficijent, eksponent;
    struct _polinom* next;
}polinom;

polinom* sum(polinom* headptr1, polinom* headptr2);
polinom* multiply(polinom* headptr1, polinom* headptr2); //TODO
int importFile(char* filename, polinom* headptr1, polinom* headptr2);
void printPolinom(polinom* headptr);
void printNode(polinom* node);
void removeZeroCoefficients(polinom* headptr);
void deleteNext(polinom* before);
polinom* createNode(int eksponent, int koeficijent);
int sortedMerge(polinom* to_merge, polinom* headptr);
void mergeAfter(polinom* to_merge, polinom* before);
void freePolinom(polinom* headptr);

int main(){
    polinom head1 = {.eksponent = 0, .koeficijent = 0, .next = NULL};
    polinom head2 = {.eksponent = 0, .koeficijent = 0, .next = NULL};
    polinom sum_head = {.eksponent = 0, .koeficijent = 0, .next = NULL};
    polinom multiply_head = {.eksponent = 0, .koeficijent = 0, .next = NULL};
    char* filename = "polinomi.txt";

    if(importFile(filename, &head1, &head2) == EXIT_FAILURE){
        freePolinom(&head1);
        freePolinom(&head2);
        return EXIT_FAILURE;
    }
    if(!head1.next || !head2.next){
        printf("Jedan ili vise unesenih polinoma su prazni.\n");
        return EXIT_FAILURE;
    }
    removeZeroCoefficients(&head1);
    removeZeroCoefficients(&head2);
    
    sum_head.next = sum(&head1, &head2);
    if(!sum_head.next){
        freePolinom(&head1);
        freePolinom(&head2);
        return EXIT_FAILURE;
    }
    removeZeroCoefficients(&sum_head);
    
    multiply_head.next = multiply(&head1, &head2);
    if(!multiply_head.next){
        freePolinom(&head1);
        freePolinom(&head2);
        return EXIT_FAILURE;
    }
    removeZeroCoefficients(&multiply_head);
    
    printf("Uvezeni polinomi glase:\n");
    printPolinom(&head1);
    printPolinom(&head2);
    printf("Rezultat zbrajanja glasi:\n");
    printPolinom(&sum_head);
    printf("Rezultat mnozenja glasi:\n");
    printPolinom(&multiply_head);


    freePolinom(&head1);
    freePolinom(&head2);
    freePolinom(&sum_head);
    freePolinom(&multiply_head);
    return EXIT_SUCCESS;
}

polinom* createNode(int eksponent, int koeficijent){
    polinom* new_node = NULL;

    new_node = (polinom*)malloc(sizeof(polinom));
    if(!new_node){
        return NULL;
    }
    new_node->eksponent = eksponent;
    new_node->koeficijent = koeficijent;
    new_node->next = NULL;

    return new_node;
}

int sortedMerge(polinom* to_merge, polinom* headptr){
    polinom* temp = headptr;

    if(!to_merge)   return EXIT_FAILURE;

    while(temp->next){
        if(temp->next->eksponent <= to_merge->eksponent){
            break;
        }
        temp = temp->next;
    }
    mergeAfter(to_merge, temp);
    return EXIT_SUCCESS;
}

void mergeAfter(polinom* to_merge, polinom* before){
    if(before->next){
        if(before->next->eksponent == to_merge->eksponent){
            before->next->koeficijent += to_merge->koeficijent;
            free(to_merge);
            return;
        }
    }

    to_merge->next = before->next;
    before->next = to_merge;
}

int importFile(char* filename, polinom* headptr1, polinom* headptr2){
    FILE* file = NULL;
    int bits, i, add;
    char buffer[MAX_LINE] = "";
    int temp1, temp2;

    file = fopen(filename, "r");
    if(!file){
        perror("Failed to open file in function importFile\n");
        return EXIT_FAILURE;
    }

    for(i=0; !feof(file); i++){
        strcpy(buffer, "");
        fgets(buffer, MAX_LINE, file);
        bits = 0;
        while(strlen(buffer + bits)){
            if(sscanf(buffer + bits, "%d %d %n", &temp1, &temp2, &add) == 2){
                bits += add;
                if(sortedMerge(createNode(temp1, temp2), i==0 ? headptr1:headptr2) == EXIT_FAILURE){
                    perror("Failed to allocate memory in function importFile\n");
                    fclose(file);
                    return EXIT_FAILURE;
                }
            }
            else break;
        }
    }
    fclose(file);
    return EXIT_SUCCESS;
}

void freePolinom(polinom* headptr){
    polinom* temp1 = headptr;
    polinom* temp2 = headptr->next;
    
    if(!temp2) return;
    
    do{
        temp1 = temp2;
        temp2 = temp2->next;
        free(temp1);
    }while(temp2);
    headptr->next = NULL;
}

void printPolinom(polinom* headptr){
    polinom* node = headptr->next;

    if(node == NULL){
        printf("Polinom je prazan.\n");
    }

    //poseban ispis za prvi clan polinoma tako da se ne ispisuje znak '+'
    if(node->koeficijent == 1 || node->koeficijent == -1)   printf("%d\b", node->koeficijent);
    else    printf("%d", node->koeficijent);

    if(node->eksponent == 0);
    else if(node->eksponent == 1)   printf("x", node->eksponent);
    else    printf("x^%d", node->eksponent);


    for(node = node->next; node; node = node->next){
        printNode(node);
    }
    puts("");
}

void printNode(polinom* node){
    if(node->koeficijent == 1 || node->koeficijent == -1)   printf("%+d\b", node->koeficijent);
    else    printf("%+d", node->koeficijent);

    if(node->eksponent == 0);
    else if(node->eksponent == 1)   printf("x", node->eksponent);
    else    printf("x^%d", node->eksponent);
}

void removeZeroCoefficients(polinom* headptr){
    polinom* temp = NULL;
    
    for(temp = headptr; temp && temp->next; temp = temp->next){
        if(temp->next->koeficijent == 0){
            deleteNext(temp);
        }
    }
}

void deleteNext(polinom* before){
    polinom* to_delete = before->next;
    before->next = to_delete->next;
    free(to_delete);
}

polinom* sum(polinom* headptr1, polinom* headptr2){
    polinom sumhead = {.eksponent = 0, .koeficijent = 0, .next = NULL};
    polinom* sumptr = &sumhead;
    polinom* temp1 = headptr1->next;
    polinom* temp2 = headptr2->next;

    //prebaciti podatke iz head1 u sumu
    while(temp1){
        sumptr->next = createNode(temp1->eksponent, temp1->koeficijent);
	    sumptr = sumptr->next;
        temp1 = temp1->next;
    }

    //merge sa head2
    while(temp2){
        if(sortedMerge(createNode(temp2->eksponent, temp2->koeficijent), &sumhead) == EXIT_FAILURE){
            freePolinom(&sumhead);
            return NULL;
        }
        temp2 = temp2->next;
    }

    return sumhead.next;
}

polinom* multiply(polinom* headptr1, polinom* headptr2){
    polinom* temp1 = NULL;
    polinom* temp2 = NULL;
    polinom multiplyhead = {.eksponent = 0, .koeficijent = 0, .next = NULL};
    polinom* multiplyptr = &multiplyhead;

    for(temp1 = headptr1->next; temp1; temp1 = temp1->next){
        for(temp2 = headptr2->next; temp2; temp2 = temp2->next){
            if(sortedMerge(createNode(temp1->eksponent + temp2->eksponent, temp1->koeficijent * temp2->koeficijent), &multiplyhead) == EXIT_FAILURE){
                freePolinom(&multiplyhead);
                return NULL;
            }
        }
    }

    return multiplyhead.next;
}
