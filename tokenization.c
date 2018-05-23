#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "tuple_space.h"

//função para verificar se um caractere está presente em uma string
bool is_in(char *str, char c){
    for(int i = 0; str[i]; i++){
        if(c == str[i])
            return true;
    }
    return false;
}

//função para verificar se uma palvra está no arquivo de stopwords.txt
bool is_stopword(char *word){
    FILE *stopwords;
    stopwords = fopen("stopwords.txt", "r");

    char stopword[1024];
    while (fscanf(stopwords, " %1023s", stopword) == 1){
        if (strcmp(stopword, word) == 0){
            printf("%s %s\n", stopword, word);
            fclose(stopwords);
            return true;
        }
    }
    fclose(stopwords);
    return false;
}

//função para remover um caractare de uma string dado o índice deste caractere
void remove_from_string(char *str, int index){
    int i;
    for(i=index+1; str[i]; i++){
        if(i != 0){
            str[i-1] = str[i];
        }
    }
    str[i-1] = '\0';
}

//função para transformar uma string para caracteres minúsculos
void string_to_lower(char *str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

//função para remover as pontuações de uma palavra
void remove_punctuation(char *str){
    char punctuation[] = "!\"#\%$&'()*+,-./:;<=>?@\\[]^_`{}~|";
    for(int i=0; str[i]; i++){
        if(is_in(punctuation, str[i])){
            remove_from_string(str, i);
            i--;
        }
    }
}

//função para realizar a tokenização de um documento
void tokenize(int doc_fd, int tokenized_doc_fd){
    // declara e abre os documentos
    FILE *document;
    FILE *tokenized_document;
    document = fdopen(doc_fd, "r");
    tokenized_document = fdopen(tokenized_doc_fd, "w");

    char word[1024];

    // pega palavra por palavra do documento, até não existirem mais palavras
    while (fscanf(document, " %1023s", word) == 1){
        string_to_lower(word); // passa para minusculas
        remove_punctuation(word); // remove as pontuações
        if (!is_stopword(word)){ // se não for uma stopword
            fputs(word, tokenized_document); // escreve a palavra no documento tokenizado
            fputs(" ", tokenized_document); // escreve um espaço no documento tokenizado
        }
    }

    // fecha os documentos
    fclose(document);
    fclose(tokenized_document);
}

int main(){
    tuple_space_t tuple_space;
    tuple_space_init(&tuple_space, "/tmp/ts");
    int fd = 0;
    int fd_to_write = 0;
    int doc = 0;
    char buffer[20];

    while(fd = tuple_space_take(&tuple_space, "doc/*")) {
        // declara os caminhos dos documentos e chama a função de tokenizar
        sprintf(buffer, "%d.txt", doc);
        fd_to_write = tuple_space_write(&tuple_space, buffer);

        // printf("start %d: %s (%d)\n", doc, buffer, fd_to_write);
        tokenize(fd, fd_to_write);
        // printf("end %d\n", doc);

        doc++;

        tuple_space_release(fd_to_write);
        tuple_space_release(fd);
    }
    
    return 0;
}
