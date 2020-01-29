#include <stdlib.h>
#include <stdio.h>

#include "assert.h"
#include "table.h"
#include "list.h"
#include "atom.h"

// takes input and stores in table, returns filled table
//void input_to_table(FILE *stream, Table_T *table) {
//	while (1) {
//		if (feof(stream)) {
//			return;
//		}
//		char fprint[512], name[512];
//		char *fpnt = &fprint;
//		char *npnt = &name;
//
//
//	}
//}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	// establish standard input stream
	// FILE *stream = stdin;
	
	// initialize a new Hanson Table and pass to I/O function
	Table_T table = Table_new(100, NULL, NULL);
	// input_to_table(stream, &table);
	
	//create nullterm strings and make them atoms
	char s_key[] = "Sample Key";
	// char s_val[] = "Sample Value";
	char f_key[] = "Fake";
	const char *atom_key = Atom_string(s_key);
	// const char *atom_val = Atom_string(s_val);
	const char *fake_key = Atom_string(f_key);
	int val = 10;
	int *val_pnt = &val;
	
	// insert key/value pair into the Hanson Table
	Table_put(table, atom_key, val_pnt);

	printf("%d\n", *(int*)Table_get(table, atom_key));
	printf("%d\n", *(int*)Table_get(table, fake_key));
	
	Table_free(&table);
	
	// print atoms test
	// printf("Key: %s\n", atom_key);
	// printf("Value: %s\n", atom_val);

	return 0;
}
