#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "writer.h"
#define imt2022002 Siddharth Palod

// Load all the writers from a CSV file
int store_writers( char *writer_data_file )
{
	FILE *cfptr;
	char writer_line[500], token;
	struct Writer c, dummy;

	cfptr = (FILE *) fopen(writer_data_file, "r");
	while(fgets(writer_line, sizeof(writer_line)-1, cfptr)){
		//printf("line:%s",writer_line);
		sscanf(writer_line, "%d%s%s", &(c.writer_id),c.writer_name,c.phone);
		print_writer( &c );
		add_writer( &c );
	}
}

void print_writer( struct Writer *c )
{
	printf("%d,%s,%s\n", c->writer_id,c->writer_name,c->phone);
}

// Use get_rec_by_key function to retrieve writer
int search_writer( int writer_id, struct Writer *c )
{
	return get_rec_by_ndx_key( writer_id, c );
}

// Add the given writer into the repository by calling put_rec_by_key
int add_writer( struct Writer *c )
{
	int status;

	status = put_rec_by_key( c->writer_id, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add writer with key %d. Error %d", c->writer_id, status );
		return WRITER_FAILURE;
	}
	return status;
}

// Use get_rec_by_non_ndx_key function to retrieve writer
// Hint: get_rec_by_non_ndx_key( phone, c, &match_writer_phone, io_count );
// Return WRITER_SUCCESS if writer is found, WRITER_FAILURE if not found
int search_writer_by_phone( char *phone, struct Writer *c, int *io_count )
{
	if( get_rec_by_non_ndx_key( phone, c, match_writer_phone, io_count ) == PDS_REC_NOT_FOUND){
		return WRITER_FAILURE;}
	else
		WRITER_SUCCESS;	
}

/* Return 0 if phone of the writer matches with phone parameter */
/* Return 1 if phone of the writer does NOT match */
/* Return > 1 in case of any other error */
int match_writer_phone(void *rec, void *key) {
	// Store the rec in a struct writer pointer
	// Store the key in a char pointer
	// Compare the phone values in key and record
	// Return 0,1,>1 based on above condition
    struct Writer *writer = (struct Writer *)rec; 
    char *phone_key = (char *)key; // Cast key to a char pointer
    int match = strcmp(writer->phone, phone_key);
    if (match == 0) {
        return 0; 
    } 
    else if (match != 0) {
        return 1;
    } 
    else {
        return 100; //>1
    }
}

int delete_writer(int writer_id){
	if( delete_rec_by_ndx_key( writer_id ) == PDS_SUCCESS){
		return WRITER_SUCCESS;}
	else 
		WRITER_FAILURE;	
}