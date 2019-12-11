#ifndef GLOBAL_PARAMETERS
#define GLOBAL_PARAMETERS
#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <boost/unordered_map.hpp>
#ifdef PARAL
#include <omp.h>
#endif
using namespace std;

const int DEBUG = 0;    //1: print debug info
const int STAS = 1;        //1: print statistics info
//section: add. only when new protein sequences are added
int ONLY_COMPUTE_NEW_PROTEIN =
    0; // 1: only compute the HSPs of new proteins and append the new HSPs into the original HSP file.
string NEW_PROTEIN_FN;
string ORIGINAL_HSP_FN;
set<int> new_proteins;
int num_old_protein = 0;
//end of section: add
string PROTEIN_FN;
string HSP_FN;
int Thit = 15;
int k_size = 20;//size of kmer, default 20
int T_kmer = 35;//k-mer similarity, default 35
int matrix_id = 1; //1: PAM120, 2: BLOSUM80, 3: BLOSUM62 
// int T_ppi = 10; //PPI predicton value, default 10
int num_protein = 0; //number of proteins
uint64_t hash_table_default = 1090715534754863;    //the largest number in large_primes
// string file_name_prefix;
uint64_t total_hsp = 0;
// string SUB_SEQ_FN;
// string SUB_SEQ_FN = "Src/compute_HSPs/sub_seq.test";

int BLOSUM80[23][23];
int B80_ordered[21][21];
/*!Below is PAM120!*/
int PAM120[23][23] =    //it's acutally PAM120, the last row and colum indicate "X"
    {
        {3, -3, -1, 0, -3, -1, 0, 1, -3, -1, -3, -2, -2, -4, 1, 1, 1, -7, -4, 0, 0, -1, -1},
        {-3, 6, -1, -3, -4, 1, -3, -4, 1, -2, -4, 2, -1, -5, -1, -1, -2, 1, -5, -3, -2, -1, -2},
        {-1, -1, 4, 2, -5, 0, 1, 0, 2, -2, -4, 1, -3, -4, -2, 1, 0, -4, -2, -3, 3, 0, -1},
        {0, -3, 2, 5, -7, 1, 3, 0, 0, -3, -5, -1, -4, -7, -3, 0, -1, -8, -5, -3, 4, 3, -2},
        {-3, -4, -5, -7, 9, -7, -7, -4, -4, -3, -7, -7, -6, -6, -4, 0, -3, -8, -1, -3, -6, -7, -4},
        {-1, 1, 0, 1, -7, 6, 2, -3, 3, -3, -2, 0, -1, -6, 0, -2, -2, -6, -5, -3, 0, 4, -1},
        {0, -3, 1, 3, -7, 2, 5, -1, -1, -3, -4, -1, -3, -7, -2, -1, -2, -8, -5, -3, 3, 4, -1},
        {1, -4, 0, 0, -4, -3, -1, 5, -4, -4, -5, -3, -4, -5, -2, 1, -1, -8, -6, -2, 0, -2, -2},
        {-3, 1, 2, 0, -4, 3, -1, -4, 7, -4, -3, -2, -4, -3, -1, -2, -3, -3, -1, -3, 1, 1, -2},
        {-1, -2, -2, -3, -3, -3, -3, -4, -4, 6, 1, -3, 1, 0, -3, -2, 0, -6, -2, 3, -3, -3, -1},
        {-3, -4, -4, -5, -7, -2, -4, -5, -3, 1, 5, -4, 3, 0, -3, -4, -3, -3, -2, 1, -4, -3, -2},
        {-2, 2, 1, -1, -7, 0, -1, -3, -2, -3, -4, 5, 0, -7, -2, -1, -1, -5, -5, -4, 0, -1, -2},
        {-2, -1, -3, -4, -6, -1, -3, -4, -4, 1, 3, 0, 8, -1, -3, -2, -1, -6, -4, 1, -4, -2, -2},
        {-4, -5, -4, -7, -6, -6, -7, -5, -3, 0, 0, -7, -1, 8, -5, -3, -4, -1, 4, -3, -5, -6, -3},
        {1, -1, -2, -3, -4, 0, -2, -2, -1, -3, -3, -2, -3, -5, 6, 1, -1, -7, -6, -2, -2, -1, -2},
        {1, -1, 1, 0, 0, -2, -1, 1, -2, -2, -4, -1, -2, -3, 1, 3, 2, -2, -3, -2, 0, -1, -1},
        {1, -2, 0, -1, -3, -2, -2, -1, -3, 0, -3, -1, -1, -4, -1, 2, 4, -6, -3, 0, 0, -2, -1},
        {-7, 1, -4, -8, -8, -6, -8, -8, -3, -6, -3, -5, -6, -1, -7, -2, -6, 12, -2, -8, -6, -7, -5},
        {-4, -5, -2, -5, -1, -5, -5, -6, -1, -2, -2, -5, -4, 4, -6, -3, -3, -2, 8, -3, -3, -5, -3},
        {0, -3, -3, -3, -3, -3, -3, -2, -3, 3, 1, -4, 1, -3, -2, -2, 0, -8, -3, 5, -3, -3, -1},
        {0, -2, 3, 4, -6, 0, 3, 0, 1, -3, -4, 0, -4, -5, -2, 0, 0, -6, -3, -3, 4, 2, -1},
        {-1, -1, 0, 3, -7, 4, 4, -2, 1, -3, -3, -1, -2, -6, -1, -1, -2, -7, -5, -3, 2, 4, -1},
        {-1, -2, -1, -2, -4, -1, -1, -2, -2, -1, -2, -2, -2, -3, -2, -1, -1, -5, -3, -1, -1, -1, -2},
    };

// int B80_ordered[21][21] = //it's PAM120_ordered
//     {
// 		{ 0, 7, 14, 15, 16, 3, 6, 19, 2, 5, 9, 11, 12, 1, 4, 8, 10, 13, 18, 17, 20 },
// 		{ 1, 11, 5, 8, 17, 2, 12, 14, 15, 9, 16, 0, 3, 6, 19, 4, 7, 10, 13, 18, 20 },
// 		{ 2, 3, 8, 6, 11, 15, 5, 7, 16, 0, 1, 9, 14, 18, 12, 19, 10, 13, 17, 4, 20 },
// 		{ 3, 6, 2, 5, 0, 7, 8, 15, 11, 16, 1, 9, 14, 19, 12, 10, 18, 4, 13, 17, 20 },
// 		{ 4, 15, 18, 0, 9, 16, 19, 1, 7, 8, 14, 2, 12, 13, 3, 5, 6, 10, 11, 17, 20 },
// 		{ 5, 8, 6, 1, 3, 2, 11, 14, 0, 12, 10, 15, 16, 7, 9, 19, 18, 13, 17, 4, 20 },
// 		{ 6, 3, 5, 2, 0, 7, 8, 11, 15, 14, 16, 1, 9, 12, 19, 10, 18, 4, 13, 17, 20 },
// 		{ 7, 0, 15, 2, 3, 6, 16, 14, 19, 5, 11, 1, 4, 8, 9, 12, 10, 13, 18, 17, 20 },
// 		{ 8, 5, 2, 1, 3, 6, 14, 18, 11, 15, 0, 10, 13, 16, 17, 19, 4, 7, 9, 12, 20 },
// 		{ 9, 19, 10, 12, 13, 16, 0, 1, 2, 15, 18, 3, 4, 5, 6, 11, 14, 7, 8, 17, 20 },
// 		{ 10, 12, 9, 19, 13, 5, 18, 0, 8, 14, 16, 17, 1, 2, 6, 11, 15, 3, 7, 4, 20 },
// 		{ 11, 1, 2, 5, 12, 3, 6, 15, 16, 0, 8, 14, 7, 9, 10, 19, 17, 18, 4, 13, 20 },
// 		{ 12, 10, 9, 19, 11, 1, 5, 13, 16, 0, 15, 2, 6, 14, 3, 7, 8, 18, 4, 17, 20 },
// 		{ 13, 18, 9, 10, 12, 17, 8, 15, 19, 0, 2, 16, 1, 7, 14, 4, 5, 3, 6, 11, 20 },
// 		{ 14, 0, 15, 5, 1, 8, 16, 2, 6, 7, 11, 19, 3, 9, 10, 12, 4, 13, 18, 17, 20 },
// 		{ 15, 16, 0, 2, 7, 14, 3, 4, 1, 6, 11, 5, 8, 9, 12, 17, 19, 13, 18, 10, 20 },
// 		{ 16, 15, 0, 2, 9, 19, 3, 7, 11, 12, 14, 1, 5, 6, 4, 8, 10, 18, 13, 17, 20 },
// 		{ 17, 1, 13, 15, 18, 8, 10, 2, 11, 5, 9, 12, 16, 0, 14, 3, 4, 6, 7, 19, 20 },
// 		{ 18, 13, 4, 8, 2, 9, 10, 17, 15, 16, 19, 0, 12, 1, 3, 5, 6, 11, 7, 14, 20 },
// 		{ 19, 9, 10, 12, 0, 16, 7, 14, 15, 1, 2, 3, 4, 5, 6, 8, 13, 18, 11, 17, 20 },
// 		{ 0, 2, 5, 6, 9, 15, 16, 19, 1, 3, 7, 8, 10, 11, 12, 14, 13, 18, 4, 17, 20},
//     };

/*!Below is BLOSUM80!*/
int BLOSUM80_1[23][23] =    //it's acutally BLOSUM80, the last row and colum indicate "X", colum and row "J" are deleted
    {
        {5, -2, -2, -2, -1, -1, -1, 0, -2, -2, -2, -1, -1, -3, -1, 1, 0, -3, -2, 0, -2, -1, -1},
        {-2, 6, -1, -2, -4, 1, -1, -3, 0, -3, -3, 2, -2, -4, -2, -1, -1, -4, -3, -3, -1, 0, -1},
        {-2, -1, 6, 1, -3, 0, -1, -1, 0, -4, -4, 0, -3, -4, -3, 0, 0, -4, -3, -4, 5, 0, -1},
        {-2, -2, 1, 6, -4, -1, 1, -2, -2, -4, -5, -1, -4, -4, -2, -1, -1, -6, -4, -4, 5, 1, -1},
        {-1, -4, -3, -4, 9, -4, -5, -4, -4, -2, -2, -4, -2, -3, -4, -2, -1, -3, -3, -1, -4, -4, -1},
        {-1, 1, 0, -1, -4, 6, 2, -2, 1, -3, -3, 1, 0, -4, -2, 0, -1, -3, -2, -3, 0, 4, -1},
        {-1, -1, -1, 1, -5, 2, 6, -3, 0, -4, -4, 1, -2, -4, -2, 0, -1, -4, -3, -3, 1, 5, -1},
        {0, -3, -1, -2, -4, -2, -3, 6, -3, -5, -4, -2, -4, -4, -3, -1, -2, -4, -4, -4, -1, -3, -1},
        {-2, 0, 0, -2, -4, 1, 0, -3, 8, -4, -3, -1, -2, -2, -3, -1, -2, -3, 2, -4, -1, 0, -1},
        {-2, -3, -4, -4, -2, -3, -4, -5, -4, 5, 1, -3, 1, -1, -4, -3, -1, -3, -2, 3, -4, -4, -1},
        {-2, -3, -4, -5, -2, -3, -4, -4, -3, 1, 4, -3, 2, 0, -3, -3, -2, -2, -2, 1, -4, -3, -1},
        {-1, 2, 0, -1, -4, 1, 1, -2, -1, -3, -3, 5, -2, -4, -1, -1, -1, -4, -3, -3, -1, 1, -1},
        {-1, -2, -3, -4, -2, 0, -2, -4, -2, 1, 2, -2, 6, 0, -3, -2, -1, -2, -2, 1, -3, -1, -1},
        {-3, -4, -4, -4, -3, -4, -4, -4, -2, -1, 0, -4, 0, 6, -4, -3, -2, 0, 3, -1, -4, -4, -1},
        {-1, -2, -3, -2, -4, -2, -2, -3, -3, -4, -3, -1, -3, -4, 8, -1, -2, -5, -4, -3, -2, -2, -1},
        {1, -1, 0, -1, -2, 0, 0, -1, -1, -3, -3, -1, -2, -3, -1, 5, 1, -4, -2, -2, 0, 0, -1},
        {0, -1, 0, -1, -1, -1, -1, -2, -2, -1, -2, -1, -1, -2, -2, 1, 5, -4, -2, 0, -1, -1, -1},
        {-3, -4, -4, -6, -3, -3, -4, -4, -3, -3, -2, -4, -2, 0, -5, -4, -4, 11, 2, -3, -5, -3, -1},
        {-2, -3, -3, -4, -3, -2, -3, -4, 2, -2, -2, -3, -2, 3, -4, -2, -2, 2, 7, -2, -3, -3, -1},
        {0, -3, -4, -4, -1, -3, -3, -4, -4, 3, 1, -3, 1, -1, -3, -2, 0, -3, -2, 4, -4, -3, -1},
        {-2, -1, 5, 5, -4, 0, 1, -1, -1, -4, -4, -1, -3, -4, -2, 0, -1, -5, -3, -4, 5, 0, -1},
        {-1, 0, 0, 1, -4, 4, 5, -3, 0, -4, -3, 1, -1, -4, -2, 0, -1, -3, -3, -3, 0, 5, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    };

// int B80_ordered[21][21] = //it's BLOSUM80_ordered
//     {
// 		{ 0, 15, 7, 16, 19, 4, 5, 6, 11, 12, 14, 1, 2, 3, 8, 9, 10, 18, 20, 13, 17, },
// 		{ 1, 11, 5, 8, 2, 6, 15, 16, 20, 0, 3, 12, 14, 7, 9, 10, 18, 19, 4, 13, 17, },
// 		{ 2, 20, 3, 5, 8, 11, 15, 16, 1, 6, 7, 0, 4, 12, 14, 18, 9, 10, 13, 17, 19, },
// 		{ 3, 20, 2, 6, 5, 11, 15, 16, 0, 1, 7, 8, 14, 4, 9, 12, 13, 18, 19, 10, 17, },
// 		{ 4, 0, 16, 19, 9, 10, 12, 15, 2, 13, 17, 18, 1, 3, 5, 7, 8, 11, 14, 20, 6, },
// 		{ 5, 6, 1, 8, 11, 2, 12, 15, 20, 0, 3, 16, 7, 14, 18, 9, 10, 17, 19, 4, 13, },
// 		{ 6, 5, 3, 11, 20, 8, 15, 0, 1, 2, 16, 12, 14, 7, 18, 19, 9, 10, 13, 17, 4, },
// 		{ 7, 0, 2, 15, 20, 3, 5, 11, 16, 1, 6, 8, 14, 4, 10, 12, 13, 17, 18, 19, 9, },
// 		{ 8, 18, 5, 1, 2, 6, 11, 15, 20, 0, 3, 12, 13, 16, 7, 10, 14, 17, 4, 9, 19, },
// 		{ 9, 19, 10, 12, 13, 16, 0, 4, 18, 1, 5, 11, 15, 17, 2, 3, 6, 8, 14, 20, 7, },
// 		{ 10, 12, 9, 19, 13, 0, 4, 16, 17, 18, 1, 5, 8, 11, 14, 15, 2, 6, 7, 20, 3, },
// 		{ 11, 1, 5, 6, 2, 0, 3, 8, 14, 15, 16, 20, 7, 12, 9, 10, 18, 19, 4, 13, 17, },
// 		{ 12, 10, 9, 19, 5, 13, 0, 16, 1, 4, 6, 8, 11, 15, 17, 18, 2, 14, 20, 3, 7, },
// 		{ 13, 18, 10, 12, 17, 9, 19, 8, 16, 0, 4, 15, 1, 2, 3, 5, 6, 7, 11, 14, 20, },
// 		{ 14, 0, 11, 15, 1, 3, 5, 6, 16, 20, 2, 7, 8, 10, 12, 19, 4, 9, 13, 18, 17, },
// 		{ 15, 0, 16, 2, 5, 6, 20, 1, 3, 7, 8, 11, 14, 4, 12, 18, 19, 9, 10, 13, 17, },
// 		{ 16, 15, 0, 2, 19, 1, 3, 4, 5, 6, 9, 11, 12, 20, 7, 8, 10, 13, 14, 18, 17, },
// 		{ 17, 18, 13, 10, 12, 0, 4, 5, 8, 9, 19, 1, 2, 6, 7, 11, 15, 16, 14, 20, 3, },
// 		{ 18, 13, 8, 17, 0, 5, 9, 10, 12, 15, 16, 19, 1, 2, 4, 6, 11, 20, 3, 7, 14, },
// 		{ 19, 9, 10, 12, 0, 16, 4, 13, 15, 18, 1, 5, 6, 11, 14, 17, 2, 3, 7, 8, 20, },
// 		{ 2, 3, 20, 6, 5, 15, 1, 7, 8, 11, 16, 0, 14, 12, 18, 4, 9, 10, 13, 19, 17, },
//     };	

/*!Below is BLOSUM62!*/
int BLOSUM62[23][23] =    //it's acutally BLOSUM62, the last row and colum indicate "X"
    {
        {4, -1, -2, -2, 0, -1, -1, 0, -2, -1, -1, -1, -1, -2, -1, 1, 0, -3, -2, 0, -2, -1, 0},
        {-1, 5, 0, -2, -3, 1, 0, -2, 0, -3, -2, 2, -1, -3, -2, -1, -1, -3, -2, -3, -1, 0, -1},
        {-2, 0, 6, 1, -3, 0, 0, 0, 1, -3, -3, 0, -2, -3, -2, 1, 0, -4, -2, -3, 3, 0, -1},
        {-2, -2, 1, 6, -3, 0, 2, -1, -1, -3, -4, -1, -3, -3, -1, 0, -1, -4, -3, -3, 4, 1, -1},
        {0, -3, -3, -3, 9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -3, -2},
        {-1, 1, 0, 0, -3, 5, 2, -2, 0, -3, -2, 1, 0, -3, -1, 0, -1, -2, -1, -2, 0, 3, -1},
        {-1, 0, 0, 2, -4, 2, 5, -2, 0, -3, -3, 1, -2, -3, -1, 0, -1, -3, -2, -2, 1, 4, -1},
        {0, -2, 0, -1, -3, -2, -2, 6, -2, -4, -4, -2, -3, -3, -2, 0, -2, -2, -3, -3, -1, -2, -1},
        {-2, 0, 1, -1, -3, 0, 0, -2, 8, -3, -3, -1, -2, -1, -2, -1, -2, -2, 2, -3, 0, 0, -1},
        {-1, -3, -3, -3, -1, -3, -3, -4, -3, 4, 2, -3, 1, 0, -3, -2, -1, -3, -1, 3, -3, -3, -1},
        {-1, -2, -3, -4, -1, -2, -3, -4, -3, 2, 4, -2, 2, 0, -3, -2, -1, -2, -1, 1, -4, -3, -1},
        {-1, 2, 0, -1, -3, 1, 1, -2, -1, -3, -2, 5, -1, -3, -1, 0, -1, -3, -2, -2, 0, 1, -1},
        {-1, -1, -2, -3, -1, 0, -2, -3, -2, 1, 2, -1, 5, 0, -2, -1, -1, -1, -1, 1, -3, -1, -1},
        {-2, -3, -3, -3, -2, -3, -3, -3, -1, 0, 0, -3, 0, 6, -4, -2, -2, 1, 3, -1, -3, -3, -1},
        {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4, 7, -1, -1, -4, -3, -2, -2, -1, -2},
        {1, -1, 1, 0, -1, 0, 0, 0, -1, -2, -2, 0, -1, -2, -1, 4, 1, -3, -2, -2, 0, 0, 0},
        {0, -1, 0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1, 1, 5, -2, -2, 0, -1, -1, 0},
        {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1, 1, -4, -3, -2, 11, 2, -3, -4, -3, -2},
        {-2, -2, -2, -3, -2, -1, -2, -3, 2, -1, -1, -2, -1, 3, -3, -2, -2, 2, 7, -1, -3, -2, -1},
        {0, -3, -3, -3, -1, -2, -2, -3, -3, 3, 1, -2, 1, -1, -2, -2, 0, -3, -1, 4, -3, -2, -1},
        {-2, -1, 3, 4, -3, 0, 1, -1, 0, -3, -4, 0, -3, -3, -2, 0, -1, -4, -3, -3, 4, 1, -1},
        {-1, 0, 0, 1, -3, 3, 4, -2, 0, -3, -3, 1, -1, -3, -1, 0, -1, -3, -2, -2, 1, 4, -1},
        {0, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, 0, 0, -2, -1, -1, -1, -1, -1},
    };

// int B80_ordered[21][21] = //it's BLOSUM62_ordered
//     {
// 		{ 0, 15, 4, 7, 16, 19, 1, 5, 6, 9, 10, 11, 12, 14, 2, 3, 8, 13, 18, 20, 17, },
// 		{ 1, 11, 5, 2, 6, 8, 0, 12, 15, 16, 20, 3, 7, 10, 14, 18, 4, 9, 13, 17, 19, },
// 		{ 2, 20, 3, 8, 15, 1, 5, 6, 7, 11, 16, 0, 12, 14, 18, 4, 9, 10, 13, 19, 17, },
// 		{ 3, 20, 6, 2, 5, 15, 7, 8, 11, 14, 16, 0, 1, 4, 9, 12, 13, 18, 19, 10, 17, },
// 		{ 4, 0, 9, 10, 12, 15, 16, 19, 13, 17, 18, 1, 2, 3, 5, 7, 8, 11, 14, 20, 6, },
// 		{ 5, 6, 1, 11, 2, 3, 8, 12, 15, 20, 0, 14, 16, 18, 7, 10, 17, 19, 4, 9, 13, },
// 		{ 6, 3, 5, 11, 20, 1, 2, 8, 15, 0, 14, 16, 7, 12, 18, 19, 9, 10, 13, 17, 4, },
// 		{ 7, 0, 2, 15, 3, 20, 1, 5, 6, 8, 11, 14, 16, 17, 4, 12, 13, 18, 19, 9, 10, },
// 		{ 8, 18, 2, 1, 5, 6, 20, 3, 11, 13, 15, 0, 7, 12, 14, 16, 17, 4, 9, 10, 19, },
// 		{ 9, 19, 10, 12, 13, 0, 4, 16, 18, 15, 1, 2, 3, 5, 6, 8, 11, 14, 17, 20, 7, },
// 		{ 10, 9, 12, 19, 13, 0, 4, 16, 18, 1, 5, 11, 15, 17, 2, 6, 8, 14, 3, 7, 20, },
// 		{ 11, 1, 5, 6, 2, 15, 20, 0, 3, 8, 12, 14, 16, 7, 10, 18, 19, 4, 9, 13, 17, },
// 		{ 12, 10, 9, 19, 5, 13, 0, 1, 4, 11, 15, 16, 17, 18, 2, 6, 8, 14, 3, 7, 20, },
// 		{ 13, 18, 17, 9, 10, 12, 8, 19, 0, 4, 15, 16, 1, 2, 3, 5, 6, 7, 11, 20, 14, },
// 		{ 14, 0, 3, 5, 6, 11, 15, 16, 1, 2, 7, 8, 12, 19, 20, 4, 9, 10, 18, 13, 17, },
// 		{ 15, 0, 2, 16, 3, 5, 6, 7, 11, 20, 1, 4, 8, 12, 14, 9, 10, 13, 18, 19, 17, },
// 		{ 16, 15, 0, 2, 19, 1, 3, 4, 5, 6, 9, 10, 11, 12, 14, 20, 7, 8, 13, 17, 18, },
// 		{ 17, 18, 13, 12, 4, 5, 7, 8, 10, 16, 0, 1, 6, 9, 11, 15, 19, 2, 3, 14, 20, },
// 		{ 18, 13, 8, 17, 5, 9, 10, 12, 19, 0, 1, 2, 4, 6, 11, 15, 16, 3, 7, 14, 20, },
// 		{ 19, 9, 10, 12, 0, 16, 4, 13, 18, 5, 6, 11, 14, 15, 1, 2, 3, 7, 8, 17, 20, },
// 		{ 3, 20, 2, 6, 5, 8, 11, 15, 1, 7, 16, 0, 14, 4, 9, 12, 13, 18, 19, 10, 17, },
//     };	




boost::unordered_map<char, int> BLOSUM_convert;
void load_BLOSUM_convert(boost::unordered_map<char, int> &BLOSUM_convert) {
  BLOSUM_convert['A'] = 0;
  BLOSUM_convert['a'] = 0;
  BLOSUM_convert['r'] = 1;
  BLOSUM_convert['R'] = 1;
  BLOSUM_convert['n'] = 2;
  BLOSUM_convert['N'] = 2;
  BLOSUM_convert['d'] = 3;
  BLOSUM_convert['D'] = 3;
  BLOSUM_convert['c'] = 4;
  BLOSUM_convert['C'] = 4;
  BLOSUM_convert['q'] = 5;
  BLOSUM_convert['Q'] = 5;
  BLOSUM_convert['e'] = 6;
  BLOSUM_convert['E'] = 6;
  BLOSUM_convert['g'] = 7;
  BLOSUM_convert['G'] = 7;
  BLOSUM_convert['h'] = 8;
  BLOSUM_convert['H'] = 8;
  BLOSUM_convert['i'] = 9;
  BLOSUM_convert['I'] = 9;
  BLOSUM_convert['l'] = 10;
  BLOSUM_convert['L'] = 10;
  BLOSUM_convert['k'] = 11;
  BLOSUM_convert['K'] = 11;
  BLOSUM_convert['m'] = 12;
  BLOSUM_convert['M'] = 12;
  BLOSUM_convert['f'] = 13;
  BLOSUM_convert['F'] = 13;
  BLOSUM_convert['p'] = 14;
  BLOSUM_convert['P'] = 14;
  BLOSUM_convert['s'] = 15;
  BLOSUM_convert['S'] = 15;
  BLOSUM_convert['t'] = 16;
  BLOSUM_convert['T'] = 16;
  BLOSUM_convert['w'] = 17;
  BLOSUM_convert['W'] = 17;
  BLOSUM_convert['y'] = 18;
  BLOSUM_convert['Y'] = 18;
  BLOSUM_convert['v'] = 19;
  BLOSUM_convert['V'] = 19;
  BLOSUM_convert['b'] = 20;//all amino acids that are not the 20 standard one are converted into the 21st one
  BLOSUM_convert['B'] = 20;
  BLOSUM_convert['z'] = 20;
  BLOSUM_convert['Z'] = 20;
  BLOSUM_convert['x'] = 20;
  BLOSUM_convert['X'] = 20;
  BLOSUM_convert['u'] = 20;
  BLOSUM_convert['U'] = 20;
  BLOSUM_convert['o'] = 20;
  BLOSUM_convert['O'] = 20;
}

//return the BLOSUM score for a given pair, if they are letters
inline int BLOSUM_score(char x, char y) {
  return BLOSUM80[BLOSUM_convert[x]][BLOSUM_convert[y]];
}

//return the BLOSUM score for a given pair, if they are numbers
inline int BLOSUM_score(int x, int y) {
  if ((x > 22) || (y > 22)) {
    cout << "parameters out of range for BLOSUM80" << endl;
    exit(2);
  }
  return BLOSUM80[x][y];
}
// all big integers for the selecting the size of the hashtables
const uint64_t hashTableSizes[] =
    {1769627, 1835027, 1900667, 1966127, 2031839, 2228483, 2359559, 2490707, 2621447, 2752679, 2883767, 3015527,
     3145739, 3277283, 3408323, 3539267, 3670259, 3801143, 3932483, 4063559, 4456643, 4718699, 4980827, 5243003,
     5505239, 5767187, 6029603, 6291563, 6553979, 6816527, 7079159, 7340639, 7602359, 7864799, 8126747, 8913119,
     9437399, 9962207, 10485767, 11010383, 11534819, 12059123, 12583007, 13107923, 13631819, 14156543, 14680067,
     15204467, 15729647, 16253423, 17825999, 18874379, 19923227, 20971799, 22020227, 23069447, 24117683, 25166423,
     26214743, 27264047, 28312007, 29360147, 30410483, 31457627, 32505983, 35651783, 37749983, 39845987, 41943347,
     44040383, 46137887, 48234623, 50331707, 52429067, 54526019, 56623367, 58720307, 60817763, 62915459, 65012279,
     71303567, 75497999, 79691867, 83886983, 88080527, 92275307, 96470447, 100663439, 104858387, 109052183, 113246699,
     117440699, 121635467, 125829239, 130023683, 142606379, 150994979, 159383759, 167772239, 176160779, 184549559,
     192938003, 201327359, 209715719, 218104427, 226493747, 234882239, 243269639, 251659139, 260047367, 285215507,
     301989959, 318767927, 335544323, 352321643, 369100463, 385876703, 402654059, 419432243, 436208447, 452986103,
     469762067, 486539519, 503316623, 520094747, 570425399, 603979919, 637534763, 671089283, 704643287, 738198347,
     771752363, 805307963, 838861103, 872415239, 905971007, 939525143, 973079279, 1006633283, 1040187419, 1140852767,
     1207960679, 1275069143, 1342177379, 1409288183, 1476395699, 1543504343, 1610613119, 1677721667, 1744830587,
     1811940419, 1879049087, 1946157419, 2013265967, 2080375127, 2281701827, 2415920939, 2550137039, 2684355383,
     2818572539, 2952791147, 3087008663, 3221226167, 3355444187, 3489661079, 3623878823, 3758096939, 3892314659,
     4026532187, 4160749883, 4563403379, 4831838783, 5100273923, 5368709219, 5637144743, 5905580687, 6174015503,
     6442452119, 6710886467, 6979322123, 7247758307, 7516193123, 7784629079, 8053065599, 8321499203, 9126806147,
     9663676523, 10200548819, 10737418883, 11274289319, 11811160139, 12348031523, 12884902223, 13421772839, 13958645543,
     14495515943, 15032386163, 15569257247, 16106127887, 16642998803, 18253612127, 19327353083, 20401094843,
     21474837719, 22548578579, 23622320927, 24696062387, 25769803799, 26843546243, 27917287907, 28991030759,
     30064772327, 31138513067, 32212254947, 33285996803, 36507222923, 38654706323, 40802189423, 42949673423,
     45097157927, 47244640319, 49392124247, 51539607599, 53687092307, 55834576979, 57982058579, 60129542339,
     62277026327, 64424509847, 66571993199, 73014444299, 77309412407, 81604379243, 85899346727, 90194314103,
     94489281203, 98784255863, 103079215439, 107374183703, 111669150239, 115964117999, 120259085183, 124554051983,
     128849019059, 133143986399, 146028888179, 154618823603, 163208757527, 171798693719, 180388628579, 188978561207,
     197568495647, 206158430447, 214748365067, 223338303719, 231928234787, 240518168603, 249108103547, 257698038539,
     266287975727, 292057776239, 309237645803, 326417515547, 343597385507, 360777253763, 377957124803, 395136991499,
     412316861267, 429496730879, 446676599987, 463856468987, 481036337207, 498216206387, 515396078039, 532575944723,
     584115552323, 618475290887, 652835029643, 687194768879, 721554506879, 755914244627, 790273985219, 824633721383,
     858993459587, 893353198763, 927712936643, 962072674643, 996432414899, 1030792152539, 1065151889507, 1168231105859,
     1236950582039, 1305670059983, 1374389535587, 1443109012607, 1511828491883, 1580547965639, 1649267441747,
     1717986918839, 1786706397767, 1855425872459, 1924145348627, 1992864827099, 2061584304323, 2130303780503,
     2336462210183, 2473901164367, 2611340118887, 2748779070239, 2886218024939, 3023656976507, 3161095931639,
     3298534883999, 3435973836983, 3573412791647, 3710851743923, 3848290698467, 3985729653707, 4123168604483,
     4260607557707, 4672924419707, 4947802331663, 5222680234139, 5497558138979, 5772436047947, 6047313952943,
     6322191860339, 6597069767699, 6871947674003, 7146825580703, 7421703488567, 7696581395627, 7971459304163,
     8246337210659, 8521215117407, 9345848837267, 9895604651243, 10445360463947, 10995116279639, 11544872100683,
     12094627906847, 12644383722779, 13194139536659, 13743895350023, 14293651161443, 14843406975659, 15393162789503,
     15942918604343, 16492674420863, 17042430234443, 18691697672867, 19791209300867, 20890720927823, 21990232555703,
     23089744183799, 24189255814847, 25288767440099, 26388279068903, 27487790694887, 28587302323787, 29686813951463,
     30786325577867, 31885837205567, 32985348833687, 34084860462083, 37383395344739, 39582418600883, 41781441856823,
     43980465111383, 46179488367203, 48378511622303, 50577534878987, 52776558134423, 54975581392583, 57174604644503,
     59373627900407, 61572651156383, 63771674412287, 65970697666967, 68169720924167, 74766790688867, 79164837200927,
     83562883712027, 87960930223163, 92358976733483, 96757023247427, 101155069756823, 105553116266999, 109951162779203,
     114349209290003, 118747255800179, 123145302311783, 127543348823027, 131941395333479, 136339441846019,
     149533581378263, 158329674402959, 167125767424739, 175921860444599, 184717953466703, 193514046490343,
     202310139514283, 211106232536699, 219902325558107, 228698418578879, 237494511600287, 246290604623279,
     255086697645023, 263882790666959, 272678883689987, 299067162755363, 316659348799919, 334251534845303,
     351843720890723, 369435906934019, 387028092977819, 404620279022447, 422212465067447, 439804651111103,
     457396837157483, 474989023199423, 492581209246163, 510173395291199, 527765581341227, 545357767379483,
     598134325510343, 633318697599023, 668503069688723, 703687441776707, 738871813866287, 774056185954967,
     809240558043419, 844424930134187, 879609302222207, 914793674313899, 949978046398607, 985162418489267,
     1020346790579903, 1055531162666507, 1090715534754863};

string seed_orig[] =
    {
        "110000110001",
        "100101000101",
        "110010001001",
        "101000000111"
    };

//protein dictionary for transfer p_name <-> p_id
boost::unordered_map<int, string> p_id_name;
boost::unordered_map<string, int> p_name_id;
boost::unordered_map<int, string> p_id_seq; //giving a protein id, return the protein sequence of it
//read protein sequences, fill the two maps -- p_id_name and p_name_id
void load_protein(string protein_fn) {
  ifstream fin(protein_fn.c_str());
  if (!fin) {
    cout << "loading protein file failed" << protein_fn << endl;
    exit(3);
  }
  string temp_pName;
  string temp_pSeq;
  int i = 0;
  while (fin >> temp_pName >> temp_pSeq) {
    p_id_name[i] = temp_pName.substr(1);
    p_name_id[temp_pName.substr(1)] = i;
    p_id_seq[i] = temp_pSeq;
    i++;
  }
  num_protein = i;
  num_old_protein = i;
  fin.close();

  //load new protein
  if (ONLY_COMPUTE_NEW_PROTEIN) {
    ifstream fin_new(NEW_PROTEIN_FN.c_str());
    if (!fin_new) {
      cout << "loading new protein file failed" << NEW_PROTEIN_FN << endl;
      exit(3);
    }
    cout << "reading added protein sequences from: " << NEW_PROTEIN_FN << endl;
    while (fin_new >> temp_pName >> temp_pSeq) {
      new_proteins.insert(i);
      p_id_name[i] = temp_pName.substr(1);
      p_name_id[temp_pName.substr(1)] = i;
      p_id_seq[i] = temp_pSeq;
      i++;
    }
    num_protein = i;
    fin_new.close();
  }

}
//from protein id to protein name
string PIDtoName(int id) {
  return p_id_name[id];
}

// from protein name to protein id
int PNametoID(string name) {
  return p_name_id[name];
}

char num_to_letter[25] =
    {'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V', 'B', 'Z', 'X',
     'U', 'O'};

int compare_smer_score(uint64_t smer1, uint64_t smer2, string seed) {
  int score = 0;
  uint64_t smer1_digit = 0;
  uint64_t smer2_digit = 0;
  string str_smer1;
  string str_smer2;
  for (uint32_t a = 0; a < seed.length(); a++) {
    if (seed.at((seed.length() - a - 1)) == '1') {
      smer1_digit = (smer1 >> (a * 5)) & 31;
      str_smer1 += num_to_letter[smer1_digit];
      smer2_digit = (smer2 >> (a * 5)) & 31;
      str_smer2 += num_to_letter[smer2_digit];
      score += BLOSUM80[smer1_digit][smer2_digit];
    }
  }
  cout << str_smer1 << " " << str_smer2 << " " << score << endl;
  return score;
}
void calculate_B80_order() {    //calculating the ordered_B80 matrix, not called, but precomputed by this function
  vector<int> temp;
  int max = -10;
  int index = 0;
  for (int a = 0; a < 21; a++) {
    for (int b = 0; b < 21; b++) {
      temp.push_back(BLOSUM80[a][b]);
    }
    for (int c = 0; c < 21; c++) {
      max = -10;
      for (uint32_t d = 0; d < temp.size(); d++) {
        if (temp[d] > max) {
          max = temp[d];
          index = d;
        }
      }
      temp[index] = -10;
      B80_ordered[a][c] = index;
    }
    temp.clear();
  }
  for (int i = 0; i < 21; i++) {
    cout << "{ ";
    for (int j = 0; j < 21; j++) {
      cout << B80_ordered[i][j] << ", ";
    }
    cout << "}," << endl;

  }
}

inline int compare_two_strings(string str1, string str2) {    //calculate the BLOSUM80 score for two strings
  int score = 0;
  for (uint32_t a = 0; a < str1.length(); a++) {
    score += BLOSUM_score(str1[a], str2[a]);
  }
  return score;
}

inline int compare_two_strings(int p1_id,
                               int p2_id,
                               int sta_1,
                               int sta_2,
                               int len) {    //calculate the BLOSUM80 score for two strings based on their position
  int score = 0;
  boost::unordered_map<int, string>::iterator p1_seq = p_id_seq.find(p1_id);
  boost::unordered_map<int, string>::iterator p2_seq = p_id_seq.find(p2_id);
  for (int a = 0; a < len; a++) {
    score += BLOSUM_score(p1_seq->second.at(sta_1 + a), p2_seq->second.at(sta_2 + a));
  }
  return score;
}

void assign_matrix(int M[23][23], int value[23][23]) {    //change sustitution matrix
  for (int a = 0; a < 23; a++) {
    for (int b = 0; b < 23; b++) {
      M[a][b] = value[a][b];
    }
  }
}

#endif
