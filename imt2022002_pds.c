#include <stdio.h>
#include <stdlib.h>
#include "pds.h"
#include "bst.h"
#include <string.h>
#define imt2022002 Siddharth Palod

struct PDS_RepoInfo repo_handle;
void pre_order(struct BST_Node *root) {
    if (root == NULL) return;
    fwrite(root->data, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);
    pre_order(root->left_child);
    pre_order(root->right_child);
}

int pds_create(char *repo_name, char*linked_repo_name) {
    char filename[30];
    strcpy(filename, repo_name);
    strcat(filename, ".dat");
    repo_handle.pds_data_fp = fopen(filename, "wb");
    if (repo_handle.pds_data_fp == NULL) {
        return PDS_FILE_ERROR;
    }
    char filename2[30];
    strcpy(filename2, repo_name);
    strcat(filename2, ".ndx");
    repo_handle.pds_ndx_fp = fopen(filename2, "wb");
    if (repo_handle.pds_ndx_fp == NULL) {
        // fclose(repo_handle.pds_data_fp); // Close data file before returning
        return PDS_FILE_ERROR;
    }
    char filename3[30];
    strcpy(filename3, linked_repo_name);
    strcat(filename3, ".dat");
    repo_handle.pds_link_fp = fopen(filename3, "wb");
    if (repo_handle.pds_link_fp == NULL) {
        return PDS_FILE_ERROR;
    }
    char filename4[60];
    strcpy(filename4, repo_name);
    strcat(filename4, "_");
    strcat(filename4, linked_repo_name);
    strcat(filename4, ".dat");
    repo_handle.pds_linked_data_fp = fopen(filename4, "wb");
    if (repo_handle.pds_linked_data_fp == NULL) {
        return PDS_FILE_ERROR;
    }



    int t=0;
    // fprintf(repo_handle.pds_ndx_fp, "%d", 0); // Write to index file
    fwrite(&t,sizeof(int),1,repo_handle.pds_ndx_fp);
    fclose(repo_handle.pds_data_fp); // Close data file
    fclose(repo_handle.pds_ndx_fp);  // Close index file
    fclose(repo_handle.pds_link_fp);
    fclose(repo_handle.pds_linked_data_fp);
    return PDS_SUCCESS;
}

int pds_open( char *repo_name, char *linked_repo_name, int rec_size, int linked_rec_size ){
    char filename[30];
    strcpy(filename, repo_name);
    strcat(filename, ".dat");
    repo_handle.pds_data_fp = fopen(filename, "rb+");
    if (repo_handle.pds_data_fp == NULL) {
        return PDS_FILE_ERROR;
    }

    char filename2[30];
    strcpy(filename2, repo_name);
    strcat(filename2, ".ndx");
    repo_handle.pds_ndx_fp = fopen(filename2, "rb+");
    if (repo_handle.pds_ndx_fp == NULL) {
        // fclose(repo_handle.pds_data_fp); // Close data file before returning
        return PDS_FILE_ERROR;
    }



    char filename3[30];
    char filename4[60];
    if(linked_repo_name != NULL){
        strcpy(filename3, linked_repo_name);
        strcpy(filename3, linked_repo_name);
        strcat(filename3, ".dat");
        repo_handle.pds_link_fp = fopen(filename3, "rb+");
        if (repo_handle.pds_link_fp == NULL) {
            return PDS_FILE_ERROR;
        }

        strcpy(filename4, repo_name);
        strcat(filename4, "_");
        strcat(filename4, linked_repo_name);
        strcat(filename4, ".dat");
        repo_handle.pds_linked_data_fp = fopen(filename4, "rb+");
        if (repo_handle.pds_linked_data_fp == NULL) {
            return PDS_FILE_ERROR;
        }
    }


    strcpy(repo_handle.pds_name, repo_name);
    repo_handle.rec_size = rec_size;
    repo_handle.linked_rec_size = linked_rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;
    // fscanf(repo_handle.pds_ndx_fp, "%d", &repo_handle.rec_count);
    fread(&repo_handle.rec_count,sizeof(int),1,repo_handle.pds_ndx_fp);
    // repo_handle.ndx_root = NULL;
    // if (pds_load_ndx() != PDS_SUCCESS) {
    //     fclose(repo_handle.pds_ndx_fp);
    //     // fclose(repo_handle.pds_data_fp);
    //     return PDS_FILE_ERROR;
    // }
    pds_load_ndx();

    fclose(repo_handle.pds_ndx_fp);
    return PDS_SUCCESS;
}

// pds_load_ndx - Internal function
// Load the index entries into the BST ndx_root by calling bst_add_node repeatedly for each 
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx() {
    int i;
    // repo_handle.ndx_root = NULL;
    // fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
    for (i = 0; i < repo_handle.rec_count; i++) {
        struct PDS_NdxInfo *ndx_info=malloc(sizeof(struct PDS_NdxInfo));
        if (fread(ndx_info, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp) != 1) {
            return PDS_FILE_ERROR;
        }
        bst_add_node(&repo_handle.ndx_root, ndx_info->key, ndx_info);
    }
    return PDS_SUCCESS;
}


int put_rec_by_key(int key, void *rec) {
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_END) != 0) {
        return PDS_FILE_ERROR; // Error seeking to the end of the file
    }
    struct PDS_NdxInfo *s = malloc(sizeof(struct PDS_NdxInfo));
    int offset = ftell(repo_handle.pds_data_fp);
    if (offset == -1) {
        return PDS_FILE_ERROR; // Error getting file offset
    }

    s->key = key;
    s->offset = offset;
    s->is_deleted = 0;

    // if (bst_search(repo_handle.ndx_root, key) != NULL) {
    //     return PDS_ADD_FAILED; // Duplicate key
    // }
    
    struct BST_Node* node = bst_search(repo_handle.ndx_root , key );
    struct PDS_NdxInfo*info;
    if(node!=NULL)
        info=(struct PDS_NdxInfo*)node->data;
    if (bst_add_node(&repo_handle.ndx_root, key, s) && info->is_deleted==0) {
        return PDS_FILE_ERROR; // Error adding node to BST
    }
    if(node!=NULL){
        info->is_deleted=0;
        info->offset=offset;
    }
    repo_handle.rec_count++;
    // fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp);
    fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
    return PDS_SUCCESS;
}

// //////////////////////////////////////////////
// put_linked_rec_by_key - NEW
// Seek to the end of the linked data file
// No need to create index entry
// Write the key at the current data file location
// Write the record after writing the key
int put_linked_rec_by_key( int key, void *rec ){
    if (fseek(repo_handle.pds_linked_data_fp, 0, SEEK_END) != 0) {
        return PDS_FILE_ERROR; // Error seeking to the end of the file
    }
    struct PDS_NdxInfo *s = malloc(sizeof(struct PDS_NdxInfo));
    s->key = key;
    fwrite(&key, sizeof(int), 1, repo_handle.pds_linked_data_fp);
    fwrite(rec, repo_handle.linked_rec_size, 1, repo_handle.pds_linked_data_fp);
    return PDS_SUCCESS;    
}

// //////////////////////////////////////////////



int get_rec_by_ndx_key(int key, void *rec) {
    struct BST_Node *node = bst_search(repo_handle.ndx_root, key);
    if (node == NULL) {
        return PDS_REC_NOT_FOUND; // Record not found
    }
    struct PDS_NdxInfo*info=(struct PDS_NdxInfo*)node->data;

    if(info->is_deleted==0){
        if (fseek(repo_handle.pds_data_fp, info->offset, SEEK_SET) != 0) {
            return PDS_FILE_ERROR; // Error seeking to the file location
        }
        if (fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) != 1) {
            return PDS_FILE_ERROR; // Error reading the record
        }
    }
    else
        return PDS_REC_NOT_FOUND;

    return PDS_SUCCESS;
}

int non_ndx_delete_finder(struct BST_Node *node , int offset){
    if(node == NULL) return 1;
    struct PDS_NdxInfo*info=(struct PDS_NdxInfo*)node->data;
    if(info->offset == offset) return info->is_deleted;
    return non_ndx_delete_finder(node->left_child, offset) && non_ndx_delete_finder(node->right_child,offset);
}

int get_rec_by_non_ndx_key(void *non_ndx_key, void *rec, int (*matcher)(void *rec, void *non_ndx_key), int *io_count) {
    fseek(repo_handle.pds_data_fp,0,SEEK_SET);
    *io_count = 0; // Initialize io_count to 0
    while (!feof(repo_handle.pds_data_fp)) {
        fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        (*io_count)++; 
        if (matcher(rec, non_ndx_key) == 0) { // If matcher returns success (0 indicates success)
            int off = ftell(repo_handle.pds_data_fp) - repo_handle.rec_size;
            if (non_ndx_delete_finder(repo_handle.ndx_root, off)){ 
                *io_count = -1;
                return PDS_REC_NOT_FOUND;
            }
            return PDS_SUCCESS; // Return success
        }
    }
    return PDS_REC_NOT_FOUND;
}

// //////////////////////////////////////////////

// Do a linear search of the given key in the linked data file
int get_linked_rec_by_key( int key, void *rec ){
    int i;
    fseek(repo_handle.pds_linked_data_fp, 0, SEEK_SET);
    int read_key = 0;
    for (i = 0; i < repo_handle.rec_count; i++) {
            fread(&read_key, sizeof(int), 1, repo_handle.pds_linked_data_fp);
            fread(rec, repo_handle.linked_rec_size, 1, repo_handle.pds_linked_data_fp);
        if (read_key == key) {
            return PDS_SUCCESS;
        }
    }
    return PDS_REC_NOT_FOUND;
}

// //////////////////////////////////////////////



// harddisk linear so no pointer so no linkedlist approach use
// No concept of free so space doesnt work no fdelete()
// normal array deletion not worth it O(N)
// delete by ndx_key instead use persistent data i.e. key,offset,is_deleted 
int delete_rec_by_ndx_key( int key ){
    struct BST_Node *node = bst_search(repo_handle.ndx_root, key);
    if (node == NULL) {
        return PDS_FILE_ERROR; // Record not found
    }
    struct PDS_NdxInfo*info=(struct PDS_NdxInfo*)node->data;
    if(info->is_deleted == 1)
        return PDS_FILE_ERROR;
    info->is_deleted = 1;
    return PDS_SUCCESS;
}

// UNDELETE
// DEfragmentation
// Reuse deleted space in put_rec

// pds_link_rec - NEW
// Create PDS_link_info instance based on key1 and key2
// Go to the end of the link file
// Store the PDS_link_info record
// int pds_link_rec(int key1, int key2) {
//     struct PDS_link_info link_info;

//     // Create the PDS_link_info instance
//     link_info.parent_key = key1;
//     link_info.child_key = key2;

//     fseek(repo_handle.pds_link_fp, 0, SEEK_END);
//     // Write the link information to the end of the link file
//     fwrite(&link_info, sizeof(struct PDS_link_info), 1, repo_handle.pds_link_fp);
//     return PDS_SUCCESS; // Return success code
// }
int pds_link_rec(int key1, int key2) {
    if(repo_handle.repo_status == PDS_REPO_CLOSED || repo_handle.repo_status == PDS_REPO_ALREADY_OPEN)
    {
        return PDS_ADD_FAILED;
    }
    fseek(repo_handle.pds_link_fp,0,SEEK_END);
	struct BST_Node* exist;
	exist=bst_search(repo_handle.ndx_root, key1);
	if(exist==NULL || exist->key!=key1 || ((struct PDS_NdxInfo*)(exist->data))->is_deleted==1)
	{
		return PDS_ADD_FAILED;
	}
    struct PDS_link_info link_info;
    // Create the PDS_link_info instance
    link_info.parent_key = key1;
    link_info.child_key = key2;
    


    rewind(repo_handle.pds_linked_data_fp);
	int key;
	while(fread(&key,sizeof(int),1,repo_handle.pds_linked_data_fp))
	{
		if(key==key2)
		{
			int a=fwrite(&key1,sizeof(int),1,repo_handle.pds_link_fp);
			int b=fwrite(&key2,sizeof(int),1,repo_handle.pds_link_fp);
			if(a==1 && b==1)
			{
				return PDS_SUCCESS;
			}
		}
		fseek(repo_handle.pds_linked_data_fp,repo_handle.linked_rec_size,SEEK_CUR);
	}
    return PDS_FILE_ERROR; // Return failure if key2 is not found
}

// pds_get_linked_rec
// Go to the beginning of the link file
// Reset result_set_size to 0
// Do a linear search of all link_info records for matching the given parent_key
// Store the matching linked key in linked_keys_result array
// Keep updating the result_set_size
int pds_get_linked_rec(int parent_key, int linked_keys_result[], int *result_set_size) {
	struct BST_Node* exist;
	exist=bst_search(repo_handle.ndx_root, parent_key);
	if(exist==NULL || exist->key!=parent_key || ((struct PDS_NdxInfo*)(exist->data))->is_deleted==1)
	{
		return PDS_REC_NOT_FOUND;
	}
    struct PDS_link_info link_info;
    *result_set_size = 0;
    fseek(repo_handle.pds_link_fp, 0, SEEK_SET);
    // Linear search through the link file for matching parent_key
    while (fread(&link_info, sizeof(struct PDS_link_info), 1, repo_handle.pds_link_fp) == 1) {
        if (link_info.parent_key == parent_key) {
            linked_keys_result[*result_set_size] = link_info.child_key;
            (*result_set_size)++;
        }
    }
    return PDS_SUCCESS; // Return success code
}


int pds_close() {
    char filename[30];
    strcpy(filename, repo_handle.pds_name);
    strcat(filename, ".ndx");
    repo_handle.pds_ndx_fp = fopen(filename, "wb");
    if (repo_handle.pds_ndx_fp == NULL) {
        return PDS_FILE_ERROR;
    }
    fwrite(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
    pre_order(repo_handle.ndx_root); // Write index information
    fclose(repo_handle.pds_data_fp);
    fclose(repo_handle.pds_ndx_fp);
    fclose(repo_handle.pds_link_fp);
    fclose(repo_handle.pds_linked_data_fp);
    bst_destroy(repo_handle.ndx_root);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}