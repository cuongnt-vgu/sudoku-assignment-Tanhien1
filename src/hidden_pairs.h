#pragma once

#include "sudoku.h"

struct hidden_pair_reserved {
    struct hidden_pair_reserved *next;
    int row;
    int col;
    int candidate1;
    int candidate2;
    
};

int hidden_pairs(SudokuBoard *p_board);
//check a candidate appearing twice for same row/col/box
bool *check_occourence(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate);
//look at 2 candidates and check for any appearance of them in same row/col/box
bool check_hidden_pair(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2, int n);
//helper function to compare 2 candidate of their appearance in row/col/box
bool hidden_pairable (SudokuBoard *board, Cell *specificCell, int idCandidate1, int idCandidate2);
//helper function to calculate sum of an array
int sumArr2(bool arr[]);

//insert candidates into a linked list along with their info
void insert_hidden_pair(struct hidden_pair_reserved **HEAD, int row, int col, int candidate1, int candidate2);
//move through linked list and reset candidates
void transerse_hidden_pair(SudokuBoard *board,struct hidden_pair_reserved *HEAD);
//free linked list
void free_linkedlist(struct hidden_pair_reserved *HEAD);