#pragma once

#include "sudoku.h"

struct naked_pair_reserved {
    struct naked_pair_reserved *next;
    int row;
    int col;
    int box;
    int candidate1;
    int candidate2;
};
int naked_pairs(SudokuBoard *board);
//look at 2 candidate and check for instance of them in same row/col/box seperartely
bool naked_pairable(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2);

//helper function to add found naked pair into a linked list
void insert_naked_pair(struct naked_pair_reserved **HEAD, int row, int col,int box, int candidate1, int candidate2);
//helper function to transverse through linked list
void transverse_naked_pair(struct naked_pair_reserved *HEAD, SudokuBoard *board);
//transverse through same row/col/box and remove move candidate of naked pair from them
void remove_candidate_naked_pair(struct naked_pair_reserved *HEAD, SudokuBoard *board);
//helper function to free allocated memory for linked list
void free_naked_pair (struct naked_pair_reserved *HEAD);