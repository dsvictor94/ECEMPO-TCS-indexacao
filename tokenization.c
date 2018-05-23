#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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
void tokenize(char *doc, char *tokenized_doc){
    // declara e abre os documentos
    FILE *document;
    FILE *tokenized_document;
    document = fopen(doc, "r");
    tokenized_document = fopen(tokenized_doc, "w");

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

    // declara os caminhos dos documentos e chama a função de tokenizar
    char doc[] = "documents/exemplo.txt";
    char tokenized_doc[] = "documents/exemplo_tokenized.txt";
    tokenize(doc, tokenized_doc);


    // declara os caminhos dos documentos e chama a função de tokenizar
    char doc2[] = "documents/exemplo2.txt";
    char tokenized_doc2[] = "documents/exemplo2_tokenized.txt";
    tokenize(doc2, tokenized_doc2);


    return 0;
}
