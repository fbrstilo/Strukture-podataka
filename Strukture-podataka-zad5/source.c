/*
Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _StackElement{
    double number;
    struct _StackElement* next;
}StackElement;

double Pop(StackElement* headptr);
StackElement* CreateElement(double number);
int Push(StackElement* headptr, double number);
int DeleteFirst(StackElement* headptr);
int Calculate(StackElement* headptr, char operation);
int ElementCount(StackElement* headptr);
void DeleteList(StackElement* headptr);
char* FileImport(char* filename);

int main(){
    StackElement head = {.number = 0, .next = NULL};
    StackElement* new = NULL;
    char* filename = "postfix.txt";
    char* buffer = NULL;
    char operation;
    int read, n, bits = 0;

    buffer = FileImport(filename);
    if(!buffer){
        return EXIT_FAILURE;
    }
    
    while(strlen(buffer + bits)){
        if(sscanf(buffer  + bits, " %d %n", &read, &n) == 1){
            if(Push(&head, read) == EXIT_FAILURE){
                DeleteList(&head);
                free(buffer);
                return EXIT_FAILURE;
            }
        }
        else if(sscanf(buffer + bits, " %c %n", &operation, &n) == 1){
            if(Calculate(&head, operation) == EXIT_FAILURE){
                DeleteList(&head);
                free(buffer);
                return EXIT_FAILURE;
            }
        }
        bits += n;
    }

    if(ElementCount(&head) != 1){
        printf("Invalid postfix: ammount remaining != 1\n");
        free(buffer);
        DeleteList(&head);
        return EXIT_FAILURE;
    }

    printf("Result is %lf.\n", head.next->number);
    free(buffer);
    DeleteList(&head);
    return EXIT_SUCCESS;
}

StackElement* CreateElement(double number){
    StackElement* new = NULL;

    new = (StackElement*)malloc(sizeof(StackElement));
    if(!new){
        perror("Failed to allocate memory for StackElement\n");
        return NULL;
    }

    new->number = number;
    new->next = NULL;
    return new;
}

int Push(StackElement* headptr, double number){
    StackElement* new = NULL;
    new = CreateElement(number);
    if(!new){
        return EXIT_FAILURE;
    }

    new->next = headptr->next;
    headptr->next = new;
    return EXIT_SUCCESS;
}

int DeleteFirst(StackElement* headptr){
    StackElement* to_delete = headptr->next;
    if(!to_delete){
        return EXIT_SUCCESS;
    }

    headptr->next = to_delete->next;
    free(to_delete);
    return EXIT_SUCCESS;
}

int Calculate(StackElement* headptr, char operation){
    double a = 0;
    double b = 0;
    double result = 0;

    b = Pop(headptr);
    a = Pop(headptr);
    if(a == NAN || b == NAN){
        return EXIT_FAILURE;
    }

    switch(operation){
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if(!b){
                printf("Can't divide by zero!\n");
                return EXIT_FAILURE;
            }
            result = a / b;
            break;
        default:
            printf("This operation is not supported yet.\n");
            return EXIT_FAILURE;
    }
    if(Push(headptr, result) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int ElementCount(StackElement* headptr){
    StackElement* temp = headptr->next;
    int count = 0;
    while(temp){
        count++;
        temp = temp->next;
    }
    return count;
}

void DeleteList(StackElement* headptr){
    StackElement* temp1 = headptr;
    StackElement* temp2 = headptr->next;
    
    while(temp2){
        temp1 = temp2;
        temp2 = temp2->next;
        free(temp1);
    }
    headptr->next = NULL;
}

double Pop(StackElement* headptr){
    double to_return;
    if(!headptr->next){
        printf("Invalid postfix!\n");
        return NAN;
    }
    to_return = headptr->next->number;
    DeleteFirst(headptr);
    return to_return;
}

char* FileImport(char* filename){
    FILE* file = NULL;
    int filesize = 0;
    char* buffer = "";
    
    file = fopen(filename, "rb");
    if(!file){
        perror("Failed to open file. Check filename and working directory.\n");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    buffer = (char*)malloc(filesize);
    if(!buffer){
        perror("Failed to allocate memory for buffer.\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, sizeof(char), filesize, file);
    fclose(file);
    return buffer;
}