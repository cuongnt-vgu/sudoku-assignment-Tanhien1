#pragma once

#include "sudoku.h"

//linked list to store found hidden singles
struct hidden_single_reserved{
    struct hidden_single_reserved *next;
    int candidate;
    int col;
    int row;
};

//insert a hidden single with its info
void insert(struct hidden_single_reserved **HEAD, int row, int col, int candidate);
//move through the linked list and remove all candidate other than hidden single of its node
void tranverse_linked_list (struct hidden_single_reserved *HEAD, SudokuBoard *board);
//free allocated memory from creating linked list
void free_linked_list(struct hidden_single_reserved *HEAD);

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board);

//check all cell in same row for 1's for specific candidate
bool check_row(SudokuBoard *board, int idRow, int idCol, int idCandidate);
//check all cell in same col for 1's for specific candidate
bool check_col(SudokuBoard *board, int idRow, int idCol, int idCandidate);
//check all cell in same box for 1's for specific candidate
bool check_box (SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate);

//remove all specific candidate in the same row, col, box 
void remove_candidates(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate);
//remove all other candidates of a cell if found a candidate to be hidden single
void self_remove_candidates(Cell *cell, int idCandidate);