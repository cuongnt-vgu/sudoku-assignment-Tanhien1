#include "naked_pairs.h"

#include "sudoku.h"
#include <stdlib.h>

int naked_pairs(SudokuBoard *board)
{
    int num_naked_pair =0;
    struct naked_pair_reserved *HEAD = NULL;
    for (int row =0; row< BOARD_SIZE; row++){
        for(int col = 0; col <BOARD_SIZE; col++){
            Cell *specificCell = &board->data[row][col];
            if(specificCell->num_candidates != 2) continue;

            for (int i = 0; i < BOARD_SIZE; i++){
                if (specificCell->candidates[i] == 1){
                    for (int j = i+1; j<BOARD_SIZE; j ++){
                        if (specificCell->candidates[j] == 1){
                            bool temp = naked_pairable(board,row,col,specificCell->box_index,i,j);
                            if(temp){
                                num_naked_pair++;
                                insert_naked_pair(&HEAD, row,col,specificCell->box_index,i,j);
                            }
                        }
                    }
                }
            }
        }
    }
    transverse_naked_pair(HEAD,board);
    free_naked_pair(HEAD);
    //duplicate of 2 so /2
    return num_naked_pair/2;
}

bool naked_pairable(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2){
    int uniqueRow = 0, uniqueCol = 0, uniqueBox =0;
    //transverse through same row
    for (int i = 0; i < BOARD_SIZE; i++){
        if (i == idCol) continue;
        Cell *cell = &board->data[idRow][i];
        if (cell->num_candidates != 2) continue;
        
        if(cell->candidates[idCandidate1] == 1
        && cell-> candidates[idCandidate2] == 1){
            uniqueRow++;
        }
    }
    //transverse through same col
     for (int i = 0; i < BOARD_SIZE; i++){
        if (i == idRow) continue;
        Cell *cell = &board->data[i][idCol];
        if (cell->num_candidates != 2) continue;
        
        if(cell->candidates[idCandidate1] == 1
        && cell-> candidates[idCandidate2] == 1){
            uniqueCol++;
        }
    }
    //transverse through same box
    int currRow = (int)(idBox/3)*3;
    int currCol = (idBox%3)*3;

    for(int i = currRow; i< currRow + 3; i++){
        for (int j = currCol; j < currCol +3; j++){
            if (i == idRow && j == idCol) continue;
            Cell *cell = &board->data[i][j];
            if(cell->num_candidates != 2) continue;
            if(cell->candidates[idCandidate1] == 1
            && cell-> candidates[idCandidate2] == 1){
                uniqueBox++;
            }
        }
    }
    return (uniqueRow == 1) || (uniqueCol == 1) || (uniqueBox == 1);
}

void insert_naked_pair(struct naked_pair_reserved **HEAD, int row, int col,int box, int candidate1, int candidate2){
    struct naked_pair_reserved *newNode = malloc (sizeof (struct naked_pair_reserved));
    newNode->row = row;
    newNode->col = col;
    newNode->box = box;
    newNode->candidate1 = candidate1;
    newNode->candidate2 = candidate2;
    newNode->next = *HEAD;
    *HEAD=newNode;
}

void transverse_naked_pair(struct naked_pair_reserved *HEAD, SudokuBoard *board){
    struct naked_pair_reserved *temp = HEAD;
    while(temp != NULL){
        remove_candidate_naked_pair(temp,board);
        temp = temp->next;
    }
}
void remove_candidate_naked_pair(struct naked_pair_reserved *HEAD, SudokuBoard *board){
    // can only remove same row/col/box, n is case
    // 0=row, 1=col, 2=box
    int n[3] = {0};
    // avoid checking 2 consecutive node coz they are the same
    bool is_already_checked = false;
    if(!is_already_checked){
        if(HEAD->next == NULL) return;
        struct naked_pair_reserved *pair1 = HEAD;
        struct naked_pair_reserved *pair2 = pair1 -> next;
        if (pair1->row == pair2->row ) n[0]=1;
        if (pair1->col == pair2->col ) n[1]=1;
        if (pair1->box == pair2->box ) n[2]=1;
        is_already_checked = true;
    }
    else is_already_checked = false;
    // move through same row
    if(n[0] == 1){
        for (int i = 0; i<BOARD_SIZE; i++){
            if(i == HEAD->col) continue;
            Cell *cell = &board->data[HEAD->row][i];
            //to avoid removing itself
            if(cell->num_candidates == 2
            && cell->candidates[HEAD->candidate1]==1
            && cell->candidates[HEAD->candidate2] == 1) continue;

            if (cell->candidates[HEAD->candidate1] == 1) unset_candidate(cell,HEAD->candidate1 + 1);
            if (cell->candidates[HEAD->candidate2] == 1) unset_candidate(cell,HEAD->candidate2 + 1);

        }
    }
    // move through same col
    if(n[1] == 1){
        for (int i = 0; i<BOARD_SIZE; i++){
            if(i == HEAD->row) continue;
            Cell *cell = &board->data[i][HEAD->col];
            //to avoid removing itself
            if(cell->num_candidates == 2
            && cell->candidates[HEAD->candidate1]==1
            && cell->candidates[HEAD->candidate2] == 1) continue;
            
            if (cell->candidates[HEAD->candidate1] == 1) unset_candidate(cell,HEAD->candidate1 + 1);
            if (cell->candidates[HEAD->candidate2] == 1) unset_candidate(cell,HEAD->candidate2 + 1);

        }
    }
    // move through same box
    if(n[2] == 1){
        int currRow = (int)(HEAD->box/3)*3;
        int currCol = (HEAD->box%3)*3;
        for (int i = currRow; i <currRow + 3; i ++){
            for(int j= currCol; j < currCol +3; j++){
                if(i == HEAD->row && j == HEAD ->col) continue;
                Cell *cell = &board->data[i][j];
                //to avoid removing itself
                if(cell->num_candidates == 2
                && cell->candidates[HEAD->candidate1]==1
                && cell->candidates[HEAD->candidate2] == 1) continue;
            
                if (cell->candidates[HEAD->candidate1] == 1) unset_candidate(cell,HEAD->candidate1 + 1);
                if (cell->candidates[HEAD->candidate2] == 1) unset_candidate(cell,HEAD->candidate2 + 1);
            }
        }
    }
}

void free_naked_pair (struct naked_pair_reserved *HEAD){
    struct naked_pair_reserved *current = HEAD;
    struct naked_pair_reserved *next;
    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}
