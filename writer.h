#ifndef WRITER_H
#define WRITER_H

#define WRITER_SUCCESS 0
#define WRITER_FAILURE 1
 
struct Writer{
	int writer_id;
	char writer_name[30];
	char phone[15];
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given writer into the repository by calling put_rec_by_key
int add_writer( struct Writer *c );

// Display writer info in a single line as a CSV without any spaces
void print_writer( struct Writer *c );

// Use get_rec_by_key function to retrieve writer
int search_writer( int writer_id, struct Writer *c );

// Load all the writers from a CSV file
int store_writers( char *writer_data_file );

int search_writer_by_phone( char *phone, struct Writer *c, int *io_count );

/* Return 0 if phone of the writer matches with phone parameter */
/* Return 1 if phone of the writer does NOT match */
/* Return > 1 in case of any other error */
int match_writer_phone( void *rec, void *key );

int delete_writer(int writer_id);

#endif
