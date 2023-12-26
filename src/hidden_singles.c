#include "hidden_singles.h"

#include "sudoku.h"

#include <stdio.h>
#include <stdlib.h>

int hidden_singles(SudokuBoard *board)
{
    struct hidden_single_reserved *HEAD = NULL;

    int num_hidden_single = 0 ;
    for (int row = 0; row<BOARD_SIZE; row++ ){
        for (int col = 0; col < BOARD_SIZE; col++){
            Cell *specificCell = &board->data[row][col];
            // if the cell is default value, skip it
            if (specificCell-> num_candidates == 1) continue;
            // else, check each candidate
            for (int i=0; i<BOARD_SIZE; i++){
                if(specificCell->candidates[i] == 1){
                    if (check_row(board,row,col,i)
                    || check_col(board,row,col,i)
                    || check_box(board,row,col,specificCell->box_index,i) ){
                        num_hidden_single++;
                        insert(&HEAD,row,col,i);
                    }
                }

            }
        }
    }
    tranverse_linked_list(HEAD,board);
    free_linked_list(HEAD);
    return num_hidden_single;
}
bool check_row(SudokuBoard *board, int idRow, int idCol, int idCandidate){
    bool unique = true;
    //check row for 1's other than itself
    for (int i = 0; i < BOARD_SIZE; i++){
        if (i == idCol){
            continue;
        }
        Cell *specificCell = &board->data[idRow][i];
        if (specificCell->candidates[idCandidate] == 1){
            unique = false;
            break;
        }
    }
    return unique;
}

bool check_col(SudokuBoard *board, int idRow, int idCol, int idCandidate){
    bool unique = true;
    //check col for 1's other than itself
    for (int i = 0; i < BOARD_SIZE; i++){
        if (i == idRow){
            continue;
        }
        Cell *specificCell = &board->data[i][idCol];
        if (specificCell->candidates[idCandidate] == 1){
            unique = false;
            break;
        }
    }
    return unique;
}
bool check_box (SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate){
    bool unique = true; 
    // calculate which col and row correspond to the current box
    int currCol = (idBox %3) *3;
    int currRow = (int)(idBox/3) *3;
    // check box for 1's other than itself
    for (int i = currRow; i< currRow + 3; i++){
        for (int j =currCol; j<currCol + 3; j++){
            if (i == idRow && j == idCol){
                continue;
        }
            Cell *specificCell = &board->data[i][j];
            if (specificCell->candidates[idCandidate] == 1){
                unique = false;
                goto end;
        }
        }
    }
    end:
    return unique;
}

void self_remove_candidates(Cell *cell, int idCandidate){
    //if found to be hidden single, remove all other candidates of that cell
    for (int i = 0; i <BOARD_SIZE; i++){
        if (i == idCandidate){
            continue;
        }
        if(cell->candidates[i] == 1){
            unset_candidate(cell, i+1);
        }
    }
}

void remove_candidates(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate){
    // remove all candidate of the same num as hidden single on same col
    for (int i =0; i <BOARD_SIZE; i++){
        if (i == idRow) continue;
        Cell *specificCell =&board->data[i][idCol];
        if(specificCell->candidates[idCandidate] == 1){
            unset_candidate(specificCell,idCandidate + 1);
        }
    }
    // remove candidate of the same num as hidden single on same row
    for (int i =0; i <BOARD_SIZE; i++){
        if (i == idCol) continue;
        Cell *specificCell =&board->data[idRow][i];
        if(specificCell->candidates[idCandidate] == 1){
            unset_candidate(specificCell,idCandidate + 1);
        }
    }
    int currCol = (idBox %3) *3;
    int currRow = (int)(idBox/3) *3;
    // remove candidate of same num as hidden single on same box
    for (int i = currRow; i< currRow + 3; i++){
        for (int j =currCol; j<currCol + 3; j++){
            if (i == idRow && j == idCol){
                continue;
        }
            Cell *specificCell = &board->data[i][j];
            if (specificCell->candidates[idCandidate] == 1){
                unset_candidate(specificCell,idCandidate +1);
        }
        }
    }

}

void insert(struct hidden_single_reserved **HEAD, int row, int col, int candidate){
    //insert found hidden singles via their info
    struct hidden_single_reserved *newNode = malloc(sizeof(struct hidden_single_reserved));
    newNode->col = col;
    newNode->row = row;
    newNode->candidate = candidate;
    newNode->next = *HEAD;
    *HEAD = newNode;
}

void tranverse_linked_list (struct hidden_single_reserved *HEAD, SudokuBoard *board){
    // move through linked list and remove candidates that is not hidden single
    struct hidden_single_reserved *temp = HEAD;
    while (temp != NULL){
        Cell *cell = &board->data[temp->row][temp->col];
        self_remove_candidates(cell,temp->candidate);
        temp = temp->next;
    }
}
void free_linked_list(struct hidden_single_reserved *HEAD){
    //free allocated memory for linked list
    struct hidden_single_reserved *current = HEAD;
    struct hidden_single_reserved *next;
    while (current != NULL){
        next = current->next;
        free (current);
        current = next;
    }
}
