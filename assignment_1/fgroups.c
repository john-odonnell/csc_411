#include <stdlib.h>
#include <stdio.h>

#include "assert.h"
#include "table.h"
#include "list.h"
#include "atom.h"

// takes input and stores in table, returns filled table
int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	// establish standard input stream
	FILE *stream = stdin;
	
	// initialize a new Hanson Table and pass to I/O function
	Table_T table = Table_new(100, NULL, NULL);
	
	while(1) {
		// end loop if end of file
		if (feof(stream)) {
			break;
		}

		// take fprint and name from input
		char fprint[512], name[512];
		int streams = fscanf(stream, "%s %s \n", fprint, name);
		(void)streams;
		// printf("streams: %d\n", streams);
		// printf("%s\n%s\n", fprint, name);
		
		const char *new_fprint = Atom_string(fprint);
		const char *new_name = Atom_string(name);
		
		List_T list = List_list((char*)new_name);
		Table_put(table, new_fprint, list);
	}
	
	// input_to_table(stream, &table);
	
	//create nullterm strings and make them atoms
	// char s_key[] = "Sample Key";
	// const char *atom_key = Atom_string(s_key);
	// int val = 10;
	// int *val_pnt = &val;
	
	// insert key/value pair into the Hanson Table
	// Table_put(table, atom_key, val_pnt);
	
	const char *foo = Atom_string("foo");
	const char *fee = Atom_string("fee");

	printf("%s\tlen=%d\n", foo, List_length(Table_get(table, foo)));
	printf("%s\tlen=%d\n", fee, List_length(Table_get(table, fee)));
		
	Table_free(&table);
	
	return 0;
}
