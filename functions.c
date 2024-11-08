#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inverted_search.h"
#include "functions.h"

extern files_list *head;
extern int update, create;

// Creating the hast table and initializing
void create_hash_table(hash_t hash_table[])
{
    for (int i = 0; i < 28; i++)
    {
        hash_table[i].index = i;
        hash_table[i].link = NULL;
    }
}

Status validate_files(char *argv[], int argc)
{
    int i = 1, duplicate = 1;

    for (int i = 1; i < argc; i++)
    {

        if (open_files(argv[i]) == SUCCESS)
        {
            files_list *new = malloc(sizeof(files_list));

            if (!new)
                return FAILURE;

            if (!head)
            {
                strcpy(new->file_name, argv[i]);
                head = new;
                new->link = NULL;
                printf(GREEN "Info : File " RED " %s " GREEN " is Successfully Added to the List.\n", new->file_name);
            }
            else
            {
                files_list *temp = head, *prev = NULL;

                while (temp != NULL)
                {
                    if (strcmp(temp->file_name, argv[i]) == 0)
                    {
                        duplicate = 0;
                        printf(RED "Error : File " GREEN " %s " RED " is repeated.\nSo, We Are Not Adding This into the List.\n", argv[i]);
                        free(new);
                        break;
                    }

                    prev = temp;
                    temp = temp->link;
                }

                if (duplicate)
                {
                    strcpy(new->file_name, argv[i]);
                    prev->link = new;
                    new->link = NULL;

                    printf(GREEN "Info : File " RED " %s " GREEN " is Successfully Added to the List.\n", new->file_name);
                }
                duplicate = 1;
            }
        }
    }
}

// validating file and it's data
Status open_files(char *argv)
{

    if (strstr(argv, ".") != NULL && strcmp((strstr(argv, ".")), ".txt") == 0)
    {
        FILE *fptr = fopen(argv, "r");

        if (fptr != NULL)
        {
            fseek(fptr, 0, SEEK_END);
            if (ftell(fptr) != 0)
            {
                fclose(fptr);
                return SUCCESS;
            }
            else
            {
                printf(RED "\nError :file " GREEN " %s " RED " is Empty.\n", argv);
                return FAILURE;
            }
        }
        else
        {
            printf(RED "\nError : Entered File" GREEN " %s " RED "does not exist.\nPlease Enter Existing Files.\n", argv);
            return FAILURE;
        }
    }
    else
    {

        printf(RED "\nError : File " GREEN " %s " RED " is Not a .txt File.\nPlease Pass the Files with .txt Extension.\n", argv);
        return FAILURE;
    }
}

// Creating the database with given files
Status create_database(files_list **head, hash_t *hash_table)
{
    files_list *list = *head, *prev = NULL, *del;
    main_node *node;
    sub_node *sub;
    int ind;

    while (list)
    {
        for (int i = 0; i < 27; i++)
        {
            main_node *mchk = hash_table[i].link;
            while (mchk)
            {
                sub_node *schk = mchk->sub_node_link;
                while (schk)
                {

                    if (strcmp(list->file_name, schk->file_name) == 0)
                    {
                        printf(RED "Note : " GREEN "%s" RED " Data Is Already Presnt In Database\nSo, It Is Not Considering While Creation\n", list->file_name);

                        del = list;
                        if (!prev)
                        {
                            *head = list->link;
                            list = list->link;
                        }
                        else
                        {
                            prev->link = list->link;
                            list = list->link;
                        }
                        free(del);
                        break;
                    }
                    schk = schk->sub_node_link;
                }
                mchk = mchk->main_node_link;
            }
            if (list == NULL)
                break;
        }

        if (list != NULL)
        {
            prev = list;
            list = list->link;
        }
    }

    list = *head;

    while (list)
    {
        printf(GREEN "Processing file: %s\n", list->file_name);
        FILE *fileptr = fopen(list->file_name, "r");
        if (!fileptr)
            return FAILURE;

        char word[20];

        while (fscanf(fileptr, "%s", word) == 1)
        {

            if (word[0] > 64 && word[0] < 91)
                ind = word[0] - 65;
            else if (word[0] > 96 && word[0] < 123)
                ind = word[0] - 97;
            else if (word[0] > 47 && word[0] < 58)
                ind = 26;
            else
                ind = 27;

            node = hash_table[ind].link;

            while (node)
            {
                if (strcmp(node->word, word) == 0)
                {
                    sub = node->sub_node_link;

                    while (sub)
                    {
                        if (strcmp(sub->file_name, list->file_name) == 0)
                        {
                            sub->word_count++;
                            break;
                        }

                        if (sub->sub_node_link == NULL)
                        {
                            sub_node *new_sub = malloc(sizeof(sub_node));
                            if (!new_sub)
                            {
                                fclose(fileptr);
                                return FAILURE;
                            }

                            strcpy(new_sub->file_name, list->file_name);
                            new_sub->word_count = 1;
                            new_sub->sub_node_link = NULL;
                            sub->sub_node_link = new_sub;
                            node->file_count++;
                            break;
                        }
                        sub = sub->sub_node_link;
                    }
                    break;
                }

                if (node->main_node_link == NULL)
                {
                    main_node *new_main = malloc(sizeof(main_node));
                    sub_node *new_sub = malloc(sizeof(sub_node));

                    if (!new_main || !new_sub)
                    {
                        fclose(fileptr);
                        return FAILURE;
                    }

                    strcpy(new_main->word, word);
                    new_main->file_count = 1;
                    new_main->main_node_link = NULL;
                    new_main->sub_node_link = new_sub;

                    strcpy(new_sub->file_name, list->file_name);
                    new_sub->word_count = 1;
                    new_sub->sub_node_link = NULL;

                    node->main_node_link = new_main;
                    break;
                }
                node = node->main_node_link;
            }

            if (!hash_table[ind].link)
            {
                main_node *new_main = malloc(sizeof(main_node));
                sub_node *new_sub = malloc(sizeof(sub_node));

                if (!new_main || !new_sub)
                {
                    fclose(fileptr);
                    return FAILURE;
                }

                strcpy(new_main->word, word);
                new_main->file_count = 1;
                new_main->main_node_link = NULL;
                new_main->sub_node_link = new_sub;

                strcpy(new_sub->file_name, list->file_name);
                new_sub->word_count = 1;
                new_sub->sub_node_link = NULL;

                hash_table[ind].link = new_main;
            }
        }

        fclose(fileptr);
        prev = list;
        list = list->link;
    }

    create = 1;
    return SUCCESS;
}

// Displying Contents of database
Status display_database(hash_t *hash_table)
{
    int once = 1, no = 0;

    printf(WHITE "\t\t\t#-DATABASE-#\n");

    for (int i = 0; i < 28; i++)
    {
        if (hash_table[i].link)
        {
            main_node *node = hash_table[i].link;

            while (node)
            {
                if (once)
                    printf(GREEN "[%d]", i), once = 0;
                else
                    printf("   ");

                if (i < 10)
                    printf(" ");

                printf(GREEN "\t%s\t%d File(s) |  ", node->word, node->file_count);
                sub_node *sub = node->sub_node_link;

                while (sub)
                {
                    printf(GREEN "%s:- %d time(s)|  ", sub->file_name, sub->word_count);

                    sub = sub->sub_node_link;
                }

                printf(GREEN "-> NULL\n");
                no = 1;
                node = node->main_node_link;
            }

            once = 1;
        }
    }

    if (no)
        return SUCCESS;
    else
        return FAILURE;
}

// Searching the data in database with user input data
Status search_word(char *word, hash_t *hash_table)
{
    int ind, found = 0, len;

    len = strlen(word);

    if (word[0] > 64 && word[0] < 91)
        ind = word[0] - 65;
    else if (word[0] > 96 && word[0] < 123)
        ind = word[0] - 97;
    else if (word[0] > 47 && word[0] < 58)
        ind = 26;
    else
        ind = 27;

    if (hash_table[ind].link)
    {
        main_node *node = hash_table[ind].link;

        while (node)
        {
            if (strncmp(word, node->word, len) == 0)
            {
                found = 1;
                printf(GREEN "[%d]\t%s\t%d File(s) |  ", ind, node->word, node->file_count);
                sub_node *sub = node->sub_node_link;

                while (sub)
                {
                    printf(GREEN "%-2s: %2d time(s)  ", sub->file_name, sub->word_count);
                    sub = sub->sub_node_link;
                }

                printf(GREEN "-> NULL\n");
            }

            node = node->main_node_link;
        }
    }

    if (found)
        return SUCCESS;
    else
        return FAILURE;
}

// Saving data to a backup file
Status save_database(hash_t *hash_table)
{
    if (!create && !update)
    {
        printf(RED "\nError : Database is empty\nCan't perform save database operation\n");
        return FAILURE;
    }

    char name[15];

    printf(WHITE "Enter File Name :");
    scanf("%s", name);

    char *ext = strrchr(name, '.');

    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf(RED "\nError: File " GREEN "%s " RED "is Not a .txt File.\nPlease Enter the Name with .txt Extension.\n", name);
        return FAILURE;
    }
    FILE *sptr = fopen(name, "w");

    if (!sptr)
        return FAILURE;

    for (int i = 0; i < 28; i++)
    {
        if (hash_table[i].link)
        {
            main_node *node = hash_table[i].link;

            while (node)
            {

                fprintf(sptr, "#%d;%s;%d;", i, node->word, node->file_count);

                sub_node *sub = node->sub_node_link;

                while (sub)
                {
                    fprintf(sptr, "%s;%d;", sub->file_name, sub->word_count);
                    sub = sub->sub_node_link;
                }

                fprintf(sptr, "#\n");
                node = node->main_node_link;
            }
        }
    }

    fclose(sptr);
}

// updating backup data to database
Status update_database(char *file, hash_t *hash_table)
{
    char c, data[30];
    int ind;

    FILE *fptr = fopen(file, "r");

    if (fptr == NULL)
    {
        printf(RED "Error : File not found.\n");
        return FAILURE;
    }

    fscanf(fptr, "%[^\n];", data);
    int l = strlen(data);

    if (data[0] != '#' && data[l - 1] != '#')
    {
        printf(RED "Error : " GREEN "%s" RED " Is Not a Backup File.\n", file);
        fclose(fptr);
        return FAILURE;
    }

    fseek(fptr, 0, SEEK_SET);

    while (fscanf(fptr, "#%[^;];", data) != EOF)
    {
        ind = atoi(data);

        main_node *new_main = malloc(sizeof(main_node));
        main_node *node = hash_table[ind].link;
        sub_node *sub;

        if (new_main == NULL)
            return FAILURE;

        new_main->main_node_link = NULL;

        fscanf(fptr, "%[^;];", data);
        strcpy(new_main->word, data);
        fscanf(fptr, "%[^;];", data);
        new_main->file_count = atoi(data);

        for (int i = 0; i < (new_main->file_count); i++)
        {
            sub_node *new_sub = malloc(sizeof(sub_node));

            if (new_sub == NULL)
                return FAILURE;

            new_sub->sub_node_link = NULL;
            fscanf(fptr, "%[^;];", data);
            strcpy(new_sub->file_name, data);
            fscanf(fptr, "%[^;];", data);
            new_sub->word_count = atoi(data);

            if (new_main->sub_node_link == NULL)
            {
                new_main->sub_node_link = new_sub;
                sub = new_sub;
            }
            else
            {
                sub->sub_node_link = new_sub;
                sub = sub->sub_node_link;
            }
        }

        while (c = fgetc(fptr) != '\n')
            ;

        if (node == NULL)
            hash_table[ind].link = new_main;
        else
        {
            while (node->main_node_link)
            {
                node = node->main_node_link;
            }
            node->main_node_link = new_main;
        }
    }
    fclose(fptr);
    update = 1;
    return SUCCESS;
}
