#include "naked_triples.h"

#include "sudoku.h"
#include <stdlib.h>

int naked_triples(SudokuBoard *board)
{
    struct naked_triple_reserved *HEAD = NULL;
    int num_naked_triple = 0;
    for (int row =8; row <BOARD_SIZE; row++){
        for (int col =0; col<BOARD_SIZE; col++){
            Cell *cell = &board->data[row][col];
            if (not_num_2_3(cell)) continue;
            if (check_board(board,cell,HEAD, &HEAD)) num_naked_triple++;
        }
    }
    transverse_naked_triple(HEAD,board);
    free_naked_triple(HEAD);
    return num_naked_triple;
}

bool check_board(SudokuBoard *board, Cell *specifiCell, struct naked_triple_reserved *current, struct naked_triple_reserved **HEAD){
    // check col/row/box for another 2 instances of 3-num-candidates cell
    // current act s temp in this case, HEAD is the global point to linked list
    int row = specifiCell->row_index;
    int col = specifiCell->col_index;
    int box = specifiCell->box_index;

    //transverse thru same row twice, to look for 2 instances
    for (int i = col + 1; i< BOARD_SIZE; i++){
        Cell *cell2 = &board->data[row][i];
        if (not_num_2_3(cell2)) continue;
        for (int j = i + 1; j <BOARD_SIZE; j++ ){
            Cell *cell3 = &board->data[row][j];
            if (not_num_2_3(cell3)) continue;
            int *temp = sumCandidates(specifiCell->candidates,cell2->candidates,cell3->candidates);
            if (can_be_candidate(temp) && naked_tripable(temp)){
                if (is_naked_triple(current,specifiCell,cell2,cell3)){
                    free(temp);
                    //insert
                    insert_naked_triple(HEAD, specifiCell);
                    insert_naked_triple(HEAD, cell2);
                    insert_naked_triple(HEAD, cell3);
                    return true;
                }
            }
            free(temp);
        } 
    } 

    //transverse thru same col twice, to look for 2 instances
     for (int i = row + 1; i< BOARD_SIZE; i++){
        Cell *cell2 = &board->data[i][col];
            if (not_num_2_3(cell2)) continue;
        for (int j = i + 1; j <BOARD_SIZE; j++ ){
            Cell *cell3 = &board->data[j][col];
            if (not_num_2_3(cell3)) continue;
            int *temp = sumCandidates(specifiCell->candidates,cell2->candidates,cell3->candidates);
            if (can_be_candidate(temp)){
                if (naked_tripable(temp)){
                    if (is_naked_triple(current,specifiCell,cell2,cell3)){
                        free(temp);
                        insert_naked_triple(HEAD, specifiCell);
                        insert_naked_triple(HEAD, cell2);
                        insert_naked_triple(HEAD, cell3);
                        return true;
                    }
                }
            }
            free(temp);
        } 
    }
    
    //transverse thru same box twice, to look for 2 instances
    // calculate first cell of box
    int currRow = (int) (box/3)*3;
    int currCol = (col%3)*3;
    for (int i = row + 1; i < currRow + 3; i++){
        for (int j = col + 1; j <currCol +3; j++){
            Cell *cell2 = &board->data[i][j];
            if (not_num_2_3(cell2)) continue;
            for (int x = i + 1; x<currRow + 3; x++){
                for(int y = j+1; y<currCol + 3; y++){
                    Cell *cell3 = &board->data[x][y];
                    if(not_num_2_3(cell3)) continue;
                    int *temp = sumCandidates(specifiCell->candidates,cell2->candidates,cell3->candidates);
                    if (can_be_candidate(temp) && naked_tripable(temp)){
                        if (is_naked_triple(current,specifiCell,cell2,cell3)){
                            free(temp);
                            insert_naked_triple(HEAD, specifiCell);
                            insert_naked_triple(HEAD, cell2);
                            insert_naked_triple(HEAD, cell3);
                            return true;
                        }
                    }
                    free(temp);
                }
            }

        }
    }
    return false;
}

int *sumCandidates (int arrCell1[], int arrCell2[], int arrCell3[]){
    //add 3 candidates arr together, only those that have non-zeros = 3 passed
    int *total = malloc(sizeof(int) * BOARD_SIZE);
    //init arr = {0}
    for (int i = 0; i< BOARD_SIZE; i++){
        total[i] = 0;
    }
    for (int i=0; i<BOARD_SIZE; i++){
        total[i] += arrCell1[i] + arrCell2[i] + arrCell3[i];
    }
    return total;
}

bool can_be_candidate(int arr[]){
    //check arr for non zeros
    int non_zeros =0;
    for (int i =0; i <BOARD_SIZE; i++){
        if (arr[i] != 0){
            non_zeros++;
        }
    }
    return (non_zeros == 3);
}

bool naked_tripable(int arr[]){
    // if all non-zeros are atleast = 2 then is naked triple
    for (int i =0; i< BOARD_SIZE; i++){
        if(arr[i] != 0 && ((arr[i] >= 2) || (arr[i] <= 3)) ){
            return true;
        }
    }
    return false;
}

bool not_num_2_3 (Cell *cell){
    // if cell is not 2 or 3 candidates, true
    return (cell->num_candidates < 2) || (cell->num_candidates > 3);
}

bool comparePos(struct naked_triple_reserved *HEAD, Cell *checkedCell){
    //check cell vs all current naked triple in reservoir
    // false if found match, true otherwise 
    struct naked_triple_reserved *temp = HEAD;
    if (temp == NULL) return true;
    while (temp != NULL){
        if (temp->box == checkedCell->box_index
        && temp->col == checkedCell ->col_index
        && temp->row == checkedCell->row_index){
            return false;
        }
        temp = temp->next;    
    }
    return true;
}

bool is_naked_triple (struct naked_triple_reserved *HEAD, Cell *cell1, Cell *cell2, Cell *cell3){
    // if the three cells do not yet exist in reservoir, then they are naked triple
    struct naked_triple_reserved *temp = HEAD;
    return comparePos(temp, cell1) && comparePos(temp,cell2) && comparePos(temp, cell3);
}

//helper for linked list section
void insert_naked_triple(struct naked_triple_reserved **HEAD, Cell *cell){
    struct naked_triple_reserved *newNode = malloc(sizeof(struct naked_triple_reserved));
    newNode ->row = cell->row_index;
    newNode->col = cell->col_index;
    newNode->box = cell->box_index;
    newNode->arrCandidate = get_candidates(cell);
    newNode->arrSize= cell->num_candidates;
    newNode-> next = *HEAD;
    *HEAD = newNode;
}

void transverse_naked_triple(struct naked_triple_reserved *HEAD, SudokuBoard *board){
    struct naked_triple_reserved *temp = HEAD;
    while(temp != NULL){
        remove_candidate_naked_triple(temp,board);
        temp= temp->next;
    }
}

void remove_candidate_naked_triple (struct naked_triple_reserved *HEAD, SudokuBoard *board){
    //remove all naked triple candidate from other cells apart itself
    struct naked_triple_reserved *temp = HEAD;
    int col = temp->col;
    int row = temp->row;
    int box = temp->box;
    int *candidateArr = temp->arrCandidate;
    int size = temp->arrSize;
    //adjust for it  to not remove itself
    static int already =0;
    if (already == 0 || already == 1) already++;
    if (already == 2 ){
        already = 0;
        free(candidateArr);
        return;
    } 
    //transverse thru same row and unset naked triple candidates
    for (int i = 0; i <BOARD_SIZE; i++){
        if (i == col) continue;
        Cell *cell = &board->data[row][i];
        for (int j =0; j <size; j ++ ){
            unset_candidate(cell, candidateArr[j] + 1);
        }
    }
    //transverse thru same col and unset naked triple candidates
    for (int i = 0; i <BOARD_SIZE; i++){
        if (i == row) continue;
        Cell *cell = &board->data[i][col];
        for (int j =0; j <size; j ++ ){
            unset_candidate(cell, candidateArr[j] + 1);
        }
    }

    //transverse thru same box and unset naked triple candidates
    //calculate first cell of box
    int currCol = (box%3)*3;
    int currRow = (box/3)*3;
    for (int i = currRow; i< currRow + 3; i++){
        for (int j = currCol; j < currCol +3; j++){
            if (i == row && j == col) continue;
            Cell *cell = &board->data[i][j];
            for (int k = 0; k <size; k++){
                unset_candidate(cell, candidateArr[k]+1);
            }
        }
    }

    free(candidateArr);
}


void free_naked_triple (struct naked_triple_reserved *HEAD){
    struct naked_triple_reserved *current = HEAD;
    struct naked_triple_reserved *next;
    while(current != NULL){
        next = current->next;
        
        free (current);
        current = next;
    }
}