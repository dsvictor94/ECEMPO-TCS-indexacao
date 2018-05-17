#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool is_in(char *str, char c){
    for(int i = 0; str[i]; i++){
        if(c == str[i])
            return true;
    }
    return false;
}

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

void remove_from_string(char *str, int index){
    int i;
    for(i=index+1; str[i]; i++){
        if(i != 0){
            str[i-1] = str[i];
        }
    }
    str[i-1] = '\0';
}

void string_to_lower(char *str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

void remove_punctuation(char *str){
    char punctuation[] = "!\"#\%$&'()*+,-./:;<=>?@\\[]^_`{}~|";
    for(int i=0; str[i]; i++){
        if(is_in(punctuation, str[i])){
            remove_from_string(str, i);
            i--;
        }
    }
}

void tokenize(char *doc, char *tokenized_doc){
    FILE *document;
    FILE *tokenized_document;
    document = fopen(doc, "r");
    tokenized_document = fopen(tokenized_doc, "w");

    char word[1024];
    while (fscanf(document, " %1023s", word) == 1){
        string_to_lower(word);
        remove_punctuation(word);
        if (!is_stopword(word)){
            fputs(word, tokenized_document);
            fputs(" ", tokenized_document);
        }
    }
    fclose(document);
    fclose(tokenized_document);
}

int main(){

    char doc[] = "documents/exemplo.txt";
    char tokenized_doc[] = "documents/exemplo_tokenized.txt";
    tokenize(doc, tokenized_doc);

    return 0;
}