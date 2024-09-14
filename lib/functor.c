/*==============================================================================
 Name        : functor.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "functor.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

bool record_gpaless(const student *v1, const student *v2)
{
	assert(v1 && v2);
	if (v1->gpa == v2->gpa)
		return (strcmp(v1->name, v2->name) < 0);

	return (v1->gpa > v2->gpa);
}

void record_swap(student *v1, student *v2)
{
	assert(v1 && v2);
	student tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}
// transform functors
void double_int(const int *psrc, int *pdest)
{
	assert(psrc && pdest);
	int val = *psrc;
	*pdest = (val * 2);
}
void rand_int(int *p) 
{ 
	assert(p);
	*p = rand() % 100; 
}
void print_int(const int *el) 
{
	 assert(el); 
	 printf("%d ", *el); 
}
void print_hex(const int *el)
{
	 assert(el); 
	 printf("%X ", *el); 
}
void print_student(const student *s)
{
	assert(s);
	student *p = (student *)s;
	printf("%s %.2f  ", p->name, p->gpa);
}
void print_str(const char *s) 
{ 
	assert(s);
	printf("%s ", s); 
}
void print_pstr(const char** pp)
{
	assert(pp);
	printf("%s ", *pp); 	
}
bool int_less(const int *v1, const int *v2)
{
	assert(v1 && v2);
	return *v1 < *v2;
}
bool str_less(const char **s1, const char **s2)
{
	assert(s1 && s2 && *s1 && *s2);
	return (strcmp(*s1, *s2) < 0);
}
bool is_even(const int *v1)
{
	assert(v1);
	return (*v1 == 0) ? true : (*v1 % 2) == 0;
}
void int_swap(int *v1, int *v2)
{
	assert(v1 && v2);
	int tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

void str_swap(char **v1, char **v2)
{
        assert(v1 && *v1 && v2 && *v2);
	char *tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

int int_cmp(const int* v1, const int* v2)
{
	assert(v1 && v2);
    	return *v1 - *v2;
}
/* for search */
int str_cmp(const char** v1, const char** v2)
{
        assert(v1 && v2 && *v1 && *v2);
        return strcmp(*v1, *v2);
}
/* for open_table*/
int table_str_cmp(const char* value1, const char* value2)
{
	assert(value1 && value2);
	return strcmp(value1, value2);	
}
int ret_int(const int* val)
{
	assert(val);
	return *val;
}
bool int_genless(const genptr v1, const genptr v2)
{
	return *(int*)v1 < *(int*)v2;
}

void int_genswap(genptr v1, genptr v2)
{
	int tmp = *(int*)v1;
	*(int*)v1 = *(int*)v2;
	*(int*)v2 = tmp;
}
bool record_genless(const genptr v1, const genptr v2)
{
	const student * p1 = (const student*)v1;
	const student *p2 = (const student*)v2;
	if (p1->gpa == p2->gpa)
		return (strcmp(p1->name, p2->name) < 0);

	return (p1->gpa > p2->gpa);
}

void record_genswap(genptr v1, genptr v2)
{
	student * p1 = (student*)v1;
	student * p2 = (student*)v2;

	student tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}
void int_genrand(genptr pi)
{
	assert(pi);
	*(int*)pi = rand() % 100; 
}
void int_genprint(const genptr pi)
{
	assert(pi);
	printf("%d ", *(int*)pi);
}
int ret_genint(const genptr pi)
{
	assert(pi);
	return *(int*)pi;
}
bool is_geneven(const genptr v1)
{
	assert(v1);
	return (*(int*)v1 == 0) ? true : (*(int*)v1 % 2) == 0;
}
int int_gencmp(const genptr v1, const genptr v2)
{
	assert(v1 && v2);
    	return *(int*)v1 - *(int*)v2;
}
int str_gencmp(const genptr v1, const genptr  v2)
{
	assert(v1 && v2);
	const char* p1 = (const char*)v1;
	const char* p2 = (const char*)v2;	
	return strcmp(p1, p2);	
}
void print_genpstr(const genptr pp)
{
	assert(pp);
	const char** s = (const char**)pp;
	printf("%s ", *s);
}
void str_genswap(genptr v1, genptr v2)
{
       assert(v1 && v2);
	char** pp1 = (const char**)v1;
	char** pp2 = (const char**)v2;
	char *tmp = *pp1;
	*pp1 = *pp2;
	*pp2 = tmp;
}
bool str_genless(const genptr s1, const genptr s2)
{
	assert(s1 && s2);
	const char** p1 = (const char**)s1;
	const char** p2 = (const char**)s2;
	return (strcmp(*p1, *p2) < 0);
}
#pragma GCC diagnostic pop
