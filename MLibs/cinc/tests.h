/*==============================================================================
 Name        : tests.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once
#include <stddef.h>

#ifndef __cplusplus
#ifndef NL
#define NL printf("\n")
#endif
#endif

/* test helper */
void print_int_array(int *arr, const size_t count);

/* array-test functions */
void array_test();
void poly_test();

void array_test_driver();

/*c-algo-test functions */
void visit_sort_trans_mod_test();
void search_test();
void bit_manip_test();
void deduce_type_test();
void merge_test();
void swap_ranges_test();
void random_shuffle_test();
void count_if_test();
void equal_test();
void replace_test();
void reverse_test();
void copy_backward_test();
void product_test();

void c_algo_test_driver();


       
