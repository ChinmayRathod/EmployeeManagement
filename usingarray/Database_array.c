#include<stdio.h>
#include<string.h>
#define SUCCESS 1
#define FAILURE 0
#define SIZE 1000
#define IND 10
//defining for keeping track of alphabetical order of inputs
int index[SIZE];
//for keeping the record of the blocks that are vacant
struct vacancy
{
	int type1[IND];
	int type2[IND];
	int type3[IND];
	int type4[IND];
}vac;
// defining the stucture datatype
struct accommodation
{
	char firstname[50];
	char lastname[50];
	char accommodation_type [20];
	int idtype;
	struct idnum
	{
		char aadhaar[15];
		char passport[15];
		char empcode [15];
	}id;
	char address[100];
}record[SIZE];
//declarations of all the functions used
void initialiseRecords(int start,int stop);
int insertRecords();                                 //case 1-------------------------------------------------------a)
int removeDuplicates(int count);                    //case 2--------------------------------------------------------b)
int printRecords();                                //case 3---------------------------------------------------------c)
int printSortedRecords();                         //case 4----------------------------------------------------------d)
int search(char first_name[],char last_name[]);  //case 5-----------------------------------------------------------e)
int deleteRecord();								//case 6------------------------------------------------------------f)
int updateRecord();							   //case 7-------------------------------------------------------------g)
int specialRequestAllocation();               //case 8--------------------------------------------------------------h)
//some extra functions
void indexleftshift(int num);
int printOneRecord(int i);
int indexing(char firstname_var[],char lastname_var[]);
int fill_vacancy(char acc_type[]);
void make_vacancy(char acc_type[]);
//the main function
int main()
{
	//initializing the index array
	for(int i=0;i<SIZE;i++)
		index[i]=-1;
	//intializing the accommodation structure
	for(int i=0;i<IND;i++)
	{
		vac.type1[i]=-1;
		vac.type2[i]=-1;
		vac.type3[i]=-1;
		vac.type4[i]=-1;
	}
	//initializing the structure
	initialiseRecords(0,SIZE);
	int process,output,exitloop=1;
	//loop to accept,delete and change data 
	while(exitloop)
	{
		printf("\n***************************************************************************************************************\n");
		printf("\nEnter the number of the process which you want to carry out:\n1. Insert a new record.\n2. Remove duplicates.\n3. Print the records.\n4. Sort the records and print.\n");
		printf("5. Search for a certain record.\n6. Delete a record.\n7. Update a record.\n8. Request for block change.\n9. Exit.\n   Input: ");
		scanf("%d",&process);
		printf("\n***************************************************************************************************************\n");
		switch(process)
		{
			case 1:
			{
				output=insertRecords();
				printf("\n***************************************************************************************************************\n");
				if(output==SUCCESS)
					printf("\nThe record was successfully added to the database.\n");
				else if(output==-1)
				{
					printf("\n\t\t\t\t\tNo new records can be inserted as all the accommodations are full.\n");
					printf("\n***************************************************************************************************************\n");
				}
				else
					printf("\nThe record was not entered into the database.\n");
				break;
			}
			case 2:
			{
				output=removeDuplicates(0);
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
				printRecords();
				printf("\nThe records are displayed according to their ID-type above.\n");
				break;
			}
			case 4:
			{
				printSortedRecords();
				printf("\nThe alphabetically sorted records are displayed above.\n");
				break;
			}
			case 5:
			{
				char name1[50],name2[50];
				name1[0]='\0';
				name2[0]='\0';
				printf("\nPlease enter the firstname of the person to be searched in the database:");
				scanf("%s",name1);
				printf("Please enter the lastname of the person to be searched in the database:");
				scanf("%s",name2);
				output=search(name1,name2);
				if(output==SUCCESS)
					printf("\nThe occurances of poeple with name '%s %s' in the database are displayed above.\n",name1,name2);
				else
					printf("\nThe name '%s %s' does not exist in the current database.\n",name1,name2);
				break;
			}
			case 6:
			{
				output=deleteRecord();
				printf("\n***************************************************************************************************************\n");
				if(output==SUCCESS)
					printf("\nThe record was successfully deleted from the database.\n");
				else
					printf("\nThe record does not exist in the current database.\n");
				break;
			}
			case 7:
			{
				output=updateRecord();
				printf("\n***************************************************************************************************************\n");
				if(output==SUCCESS)
					printf("\nThe record was successfully updated in the database.\n");
				else
					printf("\nThe record does not exist in the current database.\n");
				break;
			}
			case 8:
			{
				output=specialRequestAllocation();
				printf("\n***************************************************************************************************************\n");
				if(output==SUCCESS)
					printf("\nCongrats!!!The desired block has been allocated to you.\n");
				else if(output==-1)
					printf("\nThe record does not exist in the current database.\n");
				else
					printf("\nSorry!!!The desired block is full.\n");
				break;
			}
			case 9:
			{
				exitloop=0;
				printf("\n\t\t\t\t\tThank you!Have a great day ahead!!!\n");
				printf("\n***************************************************************************************************************\n");
				break;
			}
			default:
			{
				printf("\nEnter a number from the range of options given only(1-9).\n");
				break;
			}
		}
	}
	return 0;
}
//initializing all the  records to null or zero values
void initialiseRecords(int start,int stop)
{
	for(int i=start;i<stop;i++)
	{
		record[i].firstname[0]='\0';
		record[i].lastname[0]='\0';
		record[i].accommodation_type[0]='\0';
		record[i].idtype=-1;
		record[i].id.aadhaar[0]='\0';
		record[i].id.passport[0]='\0';
		record[i].id.empcode[0]='\0';
		record[i].address[0]='\0';
	}
}
//records are added may be new or may be existing
//case 1-----------------------------------------------a)
int insertRecords()
{
	int retval=FAILURE,vacant=0,count=0;
	int idtype_flag=1;
	char firstname_var[50];
	char lastname_var[50];
	char accommodation_type_var[20];
	char idtype_var;
	char id_var[15];
	char address_var[100];
	printf("\nPlease enter the firstname:");
	scanf("%s",firstname_var);
	printf("Please enter the lastname:");
	scanf("%s",lastname_var);
	while(vacant==0&&count<4)
	{
		int movein=1;
		printf("Please enter the accommodation type(Type-I,Type-II,Type-III,Type-IV):");
		scanf("%s",accommodation_type_var);
		if(strcmp(accommodation_type_var,"Type-I")==0||strcmp(accommodation_type_var,"Type-II")==0||strcmp(accommodation_type_var,"Type-III")==0||strcmp(accommodation_type_var,"Type-IV")==0)
			vacant=fill_vacancy(accommodation_type_var);//finds if there exists a vacancy in the block input 1 if exists free place else 0
		else
			movein=0;//if the user inputs something else than the allowed inputs 
		if(vacant==0&&movein)//no place in the desired block
		{
			printf("\n***************************************************************************************************************\n");
			printf("\nWe are sorry this block has no vacancies,please choose another block.\n");
			count++;
		}
	}
	if(count==4)//all four blocks dont have spacce to accommodate anyone
	{
		printf("\n***************************************************************************************************************\n");
		printf("\nWe are very sorry but all the accommodations are full,there are no vacancies.\n");
		printf("\n***************************************************************************************************************\n");
		idtype_flag=0;
		retval=-1;
		return retval;//exit the function as we cannot insert more records,try to change the data or exit
	}
	while(idtype_flag)//in loop till a valid idtype is given
	{
		scanf("%c",&idtype_var);
		printf("Please choose the idtype:\n0. Aadhaar number\n1. Passport number\n2. Employee code\n   Option: ");
		scanf("%c",&idtype_var);//the datatype of idtype_var is taken to be as character because if int is taken as datatype the program crashes if a character is given
		if(idtype_var=='0')
		{
			printf("Please enter the Aadhaar number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='1')
		{
			printf("Please enter the Passport number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='2')
		{
			printf("Please enter the Empcode number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else
			printf("Please enter a valid ID type number(0,1 or 2):\n");
	}
	printf("Please enter the address:");
	scanf("%s",address_var);
	int j=indexing(firstname_var,lastname_var);//the input must be arranged in the index array
	for(int i=0;i<SIZE&&retval==FAILURE;i++)//finding the first null index to fill the datd in it
	{
		if(record[i].idtype==-1)//found the index i
		{
			index[j]=i;
			strcpy(record[i].firstname,firstname_var);
			strcpy(record[i].lastname,lastname_var);
			strcpy(record[i].accommodation_type,accommodation_type_var);
			record[i].idtype=(int)idtype_var-48;
			if(record[i].idtype==0)
			{
				strcpy(record[i].id.aadhaar,id_var);
			}
			else if(record[i].idtype==1)
			{
				strcpy(record[i].id.passport,id_var);
			}
			else if(record[i].idtype==2)
			{
				strcpy(record[i].id.empcode,id_var);
			}
			strcpy(record[i].address,address_var);
			retval=SUCCESS;
		}
	}
	return retval;
}
//records with same idtype and number are duplicates
//case 2-----------------------------------------------b)
int removeDuplicates(int count)
{
	for(int i=0;i<SIZE-1;i++)//outer loop
	{
		if(record[i].idtype!=-1)//if there is no data then dont consider it
		{
		for(int j=i+1;j<SIZE;j++)//inner loop
		{
			if((record[j].idtype!=-1)&&(record[i].idtype==record[j].idtype))//if the idtypes are same
			{
				//comparing the ID proofs of both indexes,if they are the same it is a duplicate record
					//intializing the record at zth index as it is a duplicate
					//shifting all the contents of index array tp left by 1 so that the deleted array is erased till the element we stumble upon is negative
					//count keeps the track of the number of duplicates removed from the database
				if(record[i].idtype==0&&(strcmp(record[i].id.aadhaar,record[j].id.aadhaar)==0))
				{
					make_vacancy(record[j].accommodation_type);	
					initialiseRecords(j,j+1);
					indexleftshift(j);
					count++;
				}
				else if(record[i].idtype==1&&(strcmp(record[i].id.passport,record[j].id.passport)==0))
				{
					make_vacancy(record[j].accommodation_type);
					initialiseRecords(j,j+1);
					indexleftshift(j);
					count++;
				}
				else if(record[i].idtype==2&&(strcmp(record[i].id.empcode,record[j].id.empcode)==0))
				{
					make_vacancy(record[j].accommodation_type);
					initialiseRecords(j,j+1);
					indexleftshift(j);
					count++;
				}
			}
		}
		}
	}
	return count;
}
//printing the records according to idtype
//case 3-----------------------------------------------c)
int printRecords()
{
	for(int i=0;i<SIZE;i++)//printing the records with passport information
	{
		if(record[i].idtype==1)
		{
			printf("\nFull Name:%s %s\n",record[i].firstname,record[i].lastname);
			printf("Accommodation type:%s\nPassport Number: %s\nAddress: %s\n",record[i].accommodation_type,record[i].id.passport,record[i].address);
			printf("\n***************************************************************************************************************\n");
		}
	}
	for(int i=0;i<SIZE;i++)//printing the records with aadhaar information
	{
		if(record[i].idtype==0)
		{
			printf("\nName:%s %s\n",record[i].firstname,record[i].lastname);
			printf("Accommodation type:%s\nAadhaar Number: %s\nAddress: %s\n",record[i].accommodation_type,record[i].id.aadhaar,record[i].address);
			printf("\n***************************************************************************************************************\n");
		}
	}
	for(int i=0;i<SIZE;i++)//printing the records with empcode information
	{
		if(record[i].idtype==2)
		{
			printf("\nName:%s %s\n",record[i].firstname,record[i].lastname);
			printf("Accommodation type:%s\nEmployee Code: %s\nAddress: %s\n",record[i].accommodation_type,record[i].id.empcode,record[i].address);
			printf("\n***************************************************************************************************************\n");
		}
	}
	return 1;
}
//using the index array print all the names alphabetically
//case 4-----------------------------------------------d)
int printSortedRecords()
{
	for(int j=0;j<SIZE&&index[j]>=0;j++)
	{
		int i=index[j];//array index contains the index of array record alphabetically
		printOneRecord(i);
	}
	return SUCCESS;
}
//To search for a given name in the existing database
//case 5-----------------------------------------------e)
int search(char first_name[],char last_name[])
{
	int retval=FAILURE;
	for(int j=0;j<SIZE&&index[j]>=0;j++)
	{
		int i=index[j];
		if(strcmp(first_name,record[i].firstname)==0&&strcmp(last_name,record[i].lastname)==0)
		{
			retval=printOneRecord(i);
			j=SIZE+1;
		}
	}
	return retval;
}
//get the idtype and id from the user and delete the record if it exists
//case 6-----------------------------------------------f)
int deleteRecord()
{
	int idtype_flag=1,retval=0;
	char idtype_var;
	char id_var[15]; 
	while(idtype_flag)
	{
		scanf("%c",&idtype_var);
		printf("Please choose the idtype:\n0. Aadhaar number\n1. Passport number\n2. Employee code\n   Option: ");
		scanf("%c",&idtype_var);//the datatype of idtype_var is taken to be as character because if int is taken as datatype the program crashes if a character is given
		if(idtype_var=='0')
		{
			printf("Please enter the Aadhaar number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='1')
		{
			printf("Please enter the Passport number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='2')
		{
			printf("Please enter the Empcode number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else
			printf("Please enter a valid ID type number(0,1 or 2):\n");
	}
	int intidtype_var=(int)idtype_var-48;
	for(int j=0;j<SIZE;j++)
	{
		if((record[j].idtype!=-1)&&(intidtype_var==record[j].idtype))
		{
			if(record[j].idtype==0&&(strcmp(id_var,record[j].id.aadhaar)==0))
			{
				make_vacancy(record[j].accommodation_type);//removing the accommodation of the employee
				initialiseRecords(j,j+1);//using intialize to change the value to null
				indexleftshift(j);//the element is removed so overlapping it
				j=SIZE+1;//exitloop
				retval=1;
			}
			else if(record[j].idtype==1&&(strcmp(id_var,record[j].id.passport)==0))
			{
				make_vacancy(record[j].accommodation_type);
				initialiseRecords(j,j+1);
				indexleftshift(j);
				j=SIZE+1;
				retval=1;
			}
			else if(record[j].idtype==2&&(strcmp(id_var,record[j].id.empcode)==0))
			{
				make_vacancy(record[j].accommodation_type);
				initialiseRecords(j,j+1);
				indexleftshift(j);
				j=SIZE+1;
				retval=1;
			}
		}
	}
	return retval;
}
//changing/correcting some details in the existing records
//case 7-----------------------------------------------g)
int updateRecord()
{
	int idtype_flag=1,retval=0,found=-1;
	char idtype_var;
	char id_var[15]; 
	while(idtype_flag)
	{
		scanf("%c",&idtype_var);
		printf("\nPlease choose the idtype:\n0. Aadhaar number\n1. Passport number\n2. Employee code:\n");
		scanf("%c",&idtype_var);//the datatype of idtype_var is taken to be as character because if int is taken as datatype the program crashes if a character is given
		if(idtype_var=='0')
		{
			printf("Please enter the Aadhaar number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='1')
		{
			printf("Please enter the Passport number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='2')
		{
			printf("Please enter the Empcode number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else
			printf("Please enter a valid ID type number(0,1 or 2):\n");
	}
	int intidtype_var=(int)idtype_var-48;
	for(int j=0;j<SIZE;j++)
	{
		if((record[j].idtype!=-1)&&(intidtype_var==record[j].idtype))
		{
			if(record[j].idtype==0)
			{
				if(strcmp(id_var,record[j].id.aadhaar)==0)
				{
					found=j;
					j=SIZE+1;
				}
			}
			else if(record[j].idtype==1)
			{
				if(strcmp(id_var,record[j].id.passport)==0)
				{
					found=j;
					j=SIZE+1;
				}
			}
			else if(record[j].idtype==2)
			{
				if(strcmp(id_var,record[j].id.empcode)==0)
				{
					found=j;
					j=SIZE+1;
				}
			}
		}
	}
	//found contains the index the user wants now ask him which field he wants to edit
	if(found!=-1)
	{
		int option;
		printf("Enter the number in front of the field you want to edit:\n1. Firstname\n2. Lastname\n3. Accommodation\n4. ID\n5. Address\n");
		scanf("%d",&option);
		while(!retval)
		{
			switch(option)
			{
				case 1:
				{
					char name[50];
					printf("Enter the new firstname:");
					scanf("%s",name);
					indexleftshift(found);
					int k=indexing(name,record[found].lastname);
					strcpy(record[found].firstname,name);
					index[k]=found;
					retval=SUCCESS;
					break;
				}
				case 2:
				{
					char name[50];
					printf("Enter the new lastname:");
					scanf("%s",name);
					indexleftshift(found);
					int k=indexing(record[found].firstname,name);
					strcpy(record[found].lastname,name);
					index[k]=found;
					retval=SUCCESS;
					break;
				}
				case 3:
				{
					char accommodation_type_var[20];
					int count=0,vacant=0;
					while(vacant==0&&count<4)
					{
						int movein=1;
						printf("\n***************************************************************************************************************\n");
						printf("Please enter the new accommodation type(Type-I,Type-II,Type-III,Type-IV):");
						scanf("%s",accommodation_type_var);
						if(strcmp(accommodation_type_var,"Type-I")==0||strcmp(accommodation_type_var,"Type-II")==0||strcmp(accommodation_type_var,"Type-III")==0||strcmp(accommodation_type_var,"Type-IV")==0)
						{
							vacant=fill_vacancy(accommodation_type_var);
							if(vacant==1)
							{
								make_vacancy(record[found].accommodation_type);
								strcpy(record[found].accommodation_type,accommodation_type_var);
								retval=1;	
							}
						}	
						else
						{
							printf("\n***************************************************************************************************************\n");
							printf("\nPlease enter a valid accommodation type\n");
							movein=0;
						}
						if(vacant==0&&movein)
						{
							printf("\n***************************************************************************************************************\n");
							printf("\nWe are sorry this block has no vacancies,please choose another block.\n");
							count++;
						}
					}
					if(count==4)
					{
						printf("\n***************************************************************************************************************\n");
						printf("\nWe are very sorry but all the accommodations are full,there are no vacancies.\n");
						printf("\n***************************************************************************************************************\n");
						retval=0;
					}
					break;
				}
				case 4:
				{
					idtype_flag=1;
					while(idtype_flag)
					{
						scanf("%c",&idtype_var);
						printf("Please choose the idtype:\n0. Aadhaar number\n1. Passport number\n2. Employee code:\n");
						scanf("%c",&idtype_var);//the datatype of idtype_var is taken to be as character because if int is taken as datatype the program crashes if a character is given
						if(idtype_var=='0')
						{
							printf("Please enter the new Aadhaar number:");
							scanf("%s",id_var);
							record[found].idtype=(int)idtype_var-48;
							strcpy(record[found].id.aadhaar,id_var);
							idtype_flag=0;
						}
						else if(idtype_var=='1')
						{
							printf("Please enter the new Passport number:");
							scanf("%s",id_var);
							record[found].idtype=(int)idtype_var-48;
							strcpy(record[found].id.passport,id_var);
							idtype_flag=0;
						}
						else if(idtype_var=='2')
						{
							printf("Please enter the new Empcode number:");	
							scanf("%s",id_var);
							strcpy(record[found].id.empcode,id_var);
							idtype_flag=0;
							record[found].idtype=(int)idtype_var-48;
						}
						else
							printf("Please enter a valid ID type number(0,1 or 2):\n");
					}			
					retval=SUCCESS;
					break;
				}
				case 5:
				{
					char address_var[100];
					printf("Enter the new Address:");
					scanf("%s",address_var);
					strcpy(record[found].address,address_var);
					retval=SUCCESS;
					break;
				}
				default:
				{
					printf("Please enter an option from the given range only.(1-5)\n");
					break;
				}
			}
		}
	}
	return retval;
}
//giving blocks as per request if are vacant //here also found contains the index of the one who is requesting for block change
//case 8-----------------------------------------------h)
int specialRequestAllocation()
{
	int idtype_flag=1,retval=0,found=-1;
	char idtype_var;
	char id_var[15]; 
	while(idtype_flag)
	{
		scanf("%c",&idtype_var);
		printf("Please choose the idtype:\n0. Aadhaar number\n1. Passport number\n2. Employee code:\n");
		scanf("%c",&idtype_var);//the datatype of idtype_var is taken to be as character because if int is taken as datatype the program crashes if a character is given
		if(idtype_var=='0')
		{
			printf("Please enter the Aadhaar number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='1')
		{
			printf("Please enter the Passport number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else if(idtype_var=='2')
		{
			printf("Please enter the Empcode number:");
			scanf("%s",id_var);
			idtype_flag=0;
		}
		else
			printf("Please enter a valid ID type number(0,1 or 2):\n");
	}
	int intidtype_var=(int)idtype_var-48;
	for(int j=0;j<SIZE;j++)
	{
		if((record[j].idtype!=-1)&&(intidtype_var==record[j].idtype))
		{
			if(record[j].idtype==0)
			{
				if(strcmp(id_var,record[j].id.aadhaar)==0)
				{
					found=j;
					j=SIZE+1;
				}
			}
			else if(record[j].idtype==1)
			{
				if(strcmp(id_var,record[j].id.passport)==0)
				{
					found=j;
					j=SIZE+1;
				}
			}
			else if(record[j].idtype==2)
			{
				if(strcmp(id_var,record[j].id.empcode)==0)
				{
					found=j;
					j=SIZE+1;
				}
			}
		}
	}
	if(found!=-1)
	{
		char accommodation_type_var[20];
		int count=0,vacant=0;
		while(vacant==0&&count<4)
		{
			int movein=1;
			printf("\n***************************************************************************************************************\n");
			printf("Please enter the new accommodation type(Type-I,Type-II,Type-III,Type-IV):");
			scanf("%s",accommodation_type_var);
			if(strcmp(accommodation_type_var,"Type-I")==0||strcmp(accommodation_type_var,"Type-II")==0||strcmp(accommodation_type_var,"Type-III")==0||strcmp(accommodation_type_var,"Type-IV")==0)
			{
				vacant=fill_vacancy(accommodation_type_var);
				if(vacant==1)
				{	
				    make_vacancy(record[found].accommodation_type);
					strcpy(record[found].accommodation_type,accommodation_type_var);
					retval=1;	
				}
			}	
			else
			{
				printf("\n***************************************************************************************************************\n");
				printf("\nPlease enter a valid accommodation type\n");
				movein=0;
			}
			if(vacant==0&&movein)
			{
					printf("\n***************************************************************************************************************\n");
				printf("\nWe are sorry this block has no vacancies,please choose another block.\n");
				count++;
			}
		}
		if(count==4)
		{
			printf("\n***************************************************************************************************************\n");
			printf("\nWe are very sorry but all the accommodations are full,there are no vacancies.\n");
			printf("\n***************************************************************************************************************\n");
			retval=0;
		}
	}
	else 
		retval=-1;
	return retval;
}
//for keeping the alphabetical order of the inputs
void indexleftshift(int num)//num is the index of the record in the array of structure accommodation
{
	int leftshift=0;
	for(int j=0;j<SIZE-1&&index[j]>=0;j++)
	{
		if(leftshift==1)
		{
			index[j]=index[j+1];
		}
		else if(index[j]==num)//finding the index num in array of index
		{
			leftshift=1;	//afterthis all elements should be shifted to left by 1 
			index[j]=index[j+1];
		}
	}
}
//useful if we know the index of the record that we have to print
int printOneRecord(int i)
{
	if(record[i].idtype==1)
	{
		printf("\nFull Name:%s %s\n",record[i].firstname,record[i].lastname);
		printf("Accommodation type:%s\nPassport Number: %s\nAddress: %s\n",record[i].accommodation_type,record[i].id.passport,record[i].address);
		printf("\n***************************************************************************************************************\n");
	}
	else if(record[i].idtype==0)
	{
		printf("\nFull Name:%s %s\n",record[i].firstname,record[i].lastname);
		printf("Accommodation type:%s\nAadhaar Number: %s\nAddress: %s\n",record[i].accommodation_type,record[i].id.aadhaar,record[i].address);
		printf("\n***************************************************************************************************************\n");
	}
	else if(record[i].idtype==2)
	{
		printf("\nFull Name:%s %s\n",record[i].firstname,record[i].lastname);
		printf("Accommodation type:%s\nEmployee Code: %s\nAddress: %s\n",record[i].accommodation_type,record[i].id.empcode,record[i].address);
		printf("\n***************************************************************************************************************\n");
	}
	return 1;
}
//for keeping track of the proper alphabetical of the contents of the database
int indexing(char firstname_var[],char lastname_var[])
{
	int idtype_flag=1;
	int j=0;
	for(j=0;j<SIZE&&idtype_flag;j++)//we are inserting a new element to the array of record so ifnding the first negative value if array index
	{
		if(index[j]<0)
			idtype_flag=0;
	}
	j-=1;//storing the index in j
	if(j!=0)
	{
		//the element is given its proper position in this loop
		for(int i=j-1;i>=0;i--)
		{
			int pos=index[i];//index[i] is the index of array of record
			if(strcmp(record[pos].firstname,firstname_var)>0)
			{
				index[i+1]=index[i];
				if(i==0)
					j=0;
			}
			else if(strcmp(record[pos].firstname,firstname_var)<0)
			{
				j=i+1;
				i=-1;
			}
			else
			{
				if(strcmp(record[pos].lastname,lastname_var)>0)
				{
					index[i+1]=index[i];
					if(i==0)
						j=0;
				}
				else
				{
					j=i+1;
					i=-1;
				}
			}
		}
	}
	else
	{
		//if it is the first element in the array
		index[0]=0;
	}
	return j;	
}
//this function fills the blocks when someone occupies it
int fill_vacancy(char acc_type[])
{
	int retval=0;
	if(strcmp(acc_type,"Type-I")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type1[j]<0)
			{
				vac.type1[j]=1;
				retval=1;
			}
		}
	}
	else if(strcmp(acc_type,"Type-II")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type2[j]<0)
			{
				vac.type2[j]=1;
				retval=1;
			}
		}
	}
	else if(strcmp(acc_type,"Type-III")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type3[j]<0)
			{
				vac.type3[j]=1;
				retval=1;
			}
		}
	}
	else if(strcmp(acc_type,"Type-IV")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type4[j]<0)
			{
				vac.type4[j]=1;
				retval=1;
			}
		}
	}
	return retval;
}
//this fuction takes care of the vacancies when someone leaves or duplicates are removed
void make_vacancy(char acc_type[])
{
	int retval=0;
	if(strcmp(acc_type,"Type-I")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type1[j]>0)
			{
				vac.type1[j]=-1;
				retval=1;
			}
		}
	}
	else if(strcmp(acc_type,"Type-II")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type2[j]>0)
			{
				vac.type2[j]=-1;
				retval=1;
			}
		}
	}
	else if(strcmp(acc_type,"Type-III")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type3[j]>0)
			{
				vac.type3[j]=-1;
				retval=1;
			}
		}
	}
	else if(strcmp(acc_type,"Type-IV")==0)
	{
		for(int j=0;j<IND&&retval==0;j++)
		{
			if(vac.type4[j]>0)
			{
				vac.type4[j]=-1;
				retval=1;
			}
		}
	}
}
/*
	//sequence of all the functions above
	void initialiseRecords(int start,int stop);
	int insertRecords();                                 //case 1-------------------------------------------------------a)
	int removeDuplicates(int count);                    //case 2--------------------------------------------------------b)
	int printRecords();                                //case 3---------------------------------------------------------c)
	int printSortedRecords();                         //case 4----------------------------------------------------------d)
	int search(char first_name[],char last_name[]);  //case 5-----------------------------------------------------------e)
	int deleteRecord();								//case 6------------------------------------------------------------f)
	int updateRecord();							   //case 7-------------------------------------------------------------g)
	int specialRequestAllocation();               //case 8--------------------------------------------------------------h)
	void indexleftshift(int num);
	int printOneRecord(int i);
	int indexing(char firstname_var[],char lastname_var[]);
	int fill_vacancy(char acc_type[]);
	void make_vacancy(char acc_type[]);
*/
