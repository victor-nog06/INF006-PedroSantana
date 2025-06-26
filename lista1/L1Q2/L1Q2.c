//Alunos: Pedro Santana e Victor Gabriel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 3001

typedef struct Node {
    char* text;
    struct Node* next;
} Node;

void push(Node** top, const char* text) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Erro ao alocar novo nó para a pilha");
        exit(EXIT_FAILURE);
    }

    new_node->text = strdup(text);
    if (!new_node->text) {
        perror("Erro ao duplicar texto para o nó");
        free(new_node);
        exit(EXIT_FAILURE);
    }

    new_node->next = *top;
    *top = new_node;
}


char* pop(Node** top) {
    if (*top == NULL) return NULL;

    Node* temp = *top;
    *top = temp->next;
    char* popped_text = temp->text;
    free(temp);
    return popped_text;
}


void sorted_push(Node** top, const char* text) {
    Node* temp_stack = NULL;

    
    while (*top != NULL && strcmp((*top)->text, text) > 0) {
        char* val = pop(top);
        push(&temp_stack, val);
      
    }


    push(top, text);

    
    while (temp_stack != NULL) {
        char* val = pop(&temp_stack);
        push(top, val);
    }
}


void free_stack(Node** top) {
    while (*top != NULL) {
        char* val = pop(top);
        free(val);
    }
}

int main() {
    FILE *fp_in = fopen("L1Q2.in", "r");
    FILE *fp_out = fopen("L1Q2.out", "w");

    if (fp_in == NULL || fp_out == NULL) {
        perror("Erro ao abrir arquivos");
        if (fp_in) fclose(fp_in);
        if (fp_out) fclose(fp_out);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];

    
    while (fgets(line, sizeof(line), fp_in) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove o '\n'

        char* line_copy = strdup(line);
        if (!line_copy) {
            perror("Erro ao duplicar linha");
            fclose(fp_in);
            fclose(fp_out);
            return EXIT_FAILURE;
        }

        Node* stack = NULL;
        char* command_token = strtok(line_copy, " ");

        
        while (command_token != NULL) {
            if (strncmp(command_token, "push-", 5) == 0) {
                sorted_push(&stack, command_token + 5);
            } else if (strstr(command_token, "x-pop") != NULL) {
                int n;
                if (sscanf(command_token, "%dx-pop", &n) == 1) {
                    for (int i = 0; i < n; i++) {
                        char* val = pop(&stack);
                        if (val) free(val);
                    }
                }
            }
            command_token = strtok(NULL, " ");
        }

        
        Node* temp_stack_for_print = NULL;
        Node* current = stack;

        while (current != NULL) {
            push(&temp_stack_for_print, current->text);
            current = current->next;
        }

        current = temp_stack_for_print;
        if (current != NULL) {
            fprintf(fp_out, "%s", current->text);
            current = current->next;
        }

        while (current != NULL) {
            fprintf(fp_out, " %s", current->text);
            current = current->next;
        }

        fprintf(fp_out, "\n");

        
        free_stack(&temp_stack_for_print);
        free_stack(&stack);
        free(line_copy);
    }

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}
