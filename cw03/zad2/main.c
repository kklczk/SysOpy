#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NUMBER_OF_ARGS 32
#define MAX_ARG_LENGTH 16
#define MAX_LINE_LENGTH 256


char** parse_task_from_file (char* line){
        char** result = calloc(MAX_NUMBER_OF_ARGS, sizeof(char*));
        char* arg_buffor;

        result[0] = calloc(MAX_ARG_LENGTH, sizeof(char));
        arg_buffor = strtok(line, " \n\t");
        strcpy(result[0], arg_buffor);

        int i = 1;
        arg_buffor = strtok(NULL, " \n\t");
        while(arg_buffor && i < MAX_NUMBER_OF_ARGS){
            result[i] = calloc(MAX_ARG_LENGTH, sizeof(char));
            strcpy(result[i], arg_buffor);
            arg_buffor = strtok(NULL, " \n\t");
            i++;
        }
        result[i] = NULL;

        return result;
    }

int exec_line(FILE* file, char* line_buffor, int counter){
    if (!fgets(line_buffor, MAX_LINE_LENGTH, file)) {
        printf("There is no more line or something went wrong with getting line.\n");
        return 1;
    }

    char** parsed_line;

    parsed_line = parse_task_from_file(line_buffor);


    int new_process;
    new_process = fork();

    if(new_process > 0){
        int status;
        wait(&status);
        if (status != 0) {
            printf("\nSomething went wrong with %s command in line %d.\n", parsed_line[0], counter);
            exit(1);
        }
    }

    if (new_process == 0){
        int new_exec;
        printf("\nExecuting \"%s\" command with arguments ", parsed_line[0]);
        for (int i = 1; parsed_line[i] != NULL; i++){
            printf("\"%s\" ", parsed_line[i]);
        }
         printf("\n");
       new_exec = execvp(parsed_line[0], parsed_line);
        printf("\n");
        if(new_exec==-1){
            exit(1);
        }
        exit(1);
    }

    if (new_process < 0){
        printf("Something went wrong with fork.\n");
        exit(1);
    }

    return 0;
}


int main(int argc, char** argv) {

    if (argc < 2){
        printf("You did't gave enough arguments. Please type file directory. \n");
        return 1;
    }

    FILE* file_with_tasks = fopen(argv[1], "r");
    if(!file_with_tasks){
        printf("Ups, something went wrong with opening file.\n");
        return 1;
    }

    char* line_buffor = calloc(MAX_LINE_LENGTH, sizeof(char));

    int line_counter = 1;
    while(!exec_line(file_with_tasks, line_buffor, line_counter)){
        line_counter++;
    }
    fclose(file_with_tasks);
    free(line_buffor);

    return 0;
}