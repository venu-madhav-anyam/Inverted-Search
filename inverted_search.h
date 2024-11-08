#ifndef INVERTED_SEARCH_H
#define INVERTED_SEARCH_H


#define RESET          "\033[0m"
#define RED       "\033[1;38;5;202m" 
#define GREEN     "\033[1;92m"       
#define BLUE      "\033[1;36m"       
#define WHITE     "\033[1;37m"  


typedef struct sub_node
{
    int word_count;
    char file_name[10];
    struct sub_node *sub_node_link;
} sub_node;

typedef struct main_node
{
    char word[10];
    int file_count;
    sub_node *sub_node_link;
    struct main_node *main_node_link;
} main_node;

typedef struct
{
    int index;
    struct main_node *link;
} hash_t;

typedef enum
{
    SUCCESS,
    FAILURE
} Status;

typedef struct files_list
{
    char file_name[10];
    struct files_list *link;
} files_list;

#endif