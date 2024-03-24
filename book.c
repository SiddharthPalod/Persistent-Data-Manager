#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "book.h"
#define imt2022002 Siddharth Palod

// Load all the books from a CSV file
int store_books( char *book_data_file )
{
	FILE *cfptr;
	char book_line[500], token;
	struct Book c, dummy;

	cfptr = (FILE *) fopen(book_data_file, "r");
	while(fgets(book_line, sizeof(book_line)-1, cfptr)){
		//printf("line:%s",book_line);
		sscanf(book_line, "%d%s%s", &(c.book_id),c.book_name,c.publication_no);
		print_book( &c );
		add_book( &c );
	}
}

void print_book( struct Book *c )
{
	printf("%d,%s,%s\n", c->book_id,c->book_name,c->publication_no);
}

// Use get_rec_by_key function to retrieve book
int search_book( int book_id, struct Book *c )
{
	return get_linked_rec_by_key( book_id, c );
}

// Add the given book into the repository by calling put_rec_by_key
int add_book( struct Book *c )
{
	int status;

	status = put_linked_rec_by_key( c->book_id, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add book with key %d. Error %d", c->book_id, status );
		return BOOK_FAILURE;
	}
	return status;
}

