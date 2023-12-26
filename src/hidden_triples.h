#pragma once

#include "sudoku.h"

struct hidden_triple_reserved {
    struct hidden_triple_reserved *next;
    int row;
    int col;
    int box;
    int candidate1;
    int candidate2;
    int candidate3;
};

int hidden_triples(SudokuBoard *board);
//check a candidate appearing twice for same row/col/box
bool *check_occourence3(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate);
//look at 3 candidates and check for any appearance of them in same row/col/box
bool check_hidden_triple(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2, int idCandidate3, int n);
//helper function to compare 3 candidate of their appearance in row/col/box
bool hidden_triplable (SudokuBoard *board, Cell *specificCell, int idCandidate1, int idCandidate2, int idCandidate3);
//helper function to check if arr is empty 
bool is_empty_arr(bool arr[]);
//helper function to find appearence of 2 or more candidates in a cell
bool appear_atleast_twice(Cell *cell, int idCandidate1, int idCandidate2, int idCandidate3, int *cc1, int *cc2, int *cc3);
//check if a candidate appear atleast twice in all hidden triple cell
bool is_twice_thrice(int n);

//insert candidates into a linked list along with their info
void insert_hidden_triple(struct hidden_triple_reserved **HEAD, int row, int col,int box, int candidate1, int candidate2, int candidate3);
//move through linked list and reset candidates
void transverse_hidden_triple(SudokuBoard *board,struct hidden_triple_reserved *HEAD);
//sloppy attemp to catch hidden triple that are 2/3
void remove_remnant(struct hidden_triple_reserved *HEAD, SudokuBoard *board);
//free linked list
void free_triple_linkedlist(struct hidden_triple_reserved *HEAD);

//helper function to return an arr with candidate of hidden triple of a cell
int *existing_candidate(Cell *cell, struct hidden_triple_reserved *HEAD, int counter);
//helper function to find size of candidate arr
int sizeArr(Cell *cell, struct hidden_triple_reserved *HEAD);
