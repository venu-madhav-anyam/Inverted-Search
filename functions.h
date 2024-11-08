#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include "inverted_search.h"


void create_hash_table(hash_t hash_table[]);
Status validate_files(char *argv[],int argc);
Status open_files(char *argv);
Status create_database(files_list **head, hash_t *hash_table);
Status display_database(hash_t *hast_table);
Status search_word(char *word, hash_t *hash_table);
Status save_database(hash_t *hash_table);
Status update_database(char *file, hash_t *hash_table);

#endif // FUNCTIONS_H
