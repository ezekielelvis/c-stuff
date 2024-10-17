#include <stdio.h>

struct data {
    char firstname[20];
    char lastname[20];
    int age, employee_id, last_4_SSN;
    char gender;
    char title[20];
};

struct employee{
    struct data employee1;
    struct data employee2;
}engineering_dept;


int main(){

    for(int i = 0; i < 2; i++){
        struct data *employee = &engineering_dept.employee1;
        if(i > 0)
            employee = &engineering_dept.employee2;

        printf("Enter the employee's first name: ");
        scanf("%s", employee->firstname);

        printf("Enter the employee's last name: ");
        scanf("%s", employee->lastname);

        printf("Enter the employee's age: ");
        scanf("%d", &employee->age);

        printf("Enter the employee's employee id: ");
        scanf("%d", &employee->employee_id);

        printf("Enter the employee's last 4 SSN: ");
        scanf("%d", &employee->last_4_SSN);

        printf("Enter the employee's gender: ");
        scanf("%c", &employee->gender);

        printf("Enter the employee's title: ");
        scanf("%s", employee->title);
    }

    for(int i = 0; i < 2; i++){
        struct data *employee = &engineering_dept.employee1;
        if(i > 0)
            employee = &engineering_dept.employee2;

        printf("The employee's first name is %s\n", employee->firstname);
        printf("The employee's last name is %s\n", employee->lastname);
        printf("The employee's age is %d\n", employee->age);
        printf("The employee's employee id is %d\n", employee->employee_id);
        printf("The employee's last 4 SSN is %d\n", employee->last_4_SSN);
        printf("The employee's gender is %c\n", employee->gender);
        printf("The employee's title is %s\n", employee->title);
    }
}