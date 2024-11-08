/*
Name:- Bala Balaji Venu Madhav Anyam
Date:- 28/11/24
Description:- Inveted search Using DSA-Hashing
*/

#include <stdio.h>
#include <stdlib.h>
#include "inverted_search.h"
#include "functions.h"

files_list *head = NULL;
int update, create;

int main(int argc, char *argv[])
{

	if (argc <= 1)
	{
		printf(RED "Error : Invalid no. of arguments\nUsage : ./a.out <file> [file] [file] ...\n");
		return 1;
	}

	hash_t hash_table[28];

	validate_files(argv, argc);
	create_hash_table(hash_table);

	while (1)
	{
		int choice;

		printf(BLUE "\n1. Create Database\n"
					"2. Display Database\n"
					"3. Search Database\n"
					"4. Save Database\n"
					"5. Update Database\n"
					"6. Exit the Program\n" WHITE "Enter Your Choice : ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			if (create)
			{
				printf(RED "\nError : The Files passed through command line are already added into the database\n"
						   "Can't create database again\n");
				break;
			}

			if (create_database(&head, hash_table) == FAILURE)
				printf(RED "\nError : Creating Database Failed.\n");
			else
				printf(GREEN "\nInfo : Database Created Successfully.\n");
			break;

		case 2:
			if (display_database(hash_table) == FAILURE)
				printf(RED "\nDatabase is Empty.\n");
			break;

		case 3:
			printf(WHITE "Enter the data :");
			char word[9];
			scanf("%s", word);

			if (search_word(word, hash_table) == FAILURE)
				printf(RED "\nData Not Found.\n");
			break;

		case 4:
			if (save_database(hash_table) == SUCCESS)
				printf(GREEN "\nInfo : Database Saved Successfully\n");
			break;

		case 5:

			if (create)
			{
				printf(RED "\nError : Update Database Is Not Possible After Create Database\n");
				break;
			}

			if (update)
			{
				printf(RED "\nError : Update Database Is Already Performed\n");
				break;
			}

			printf(WHITE "Enter the Backup File Name:");
			char file[15];
			scanf("%s", file);

			if (update_database(file, hash_table) != FAILURE)
				printf(GREEN "\nInfo : Database is Updated Successfully with %s File\n", file);
			else
				printf(RED "Error : Database Update Failed\n");
			break;

		case 6:
			exit(0);
			break;

		default:
			printf(RED "Error : Enter a Valid Option..!\n");
			break;
		}

		getchar();
	}
}
