// IMT2022002 Siddharth Palod
// To run the file type:
// gcc -o pds_own my_tester.c imt2022002_pds.c writer.c book.c bst.c
// ./pds_own my_testcase.in
// Parent - Writer and Child - Book

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "writer.h"
#include "book.h"

#define TREPORT(a1,a2) printf("Status: %s - %s\n\n",a1,a2); fflush(stdout);

int arraycheckhelper(const void *a, const void *b);
int sameArrayChecker(int *array1, int *array2, int size);
void getLine(char* templine, int* param1, int* param2, int** expected_nos, int* expected_status);
void process_line( char *test_case );
int main(int argc, char *argv[])
{
	FILE *cfptr;
	char test_case[50];

	if( argc != 2 ){
		fprintf(stderr, "Usage: %s testcasefile\n", argv[0]);
		exit(1);
	}

	cfptr = (FILE *) fopen(argv[1], "r");
	while(fgets(test_case, sizeof(test_case)-1, cfptr)){
		// printf("line:%s",test_case);
		if( !strcmp(test_case,"\n") || !strcmp(test_case,"") )
			continue;
		process_line( test_case );
	}
}

void process_line( char *test_case )
{
	char repo_name[30];
	char linked_repo_name[30];
	char command[15], param1[15], param2[15],param3[15] , info[1024];
	int parent_id, status, rec_size, expected_status,linked_rec_size;
	int child_id,result_set_size;
	struct Writer testWriter;
	struct Book testBook;

	// strcpy(testWriter.contact_name, "dummy name");
	// strcpy(testWriter.phone, "dummy number");

	rec_size = sizeof(struct Writer);
	linked_rec_size = sizeof(struct Book);

	sscanf(test_case, "%s%s%s%s", command, param1, param2, param3);
	printf("Test case: %s", test_case); fflush(stdout);
	if( !strcmp(command,"CREATE") ){
		if( !strcmp(param3,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;

		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2); 
		status = pds_create( repo_name, linked_repo_name);
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_create returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"OPEN") ){
		if( !strcmp(param3,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		status = pds_open( repo_name, linked_repo_name, rec_size, linked_rec_size );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_open returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"OPEN") ){
		if( !strcmp(param3,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		status = pds_open( repo_name, linked_repo_name, rec_size, linked_rec_size );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_open returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"LINK") ){
		if( !strcmp(param3,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;
		sscanf(param1, "%d", &parent_id);
		sscanf(param2, "%d", &child_id);
		status = pds_link_rec( parent_id, child_id );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_link_rec returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"PAR_STORE") ){
		if( !strcmp(param2,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;

		sscanf(param1, "%d", &parent_id);
		testWriter.writer_id = parent_id;
		sprintf(testWriter.phone, "Phone-of-%d", parent_id);
		sprintf(testWriter.writer_name, "Name-of-%d", parent_id);
		status = add_writer( &testWriter );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"add_contact returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"CHD_STORE") ){
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &child_id);
		testBook.book_id = child_id;
		sprintf(testBook.publication_no, "Phone-of-%d", child_id);
		sprintf(testBook.book_name, "Name-of-%d", child_id);
		status = add_book( &testBook );
		if(status == PDS_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"add_book returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"NDX_SEARCH") ){
		if( !strcmp(param2,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;

		sscanf(param1, "%d", &parent_id);
		testWriter.writer_id = -1;
		status = search_writer( parent_id, &testWriter );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",parent_id, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match
			char expected_phone[30];
    		sprintf(expected_phone, "Phone-of-%d", parent_id);
			char expected_name[30];
    		sprintf(expected_name, "Name-of-%d", parent_id);
			if( expected_status == 0 ){
				if (testWriter.writer_id == parent_id && 
					strcmp(testWriter.writer_name,expected_name) == 0 &&
					strcmp(testWriter.phone, expected_phone) == 0){
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Contact data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
						parent_id, expected_name, expected_phone, 
						testWriter.writer_id, testWriter.writer_name, testWriter.phone
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"CHD_SEARCH") ){
		if( !strcmp(param2,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;

		sscanf(param1, "%d", &child_id);
		testBook.book_id = -1;
		status = search_book( child_id, &testBook );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",child_id, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match
			char expected_phone[30];
    		sprintf(expected_phone, "Phone-of-%d", child_id);
			char expected_name[30];
    		sprintf(expected_name, "Name-of-%d", child_id);
			if( expected_status == 0 ){
				if (testBook.book_id == child_id && 
					strcmp(testBook.book_name,expected_name) == 0 &&
					strcmp(testBook.publication_no, expected_phone) == 0){
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Contact data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
						child_id, expected_name, expected_phone, 
						testBook.book_id, testBook.book_name, testBook.publication_no
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"NON_NDX_SEARCH") ){
		char phone_num[30], expected_name[30], expected_phone[30];
		int expected_io, actual_io;
		
		if( strcmp(param2,"-1") == 0 )
			expected_status = WRITER_FAILURE;
		else
			expected_status = WRITER_SUCCESS;

		sscanf(param1, "%s", phone_num);
		sscanf(param2, "%d", &expected_io);
		testWriter.writer_id = -1;
		actual_io = 0;
		status = search_writer_by_phone( phone_num, &testWriter, &actual_io );
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",parent_id, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match
			// Check if num block accesses match too
			// Extract the expected parent_id from the phone number
			sscanf(phone_num+sizeof("Phone-of"), "%d", &parent_id);
			sprintf(expected_name,"Name-of-%d",parent_id);
			sprintf(expected_phone,"Phone-of-%d",parent_id);
			if( expected_status == 0 ){
				if (testWriter.writer_id == parent_id && 
					strcmp(testWriter.writer_name, expected_name) == 0 &&
					strcmp(testWriter.phone, expected_phone) == 0 ){
						if( expected_io == actual_io ){
							TREPORT("PASS", "");
						}
						else{
							sprintf(info,"Num I/O not matching for contact %d... Expected:%d Got:%d\n",
								parent_id, expected_io, actual_io
							);
							TREPORT("FAIL", info);
						}
				}
				else{
					sprintf(info,"Contact data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
						parent_id, expected_name, expected_phone, 
						testWriter.writer_id, testWriter.writer_name, testWriter.phone
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"NDX_DELETE") ){
		if( strcmp(param2,"0") == 0 )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;

		sscanf(param1, "%d", &parent_id);
		testWriter.writer_id = -1;
		// printf("MY CONTACT ID IS : %d\n",parent_id);
		status = delete_writer( parent_id );
		if( status != expected_status ){
			sprintf(info,"delete key: %d; Got status %d",parent_id, status);
			TREPORT("FAIL", info);
		}
		else{
			TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"CLOSE") ){
		if( !strcmp(param1,"0") )
			expected_status = WRITER_SUCCESS;
		else
			expected_status = WRITER_FAILURE;

		status = pds_close();
		if(status == PDS_SUCCESS)
			status = WRITER_SUCCESS;
		else
			status = WRITER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_close returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command, "LINK_GET")) {
		char string[sizeof(test_case)+1]; //let us consider a buffer for input line 
		strcpy(string,test_case);
		int *expected_linked_keys;
		getLine(string, &parent_id, &result_set_size, &expected_linked_keys, &expected_status);
		int *linked_keys_result = (int *)malloc(result_set_size * sizeof(int));
		if (linked_keys_result == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			exit(1);
		}		
		int size = result_set_size;
		// Call pds_get_linked_rec function to retrieve linked records
		int status = pds_get_linked_rec(parent_id, linked_keys_result, &result_set_size);

	    status = sameArrayChecker(linked_keys_result, expected_linked_keys, size);
		
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_close returned status %d",status);
			TREPORT("FAIL", info);
		}

		// Free the allocated memory
		free(linked_keys_result);
	}

}

int arraycheckhelper(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
int sameArrayChecker(int *array1, int *array2, int size) {
    // Sort both arrays
    qsort(array1, size, sizeof(int), arraycheckhelper);
    qsort(array2, size, sizeof(int), arraycheckhelper);
	if(sizeof(array1) == 0)
		return WRITER_FAILURE;
    // ArraycheckHelper each element
    for (int i = 0; i < size; i++) {
        if (array1[i] != array2[i]) {
            return WRITER_FAILURE; // Arrays are not equal
        }
    }
    return WRITER_SUCCESS; // Arrays are equal
}

void getLine(char* templine, int* param1, int* param2, int** expected_nos, int* expected_status) {
	// Read param1, param2, and expected_status
	int count = 0;
	sscanf(templine, "%*s %d %d", param1, param2);
	// Find the position of the last number
	char* last_number = strrchr(templine, ' ');
	if (last_number != NULL) {
		*expected_status = atoi(last_number);
	}
	// Allocate memory for the expected_nos array
	*expected_nos = (int*)malloc(*param2 * sizeof(int));
	if (*expected_nos == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}
	// Parse expected_nos into the array
	char* token = strtok(templine, " ");
	int tokens_read = 0;
	while (token != NULL) {
		token = strtok(NULL, " "); // Get the next token
		if (token != NULL && tokens_read >= 2 && count < *param2) {
			(*expected_nos)[(count)++] = atoi(token);
		}
		tokens_read++;
	}
}