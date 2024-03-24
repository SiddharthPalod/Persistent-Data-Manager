#ifndef BOOK_H
#define BOOK_H

#define BOOK_SUCCESS 0
#define BOOK_FAILURE 1
 
struct Book{
	int book_id;
	char book_name[30];
	char publication_no[15];
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given book into the repository by calling put_rec_by_key
int add_book( struct Book *c );

// Display book info in a single line as a CSV without any spaces
void print_book( struct Book *c );

// Use get_rec_by_key function to retrieve book
int search_book( int book_id, struct Book *c );

// Load all the books from a CSV file
int store_books( char *book_data_file );

#endif
