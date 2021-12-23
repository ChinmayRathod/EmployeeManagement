/*
	The code written below is for the management of employees and the houses they live in.
	The code has different functionalities as in one can edit the inserted data, have a special request for allocation and so on.
	The data structure used in this system is linked list and the two major part being "struct record" and "struct sorted".
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SUCCESS 1		
#define FAILURE 0		
#define HOUSE 4			//the number of buildings in the campus
#define FLAT 10			//the number of flats in each building
#define NO_FLAT_AVAILABLE -1	

typedef int status;		//used for the regulation of output from the functions

//This stores all the data of the employees in a linked list format
typedef struct record{
	char firstname[50];
	char lastname[50];
	char address[100];
	char accommodation_type[20];
	int idtype;
	struct idnum
	{
		char aadhaar[15];
		char passport[15];
		char empcode [15];
	}id;
	struct record* next;
}record;

//This stores all the sorted order of the employees wrt to their names in a linked list format.
typedef struct sorted{
	record* data;
	struct sorted* next;
}sorted;

//main functions for the system
status insert(record* node,record* record_head,sorted* sorted_head);
int removeDuplicates(record* record_head,sorted* sorted_head,int count,int* vacancy);
status print(record* record_head);
status print_sort(sorted* sorted_head);
status search(char fname[],char lname[],sorted* sorted_head);
status deleteRecord(record* record_head,sorted* sorted_head,int* vacancy);
status update(record* record_head,sorted* sorted_head,int* vacancy);
status special_request_allocation(record* record_head,sorted* sorted_head,int* vacancy);

//helper functions for the system
record* input(int* vacancy);
status input_id(record* node);
status sort(record* data,sorted* sorted_head);
status remove_record(record* node,record* record_head);
status remove_sort(record* node,sorted* sorted_head);
status print_one(record* run);
status print_sorted_one(sorted* run);
status house_allot(record* node,int* vacancy);
status house_remove(record* node,int* vacancy);
status free_record(record* node);
status free_sorted(sorted* node);
record* search_return_id(record* node,sorted* sorted_head);

//Driver function
int main()
{
	int loop=1,process=0,output=0;
	
	record* record_head=(record*)malloc(sizeof(record));
	sorted* sorted_head=(sorted*)malloc(sizeof(sorted));
	
	record_head->next=NULL;
	sorted_head->next=NULL;
	
	int* vacancy=(int *)malloc(sizeof(int)*HOUSE);
	
	int i;
	
	for(i=0;i<HOUSE;i++)
		vacancy[i]=0;
		
	printf("\t\t\t\t\tInsert some data into the database\n");
	record* node=input(vacancy);
	loop=insert(node,record_head,sorted_head);
	printf("\nThe record was successfully added to the database.\n");
	while(loop)
	{
		printf("\nEnter the number of the process which you want to carry out:\n1. Insert a new record.\n2. Remove duplicates.\n3. Print the records.\n4. Sort the records and print.\n");
		printf("5. Search for a certain record.\n6. Delete a record.\n7. Update a record.\n8. Request for block change.\n9. Exit.\n   Input: ");
		scanf("%d",&process);
		switch(process)
		{
			case 1:
			{
				node=input(vacancy);
				if(node==NULL)
					printf("\n\t\t\t\t\tNo new records can be inserted as all the accommodations are full.\n");
				else
				{
					output=insert(node,record_head,sorted_head);
					if(output==SUCCESS)
						printf("\nThe record was successfully added to the database.\n");
					else
						printf("\nThe record was not entered into the database.\n");
				}
				break;
			}
			case 2:
			{
				output=removeDuplicates(record_head,sorted_head,0,vacancy);
				if(output==SUCCESS)
					printf("\nThere was %d duplicate record and it was sucessfully removed from the database.\n",output);	
				else if(output>SUCCESS)
					printf("\nThere were %d duplicate records and they were sucessfully removed from the database.\n",output);
				else
					printf("\nThere were no duplicate records in the database.\n");
				break;
			}
			case 3:
			{
				print(record_head);
				printf("\nThe records are displayed above.\n");
				break;
			}
			case 4:
			{
				print_sort(sorted_head);
				printf("\nThe alphabetically sorted records are displayed above.\n");
				break;
			}
			case 5:
			{
				char fname[50],lname[50];
				fname[0]='\0',lname[0]='\0';
				printf("\nPlease enter the firstname of the person to be searched in the database:");
				scanf("%s",fname);
				printf("Please enter the lastname of the person to be searched in the database:");
				scanf("%s",lname);
				output=search(fname,lname,sorted_head);
				if(output==SUCCESS)
					printf("\nThe occurances of poeple with name '%s %s' in the database are displayed above.\n",fname,lname);
				else
					printf("\nThe name '%s %s' does not exist in the current database.\n",fname,lname);
				break;
			}
			case 6:
			{
				output=deleteRecord(record_head,sorted_head,vacancy);
				if(output==SUCCESS)
					printf("\nThe record was successfully deleted from the database.\n");
				else
					printf("\nThe record does not exist in the current database.\n");
				break;
			}
			case 7:
			{
				output=update(record_head,sorted_head,vacancy);
				if(output==SUCCESS)
					printf("\nThe record was successfully updated in the database.\n");
				else
					printf("\nThe record does not exist in the current database.\n");
				break;
			}
			case 8:
			{
				output=special_request_allocation(record_head,sorted_head,vacancy);
				if(output==SUCCESS)
					printf("\nCongrats!!!The desired block has been allocated to you.\n");
				else if(output==0)
					printf("\nThe record does not exist in the current database.\n");
				else
					printf("\nSorry!!!The desired block is full.\n");
				break;
			}
			case 9:
			{
				loop=0;
				printf("\n\t\t\t\t\tThank you!Have a great day ahead!!!\n");
				break;
			}
			default:
			{
				printf("\nEnter a number from the range of options given only(1-9).\n");
				break;
			}
		}
	}
	
	free_record(record_head);
	free_sorted(sorted_head);
	
	return 0;
}

//main functions for the system
status insert(record* node,record* record_head,sorted* sorted_head) 
{
	//This function takes the node and inserts it in its proper sorted place and also in the records.
	record* run=record_head;
	node->next=NULL;
	while(run->next!=NULL)		//search for the last element
		run=run->next;
	run->next=node;				//new data is added to the end of the Linked List
	return sort(node,sorted_head);	//sorts and places the record in its right place
}

int removeDuplicates(record* record_head,sorted* sorted_head,int count,int* vacancy)
{
	//This function removes all the duplicates present in the database wrt the id-type and the respective number.
	record* run=record_head;
	record* node;
	int found=0;
	while(run->next!=NULL)// The data that is being compared with the next in the link
	{
		node=run->next;	//The data that is nezt in the link
		while(node->next!=NULL)
		{
			if(run->next->idtype==node->next->idtype)	//comparing the id-types
			{
				//If the id-types are same comparing the respective id numbers
				if(run->next->idtype==0&&strcmp(run->next->id.aadhaar,node->next->id.aadhaar)==0)
				{
					found=1;
				}
				else if(run->next->idtype==1&&strcmp(run->next->id.passport,node->next->id.passport)==0)
				{
					found=1;
				}
				else if(run->next->idtype==2&&strcmp(run->next->id.empcode,node->next->id.empcode)==0)
				{
					found=1;
				}
				//When a duplicate is found
				if(found==1)
				{
					count++;		//increments for every duplicate found
					house_remove(node->next,vacancy);	//removes the allottment of the flat
					remove_sort(node->next,sorted_head);	//removes the data of the employee from sorted Linked List
					remove_record(node->next,record_head);	//reomves the data of the employee from the Linked List
				}
			}
			if(found==0&&node->next!=NULL)	//go to next if not duplicate
				node=node->next;
			else		
				found=0;	//found must be made 1 to find other duplicates
		}
		if(run->next!=NULL)
			run=run->next;
	}
	return count;		//The number of duplicates removed is returned
}

status print(record* record_head)
{
	//This function prints the data of the employees.
	record* run=record_head;
	status value=1;
	while(run->next!=NULL)
	{
		run=run->next;
		if(value)
			value=print_one(run);	//To print a single record
		else
			return FAILURE;
	}
	return SUCCESS;
}

status print_sort(sorted* sorted_head)
{
	//This function prints the data of the employees in alphabetically sorted manner.
	int value=1;
	sorted* run=sorted_head;
	while(run->next!=NULL)
	{
		run=run->next;
		if(value)
			print_sorted_one(run);	//To print a single sorted record
		else
			return FAILURE;
	}
	return SUCCESS;
}

status search(char fname[],char lname[],sorted* sorted_head)
{
	//This function searches the sorted Linked List to find all the occurances of the names.
	sorted* run=sorted_head;
	int count=0;
	while( run->next!=NULL)
	{
		if(strcmp(run->next->data->firstname,fname)==0&&strcmp(run->next->data->lastname,lname)==0)
		{
			print_sorted_one(run->next);
			count++;
		}
		run=run->next;
	}
	if(count>0)
		return SUCCESS;
	else
		return FAILURE;
}

status deleteRecord(record* record_head,sorted* sorted_head,int* vacancy)
{
	//This function deletes the data of the employees from the database.
	record* node=(record*)malloc(sizeof(record));
	input_id(node);								//takes the id of the employee to be deleted
	node=search_return_id(node,sorted_head);	//searches for the node of the employee
	if(node!=NULL)								//if employee exists
	{
		house_remove(node,vacancy);				//removes the flat from the employee
		remove_sort(node,sorted_head);			//removes data of employee from the sorted Linked List
		remove_record(node,record_head);		//removes data of employee from the record Linked List
		return SUCCESS;
	}	
	else										//if employee doesn't exist
		return FAILURE;
}

status update(record* record_head,sorted* sorted_head,int* vacancy)
{
	//This function is used to update a data of the employee
	record* node=(record*)malloc(sizeof(record));
	input_id(node);								//takes the id of the employee to be updated
	node=search_return_id(node,sorted_head);	//searches for the node of the employee
	if(node!=NULL)								//if employee exists
	{
		printf("\nThe record is:\n");
		print_one(node);						//The record of the employee
		int option,loop=1;
		while(loop)
		{
			printf("Enter the number in front of the field you want to edit:\n1. Firstname\n2. Lastname\n3. Accommodation\n4. ID\n5. Address\n");
			scanf("%d",&option);
			switch(option)
			{
				case 1:					//To edit the firstname of the employee
				{
					remove_sort(node,sorted_head);		//removes data of employee from the sorted Linked List
					remove_record(node,record_head);	//removes data of employee from the record Linked List
					printf("Enter the new firstname:");
					scanf("%s",node->firstname);		//new firstname added
					insert(node,record_head,sorted_head);	//The node will be inserted as a new node
					loop=0;
					break;
				}
				case 2:					//To edit the lastname of the employee
				{
					remove_sort(node,sorted_head);			//removes data of employee from the sorted Linked List
					remove_record(node,record_head);		//removes data of employee from the record Linked List
					printf("Enter the new lastname:");	
					scanf("%s",node->lastname);				//new lastname added
					insert(node,record_head,sorted_head);	//The node will be inserted as a new node
					loop=0;
					break;
				}
				case 3:
				{
					int allotted=0,count=0;
					char accommodation_type_var[20];
					house_remove(node,vacancy);				//removes the flat from the employee
					strcpy(accommodation_type_var,node->accommodation_type);
					while(allotted==0&&count<4)
					{
						count++;
						printf("Please enter your Accommodation Type(I,II,III,IV):");
						scanf("%s",node->accommodation_type);
						allotted=house_allot(node,vacancy);		//checks if there is vacancy or not then allots the flat
						if(allotted==-1)
						{
							printf("\nWe are sorry this block has no vacancies,please choose another block.\n");
							allotted=0;
						}
					}
					if(allotted==0&&count==4)
					{
						strcpy(node->accommodation_type,accommodation_type_var);
						house_allot(node,vacancy);
						printf("\nWe are very sorry but all the accommodations are full,there are no vacancies.\n");
					}
					loop=0;
					break;
				}
				case 4:
				{
					input_id(node);		//changes the id details of the employee
					loop=0;
					break;
				}
				case 5:
				{
					printf("Enter the new Address:");
					scanf("%s",node->address);		//changes the address of the employee
					loop=0;
					break;
				}
				default:
				{
					printf("Please enter an option from the given range only.(1-5)\n");
					break;
				}
			}
		}
		return SUCCESS;
	}
	else											//if employee doesn't exist
		return FAILURE;
}

status special_request_allocation(record* record_head,sorted* sorted_head,int* vacancy)
{
	//This function to reallocate the employee of he wants to and if there are vacancies in the buildings
	record* node=(record*)malloc(sizeof(record));
	input_id(node);										//takes the id of the employee to be updated
	node=search_return_id(node,sorted_head);			//searches for the node of the employee
	if(node!=NULL)										//if employee exists
	{
		printf("\nThe record is:\n");
		print_one(node);
		int allotted=1,count=0;
		char accommodation_type_var[20];
		house_remove(node,vacancy);									//removes the flat from the employee
		strcpy(accommodation_type_var,node->accommodation_type);
		printf("Please enter your Accommodation Type(I,II,III,IV):");
		scanf("%s",node->accommodation_type);
		allotted=house_allot(node,vacancy);					//checks if there is vacancy or not then allots the flat
		if(allotted==NO_FLAT_AVAILABLE)
		{
			strcpy(node->accommodation_type,accommodation_type_var);
			house_allot(node,vacancy);
			return NO_FLAT_AVAILABLE;
		}
	}
	else												//if employee doesn't exist
		return FAILURE;
}

//helper functions for the system
record* search_return_id(record* node,sorted* sorted_head)
{
	//This function searches for the employee with his id details and then returns his node
	sorted* run=sorted_head;
	while(run->next!=NULL)
	{
		if(run->next->data->idtype==node->idtype)
		{
			if(node->idtype==0)
			{
				if(strcmp(run->next->data->id.aadhaar,node->id.aadhaar)==0)
					return run->next->data;
			}
			else if(node->idtype==1)
			{
				if(strcmp(run->next->data->id.passport,node->id.passport)==0)
					return run->next->data;
			}
			else if(node->idtype==2)
			{
				if(strcmp(run->next->data->id.empcode,node->id.empcode)==0)
					return run->next->data;
			}
		}
		run=run->next;
	}
	return	NULL;	//if the employee doesn't exist
}

status remove_sort(record* node,sorted* sorted_head)
{
	//This function is used to remove the element from the sorted Linked List
	sorted* run=sorted_head;
	while(run->next!=NULL)
	{
		if(run->next->data==node)
		{
			sorted* temp=run->next;
			run->next=run->next->next;
			free_sorted(temp);
		}
		if(run->next!=NULL)
			run=run->next;
	}
	return SUCCESS;
}

status remove_record(record* node,record* record_head)
{
	//This function is used to remove the node from the record Linked List
	record* run=record_head;
	while(run->next!=NULL)
	{
		if(node==run->next)
		{
			record* temp=run->next;
			run->next=run->next->next;
			free_record(temp);
		}
		if(run->next!=NULL)
			run=run->next;
	}
	return SUCCESS;
}

status sort(record* data,sorted* sorted_head)
{
	//This function is used to place the new node entered to its proper place aplhabetically 
	sorted* run=sorted_head;
	sorted* node=(sorted*)malloc(sizeof(sorted));
	node->data=data;
	node->next=NULL;
	while( run->next!=NULL)
	{
		if(strcmp(run->next->data->firstname,data->firstname)<0)
			run=run->next;
		else if(strcmp(run->next->data->firstname,data->firstname)>0)
			break;
		else				//if the firstnames are the same compare the lastnames
		{
			if(strcmp(run->next->data->lastname,data->lastname)<=0)	
				run=run->next;
			else if(strcmp(run->next->data->lastname,data->lastname)>0)
				break;
		}		//if the lastnames are the same place it just after it
	}	
	if(run->next==NULL)
		run->next=node;
	else
	{
		node->next=run->next;
		run->next=node;
	}
	return SUCCESS;
}

status print_one(record* run)
{
	//This function is used to print the details from the record Linked List
	if(run->idtype==0)//printing the records with aadhaar information
	{
		printf("\nName:%s %s\n",run->firstname,run->lastname);
		printf("Accommodation type:%s\nAadhaar Number: %s\nAddress: %s\n\n",run->accommodation_type,run->id.aadhaar,run->address);
	}
	else if(run->idtype==1)//printing the records with passport information
	{
		printf("\nName:%s %s\n",run->firstname,run->lastname);
		printf("Accommodation type:%s\nPassport Number: %s\nAddress: %s\n\n",run->accommodation_type,run->id.passport,run->address);
	}
	else if(run->idtype==2)//printing the records with Employee code information
	{
		printf("\nName:%s %s\n",run->firstname,run->lastname);
		printf("Accommodation type:%s\nEmployee Code: %s\nAddress: %s\n\n",run->accommodation_type,run->id.empcode,run->address);
	}
	else
		return FAILURE;
}

status print_sorted_one(sorted* run)
{
	return print_one(run->data);
}

status house_allot(record* node,int* vacancy)
{
	//This function takes care of the allottment and the filling of the flats
	if(strcmp(node->accommodation_type,"I")==0&&vacancy[0]<FLAT)
		vacancy[0]++;
	else if(strcmp(node->accommodation_type,"II")==0&&vacancy[1]<FLAT)
		vacancy[1]++;
	else if(strcmp(node->accommodation_type,"III")==0&&vacancy[2]<FLAT)
		vacancy[2]++;
	else if(strcmp(node->accommodation_type,"IV")==0&&vacancy[3]<FLAT)
		vacancy[3]++;
	else
	{
		return NO_FLAT_AVAILABLE;
	}
	return SUCCESS;
}

status house_remove(record* node,int* vacancy)
{
	//This function takes care of the leaving of the employees from their flats
	if(strcmp(node->accommodation_type,"I")==0)
		vacancy[0]--;
	else if(strcmp(node->accommodation_type,"II")==0)
		vacancy[1]--;
	else if(strcmp(node->accommodation_type,"III")==0)
		vacancy[2]--;
	else if(strcmp(node->accommodation_type,"IV")==0)
		vacancy[3]--;
	return SUCCESS;
}

status free_record(record* node)
{
	//This function frees space occupied by a record node
	node->next=NULL;
	free(node);
	node=NULL;
	return SUCCESS;
}

status free_sorted(sorted* node)
{
	//This function frees space occupied by a sorted node
	node->next=NULL;
	node->data=NULL;
	free(node);
	node=NULL;
	return SUCCESS;
}

record* input(int* vacancy)
{
	//This function takes the input from the user and returns it in the form of a record node 
	int allotted=0,count=0;
	record* node=(record*)malloc(sizeof(record));
	printf("\nPlease enter the firstname:");
	scanf("%s",node->firstname);
	printf("Please enter the lastname:");
	scanf("%s",node->lastname);
	printf("Please enter your Address:");
	scanf("%s",node->address);
	while(allotted==0&&count<4)
	{
		count++;
		printf("Please enter your Accommodation Type(I,II,III,IV):");
		scanf("%s",node->accommodation_type);
		allotted=house_allot(node,vacancy);
		if(allotted==-1)
		{
			printf("\nWe are sorry this block has no vacancies,please choose another block.\n");
			allotted=0;
		}
	}
	if(count==4&&allotted==0)
	{
		printf("No accommodations are available.\n");
		node=NULL;
	}
	else
	{
		input_id(node);
	}
	return node;
}

status input_id(record* node)
{
		//This function takes the id details of the employee as input
		int id_flag=0;
		while(id_flag==0)
		{
			printf("Please choose the idtype:\n0. Aadhaar number\n1. Passport number\n2. Employee code\n   Option: ");
			scanf("%d",&node->idtype);
			if(node->idtype==0)
			{
				printf("Please enter the Aadhaar number:");
				scanf("%s",node->id.aadhaar);
				id_flag=1;
			}
			else if(node->idtype==1)
			{
				printf("Please enter the Passport number:");
				scanf("%s",node->id.passport);
				id_flag=1;
			}
			else if(node->idtype==2)
			{
				printf("Please enter the Empcode number:");
				scanf("%s",node->id.empcode);
				id_flag=1;
			}
			else
				printf("Please enter a valid ID type number(0,1 or 2):\n");
		}
		return SUCCESS;
}