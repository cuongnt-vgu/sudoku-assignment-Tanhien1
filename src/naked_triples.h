#pragma once

#include "sudoku.h"

struct naked_triple_reserved{
    struct naked_triple_reserved *next;
    int col;
    int row;
    int box;
    int *arrCandidate;
    int arrSize;
};


int naked_triples(SudokuBoard *board);

//main fuction to check for naked triple
bool check_board(SudokuBoard *board, Cell *specifiCell, struct naked_triple_reserved *HEAD);
// output an array that is the sum of 3 candidates array of 3 cel
int *sumCandidates (int arrCell1[], int arrCell2[], int arrCell3[]);
//check sum arr for total of non-zeros = 3
bool can_be_candidate(int arr[]);
//if all appearence of sum arr of non-zeros atleast 2 then is can naked triple
bool naked_tripable(int arr[]);
//check if cell has 2 or 3 num_candidates then false
bool not_num_2_3 (Cell *cell);
//compare with naked_triple_reserved, if found same col,row,box then is duplicate
bool comparePos(struct naked_triple_reserved *HEAD, Cell *checkedCell);
//compare all three cell for dulplicate (the naked triple has to be brand new)
bool is_naked_triple (struct naked_triple_reserved *HEAD, Cell *cell1, Cell *cell2, Cell *cell3);

//linked list helper functions section
//insert a cell into linkedlist
void insert_naked_triple(struct naked_triple_reserved **HEAD, Cell *cell);
//move thru linked list and remove naked triple candidate other than itself
void transverse_naked_triple(struct naked_triple_reserved *HEAD, SudokuBoard *board);
//remove all naked triple candidate other than self in row/col/box
void remove_candidate_naked_triple (struct naked_triple_reserved *HEAD, SudokuBoard *board);
//free linkedlist
void free_naked_triple (struct naked_triple_reserved *HEAD);