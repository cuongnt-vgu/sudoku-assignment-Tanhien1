#include "naked_triples.h"

#include "sudoku.h"
#include <stdlib.h>

int naked_triples(SudokuBoard *board)
{
    struct naked_triple_reserved *HEAD = NULL;
    int num_naked_triple = 0;
    for (int row =0; row <BOARD_SIZE; row++){
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

bool has_naked_triple (Cell *cell, int arr[], int size){
    // check for if that cell has any naked triple candidate that has not been removed
    for (int i =0; i<size; i++){
        if (cell->candidates[arr[i] - 1] == 1){
            return true; 
        }
    }
    return false;
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
    if (temp == NULL) return;
    int *candidateArr = temp->arrCandidate;
    int size = temp->arrSize;
    //adjust for it  to not remove itself and avoid naked triple cell
    // extremely sloppy attempt to move through all naked triple while not allowing removal of self
    // and skip already declared positions
    static int skip =0;
    if (skip == 0) skip = 3;
    //declare pos 1st cell
    static int col;
    static int row;
    static int box;
    //declare pos 2nd cell
    static int col2;
    static int row2;
    static int box2;
    //declare pos 3rd cell
    static int col3;
    static int row3;
    static int box3;
    if (skip == 3){
        //pos of 1st cell
        col = temp->col;
        row = temp->row;
        box = temp->box;
        //pos of 2nd cell
        temp= temp->next;
        col2 = temp->col;
        row2 = temp->row;
        box2 = temp->box;
        //pos of 3rd cell
        temp= temp->next;
        col3 = temp->col;
        row3 = temp->row;
        box3 = temp->box;
    }

    skip --;
    // n is case, 0=row 1=col 2=box
    int n = 9;
    if (col == col2 && col2 == col3) n=1;
    if (row == row2 && row2 == row3) n=0;
    if (box == box2 && box2 == box3) n=2;

    
    //transverse thru same row and unset naked triple candidates
    if (n==0){
        for (int i = 0; i <BOARD_SIZE; i++){
            if (i == col || i == col2 || i == col3) continue;
            Cell *cell = &board->data[row][i];
            if (!has_naked_triple(cell,candidateArr,size)) continue;
            for (int j =0; j <size; j ++ ){
                // so that it does not remove same candidate twice which leads to num candidate<real
                if (cell->candidates[candidateArr[j]-1] == 0) continue;
                unset_candidate(cell, candidateArr[j] );
            }
        }
    }
    //transverse thru same col and unset naked triple candidates
    if (n==1){
        for (int i = 0; i <BOARD_SIZE; i++){
            if (i == row || i == row2 || i == row3) continue;
            Cell *cell = &board->data[i][col];
            if (!has_naked_triple(cell,candidateArr,size)) continue;
            for (int j =0; j <size; j ++ ){
                // so that it does not remove same candidate twice which leads to num candidate<real
                if (cell->candidates[candidateArr[j]-1] == 0) continue;
                unset_candidate(cell, candidateArr[j] );
            }
        }
    }
    

    //transverse thru same box and unset naked triple candidates
    //calculate first cell of box
    if (n==2){
        int currCol = (box%3)*3;
        int currRow = (box/3)*3;
        for (int i = currRow; i< currRow + 3; i++){
            for (int j = currCol; j < currCol +3; j++){
                if (((i == row) || (i == row2) || (i == row3))
                && ((j == col) || (j == col2) || (j == col3))) continue;
                Cell *cell = &board->data[i][j];
                if (!has_naked_triple(cell,candidateArr,size)) continue;
                for (int k = 0; k <size; k++){
                    // so that it does not remove same candidate twice which leads to num candidate<real
                    if (cell->candidates[candidateArr[k]-1] == 0) continue;
                    unset_candidate(cell, candidateArr[k]);
                }
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