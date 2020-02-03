#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "assert.h"
#include "table.h"
#include "atom.h"
#include "list.h"
#include "array.h"
#include "mem.h"
#include "set.h"

void print_list(void **x, void  *cl){
	(void)cl;
	// for each entry in the list, print
	char **name = (char **)x;
	if(name != NULL){
		printf("%s\n", *name);
	}
}

void print_table(const void *key, void **value, void *cl){
	(void)key; (void)cl;
	// type cast the void ** to a List_T **
	List_T **this_list = (List_T **)value;
	// Set_T **this_set = (Set_T **)value;
	List_T *to_print = *this_list;
	// Set_T *to_print = *this_set;
	// if the list has more than one entry, map a print function to it
	if(List_length(*to_print) > 1){
		List_map(*to_print, print_list, NULL);
		printf("\n");
	}
}

void print_results(Table_T table){
	// map a print function to each key-value pair
	Table_map(table, print_table, NULL);
}

char *get_name(FILE *stream, int size){
	// establish an empty char array
	char *name;
	int chr;
	size_t len = 0;
	unsigned char space = 32;
	unsigned char tab = 9;
	
	// allocate an initial amount of memory
	name = (char *)realloc(NULL, sizeof(name)*size);
	
	// get the first character
	chr = fgetc(stream);
	// if the character is a whitespace, burn whitespaces until not whitespace
	while(chr == (int)space || chr == (int)tab){
		chr = fgetc(stream);
	}
	// while the character is not a newline or EOF
	while(chr != -1 && chr != '\n'){
		// insert character into name
		name[len] = chr;
		len++;
		// if the length approaches the allocated size, double the size
		if((int)len == size){
			name = (char *)realloc(name, sizeof(name)*2);
			if(!name) return name;
		}
		
		// get next character
		chr = fgetc(stream);
	}
	
	// add the terminal character to the end of the char array
	name[len++] = '\0';
	// return a better allocated char array
	return (char *)realloc(name, sizeof(char)*len);
}

char *get_fprint(FILE *stream){
	// initialize char array
	char *fprint;
	int chr;
	size_t len = 0;
	int max_len = 512;
	unsigned char space = 32;
	unsigned char tab = 9;
	
	// allocate a fprint size of 512 character
	fprint = (char *)realloc(NULL, sizeof(fprint)*512);
	
	// get the first character of the fprint
	chr = fgetc(stream);
	// if EOF, return NULL
	if(chr == -1){
		return NULL;
	}
	
	// while the character is not a whitespace character
	while(chr != (int)space && chr != (int)tab){
		// assign char to next idx
		fprint[len] = chr;
		len++;
		// if array reaches max allocation, double allocation
		if((int)len == max_len){
			fprint = (char *)realloc(fprint, sizeof(fprint)*2);
			if(!fprint) return fprint;
		}
		// get next char
		chr = fgetc(stream);
	}
	
	// add terminal character to end of char arrray
	fprint[len++] = '\0';
	// return better allocated char array
	return (char *)realloc(fprint, sizeof(char)*len);
}

// for each list in the table, free it
void free_table(const void *key, void **value, void *cl){
	(void)cl; (void)key;
	List_free(*value);
	return;
}

// take lines of input and insert them into a table in key value pairs
Table_T input_table(FILE *stream){
	// initialize table
	Table_T table = Table_new(100, NULL, NULL);
	
	// initialize input, fprint and name char arrays
	// int max_fprint_len = 512;
	// for each input line, seperate the input into fprint and name
	while(1){
		if(feof(stream)){
			break;
		}
		// initialize fprint and name char arrays
		char *fprint;
		char *name;
		// get the fprint from stdin
		fprint = get_fprint(stream);
		// if the function returned NULL, EOF -> break
		if(fprint == NULL){
			break;
		}
		// get the name from stdin
		name = get_name(stream, 100);
		
		// int max_in_length = 1000;
		// int max_fprint_length = 512;
		// int max_name_length = max_in_length - max_fprint_length - 1;
		// char *input[max_in_length];
		// NEW(input);
		// input = gets(input);
		
		// char fprint[max_fprint_length];
		// char name[max_name_length];

		// char *fprint = strtok(input, " ");



		// char *input_pnt;
		// input_pnt = &input;
		// char *fprint;
		// char *name = input;	
		// input_pnt = fgets(input, input_max_len, stream);
		// fprint = __strtok_r(input, " ", &name);
		
		// make new atom to use as key in table
		const char *new_fprint;
		new_fprint = Atom_string(fprint);

		List_T *list;
		// Set_T *set;
		NEW(list);
		// NEW(set);
		*list = List_list(name, NULL);
		// *set = Set_new(100, NULL, NULL);
		// Set_put(*set, name);
		list = realloc(list, sizeof(list));
		// set = realloc(set, sizeof(set));

		List_T *gotten_list;
		// Set_T *gotten_set;
		NEW(gotten_list);
		// NEW(gotten_set);
		*gotten_list = Table_get(table, new_fprint);
		// *gotten_set = Table_get(table, new_fprint);
		gotten_list = realloc(gotten_list, sizeof(gotten_list));
		// gotten_set = realloc(gotten_set, sizeof(gotten_set));

		if(gotten_list == NULL){	
			// create new list based off name
			List_T *list;
			NEW(list);
			*list = List_list(name, NULL);
			list = realloc(list, sizeof(list));

			// printf("list len: %d\n", List_length(*list));
		
			Table_put(table, new_fprint, list);
			// Table_put(table, new_fprint, set);
		}
		else{
			// *gotten_list = List_append(*gotten_list, *name);
			// gotten_list = realloc(gotten_list, sizeof(gotten_list));

			// printf("list len: %d\n", List_length(*gotten_list));
			*gotten_list = List_push(*gotten_list, name);
			// Set_put(*set, name);
			gotten_list = realloc(gotten_list, sizeof(gotten_list));
			// gotten_set = realloc(gotten_set, sizeof(gotten_set));
			Table_put(table, new_fprint, gotten_list);
			// Table_put(table, new_fprint, gotten_set);
		}

		// insert atom-list pair into the table
		// Table_put(table, new_fprint, list);
	}
	
	// print_results(table);
	Table_map(table, print_table, NULL);


	return table;
}

int main(int argc, char* argv[]){
	// void unnecessary argument variables
	(void)argc; (void)argv;
	// initialise standard input as file stream
	FILE *stream = stdin;

	// initialise table
	// Table_T table = input_table(stream);
	input_table(stream);
	fclose(stream);
	// Table_T table = Table_new(100, NULL, NULL);
	
	// initialise a fprint and name combo
	// char fprint[] = "Hello";
	// char name[] = "John";
	// store fprint as an atom
	// const char *new_fprint;
	// NEW(new_fprint);
	// new_fprint = Atom_string(fprint);
	// create a list and new pointer to the list with the name in it
	// List_T list;
	// List_T *list_pnt;
	// NEW(list);
	// NEW(list_pnt);
	// list = List_list(name);
	// list_pnt = &list;

	// put the new key value pair in the list
	// Table_put(table, new_fprint, &list);
	
	// check table length
	// printf("table_len = %d\n", Table_length(table));

	// print key/value pair using map functions
	// printf("key: atom_s Hello\tvalue mem loc: %p\n", Table_get(table, new_fprint));
	// List_T *list_for_print = Table_get(table, new_fprint);
	// printf("list len: %d\n", List_length(*list_for_print));
	
	// List_map(*list_for_print, apply_print, NULL);
	
	// const char *foo;
	// const char *fee;
	// const char *faa;
	// foo = Atom_string("foo");
	// fee = Atom_string("fee");
	// faa = Atom_string("faa");

	// List_T *this_value = Table_get(table, foo);
	// printf("list len: %d\n", List_length(*this_value));
	
	// print each fgroup
	// print_results(table);
	
	// free the mem allocated for the lists and table
	// Table_map(table, free_table, NULL);
	// Table_free(&table);

	return 0;
}
