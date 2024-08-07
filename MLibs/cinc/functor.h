/*==============================================================================
 Name        : functor.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef struct student {
	const char *name;
	double gpa;
} student;

#ifdef __cplusplus
extern "C" {
#endif

// predicates
bool record_gpaless(const student *v1, const student *v2);
bool str_less(const char **s1, const char **s2);
bool int_less(const int *v1, const int *v2);
bool is_even(const int *v1);

/* for search */
int int_cmp(const int* v1, const int* v2);
int str_cmp(const char** v1, const char** v2);
/* for open_table */
int table_str_cmp(const char* value1, const char* value2);


// swap functors 
void record_swap(student *v1, student *v2);
void int_swap(int *v1, int *v2);
void str_swap(char **v1, char **v2);

// various
void double_int(const int *psrc, int *pdest);
void rand_int(int *p);
void print_student(const student *s);
void print_int(const int *el);
void print_str(const char *s); 
void print_pstr(const char** pp);
void print_hex(const int *el);
int ret_int(const int* val);

/* cpp builds for c\cpp mixed tests*/
bool int_genless(const genptr v1, const genptr v2);
void int_genswap(genptr v1, genptr v2);
bool record_genless(const genptr v1, const genptr v2);
void record_genswap(genptr v1, genptr v2);
void int_genprint(const genptr pi);
void int_genrand(genptr pi);
int ret_genint(const genptr pi);

int int_gencmp(const genptr v1, const genptr v2);
int str_gencmp(const genptr v1, const genptr v2);
bool is_geneven(const genptr v1);
void print_genpstr(const genptr pp);	
void str_genswap(genptr v1, genptr v2);
bool str_genless(const genptr s1, const genptr s2);
#ifdef __cplusplus
}
#endif

