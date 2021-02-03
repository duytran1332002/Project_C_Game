#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int ID;
	char Name;
	int Age;
	int Salary;
	printf("Enter ID:");
	scanf("%d",&ID);
	getchar();
	printf("\n");
	
	printf("Enter your Name:");
	scanf("%c",&Name);
	getchar();
	printf("\n");
	
	
	printf("Enter your Age:");
	scanf("%d",&Age);
	getchar();
	printf("\n");
	
	
	printf("Enter your salary:");
	scanf("%d",&Salary);
	//getchar();
	printf("\n");
	
	
	printf("ID\t:\t%d",ID);
	printf("\n");
	printf("Name\t:\t%c",Name);
	printf("\n");
	printf("Age\t:\t%d",Age);
	printf("\n");
	printf("Salary\t:\t%d$",Salary);
	return 0;
}
