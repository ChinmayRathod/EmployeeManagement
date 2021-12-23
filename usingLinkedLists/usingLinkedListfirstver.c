#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SUCCESS 1
#define FAILURE 0
#define HOUSE 4
#define FLAT 1
#define NO_FLAT_AVAILABLE -1

typedef int status;

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

typedef struct sorted{
	record* data;
	struct sorted* next;
}sorted;

int removeDuplicates(record* record_head,sorted* sorted_head,int count,int* vacancy);
status special_request_allocation(record* record_head,sorted* sorted_head,int* vacancy);
status update(record* record_head,sorted* sorted_head,int* vacancy);
status house_remove(record* node,int* vacancy);
record* search_return_id(record* node,sorted* sorted_head);
status remove_sort(record* node,sorted* sorted_head);
status search(char fname[],char lname[],sorted* sorted_head);
record* input(int* vacancy);
status print_one(record* node);
int insert(record* node,record* record_head,sorted* sorted_head);
int deleteRecord(record* record_head,sorted* sorted_head,int* vacancy);
int print(record* record_head);
status house_allot(record* node,int* vacancy);

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

int removeDuplicates(record* record_head,sorted* sorted_head,int count,int* vacancy)
{
	record* run=record_head;
	record* node;
	int found=0;
	while(run->next!=NULL)
	{
		node=run->next;
		while(node->next!=NULL)
		{
			if(run->next->idtype==node->next->idtype)
			{
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
				if(found==1)
				{
					count++;
					house_remove(node->next,vacancy);
					remove_sort(node->next,sorted_head);
					remove_record(node->next,record_head);
				}
			}
			if(found==0&&node->next!=NULL)
				node=node->next;
			else
				found=1;
		}
		if(run->next!=NULL)
			run=run->next;
	}
	return count;
}

status special_request_allocation(record* record_head,sorted* sorted_head,int* vacancy)
{
	record* node=(record*)malloc(sizeof(record));
	input_id(node);
	node=search_return_id(node,sorted_head);
	if(node!=NULL)
	{
		printf("\nThe record is:\n");
		print_one(node);
		int allotted=1,count=0;
		char accommodation_type_var[20];
		house_remove(node,vacancy);
		strcpy(accommodation_type_var,node->accommodation_type);
		printf("Please enter your Accommodation Type(I,II,III,IV):");
		scanf("%s",node->accommodation_type);
		allotted=house_allot(node,vacancy);
		if(allotted==NO_FLAT_AVAILABLE)
		{
			strcpy(node->accommodation_type,accommodation_type_var);
			house_allot(node,vacancy);
			return NO_FLAT_AVAILABLE;
		}
	}
	else
		return FAILURE;
}

status update(record* record_head,sorted* sorted_head,int* vacancy)
{
	record* node=(record*)malloc(sizeof(record));
	input_id(node);
	node=search_return_id(node,sorted_head);
	if(node!=NULL)
	{
		printf("\nThe record is:\n");
		print_one(node);
		int option,loop=1;
		while(loop)
		{
			printf("Enter the number in front of the field you want to edit:\n1. Firstname\n2. Lastname\n3. Accommodation\n4. ID\n5. Address\n");
			scanf("%d",&option);
			switch(option)
			{
				case 1:
				{
					remove_sort(node,sorted_head);
					remove_record(node,record_head);
					printf("Enter the new firstname:");
					scanf("%s",node->firstname);
					insert(node,record_head,sorted_head);
					loop=0;
					break;
				}
				case 2:
				{
					remove_sort(node,sorted_head);
					remove_record(node,record_head);
					printf("Enter the new lastname:");
					scanf("%s",node->lastname);
					insert(node,record_head,sorted_head);
					loop=0;
					break;
				}
				case 3:
				{
					int allotted=0,count=0;
					char accommodation_type_var[20];
					house_remove(node,vacancy);
					strcpy(accommodation_type_var,node->accommodation_type);
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
					input_id(node);
					loop=0;
					break;
				}
				case 5:
				{
					printf("Enter the new Address:");
					scanf("%s",node->address);
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
	else
		return FAILURE;
}

record* search_return_id(record* node,sorted* sorted_head)
{
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
	return	NULL;	
}

status remove_sort(record* node,sorted* sorted_head)
{
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
	return 1;
}

status deleteRecord(record* record_head,sorted* sorted_head,int* vacancy)
{
	record* node=(record*)malloc(sizeof(record));
	input_id(node);
	node=search_return_id(node,sorted_head);
	if(node!=NULL)
	{
		house_remove(node,vacancy);
		remove_sort(node,sorted_head);
		remove_record(node,record_head);
		return SUCCESS;
	}
	else
		return FAILURE;
}

status search(char fname[],char lname[],sorted* sorted_head)
{
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

int print_sort(sorted* sorted_head)
{
	sorted* run=sorted_head;
	while(run->next!=NULL)
	{
		run=run->next;
		print_sorted_one(run);
	}
	return SUCCESS;
}

status sort(record* data,sorted* sorted_head)
{
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
		else
		{
			if(strcmp(run->next->data->lastname,data->lastname)<=0)
				run=run->next;
			else if(strcmp(run->next->data->lastname,data->lastname)>0)
				break;
		}
	}	
	if(run->next==NULL)
		run->next=node;
	else
	{
		node->next=run->next;
		run->next=node;
	}
	return 1;
}

int insert(record* node,record* record_head,sorted* sorted_head)
{
	record* run=record_head;
	node->next=NULL;
	while(run->next!=NULL)
		run=run->next;
	run->next=node;
	return sort(node,sorted_head);
}

status print_sorted_one(sorted* run)
{
	if(run->data->idtype==0)//printing the records with aadhaar information
	{
		printf("\nName:%s %s\n",run->data->firstname,run->data->lastname);
		printf("Accommodation type:%s\nAadhaar Number: %s\nAddress: %s\n",run->data->accommodation_type,run->data->id.aadhaar,run->data->address);
	}
	else if(run->data->idtype==1)//printing the records with passport information
	{
		printf("\nName:%s %s\n",run->data->firstname,run->data->lastname);
		printf("Accommodation type:%s\nPassport Number: %s\nAddress: %s\n",run->data->accommodation_type,run->data->id.passport,run->data->address);
	}
	else if(run->data->idtype==2)//printing the records with Employee code information
	{
		printf("\nName:%s %s\n",run->data->firstname,run->data->lastname);
		printf("Accommodation type:%s\nEmployee Code: %s\nAddress: %s\n",run->data->accommodation_type,run->data->id.empcode,run->data->address);
	}
	else
		return FAILURE;
}

status print_one(record* run)
{
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

int print(record* record_head)
{
	record* run=record_head;
	while(run->next!=NULL)
	{
		run=run->next;
		print_one(run);
	}
	return SUCCESS;
}

status house_allot(record* node,int* vacancy)
{
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
	node->next=NULL;
	free(node);
	node=NULL;
	return SUCCESS;
}

status free_sorted(sorted* node)
{
	node->next=NULL;
	node->data=NULL;
	free(node);
	node=NULL;
	return SUCCESS;
}

record* input(int* vacancy)
{
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
}
