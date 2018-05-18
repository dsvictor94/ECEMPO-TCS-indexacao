#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define INDEX_ROOT_PATH "index/"
#define N_FILE "index/_properties/N"

int doc_frequence(char *term){
	
	char *term_path = malloc(strlen(term) + strlen(INDEX_ROOT_PATH) + 1);
	
	strcpy(term_path, INDEX_ROOT_PATH);
	strcat(term_path, term);

	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(term_path); /* There should be error handling after this */
	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_REG) { /* If the entry is a regular file */
		     file_count++;
		}
	}
	closedir(dirp);
	return file_count;
}

int read_N() {

	FILE *N_file;
	N_file = fopen(N_FILE, "r");
	int pos = ftell(N_file);

	int N = 0;
	fscanf(N_file, "%d", &N);
	fclose(N_file);

	return N;
}

double calc_idf(char *term) {

	int N = read_N();
	int df = doc_frequence(term);

	return log(N/df);
}

void create_term_folder(char *term){
	
}


void update_counter_file(char *file){

	FILE *counter_file;
	if(counter_file = fopen(file, "r")) {
		int pos = ftell(counter_file);

		int N = 0;
		fscanf(counter_file, "%d", &N);
		N++;

		fclose(counter_file);

		counter_file = fopen(file, "w");
		fprintf(counter_file, "%d", N);
	} else {
		counter_file = fopen(file, "ab+");
		fputs("1", counter_file);
	}

	fclose(counter_file);
	
}

void add_term_frequence(char *term, char *doc_id) {

	char *term_path = malloc(strlen(term) + strlen(INDEX_ROOT_PATH) + 1);
	
	strcpy(term_path, INDEX_ROOT_PATH);
	strcat(term_path, term);

	struct stat st = {0};

	if (stat(term_path, &st) == -1) {
		mkdir(term_path, 0700);
	}

	char *term_doc_path = malloc(strlen(term) + strlen(INDEX_ROOT_PATH) + strlen(doc_id) + 2);
	
	strcpy(term_doc_path, INDEX_ROOT_PATH);
	strcat(term_doc_path, term);
	strcat(term_doc_path, "/");
	strcat(term_doc_path, doc_id);
	strcat(term_doc_path, "\0");

	update_counter_file(term_doc_path);
}



void indexing(char *doc_id, char *tokenized_doc) {
	// soma 1 ao número total de docs indexados
	update_counter_file(N_FILE);
	
	FILE *tokenized_document;
	tokenized_document = fopen(tokenized_doc, "r");
	char word[1024];
    while (fscanf(tokenized_document, " %1023s", word) == 1){
		add_term_frequence(word, doc_id);
		
    }
    fclose(tokenized_document);
}

int main() {
	char tokenized_doc[] = "documents/exemplo_tokenized.txt";
	char doc_id[] = "exemplo.txt";
	indexing(doc_id, tokenized_doc);


	char tokenized_doc2[] = "documents/exemplo2_tokenized.txt";
	char doc_id2[] = "exemplo2.txt";
	indexing(doc_id2, tokenized_doc2);

}
