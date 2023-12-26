#include "hidden_pairs.h"

#include "sudoku.h"

#include<stdlib.h>

int hidden_pairs(SudokuBoard *board)
{
    //appear twice in same col/row/box in same cell
    //compare each candidate with one another for appearing twice in row/col/box SEPERATELY
    int num_hidden_pair =0;
    struct hidden_pair_reserved *HEAD = NULL;
    
    for (int row = 0; row < BOARD_SIZE; row++){
        for (int col =0; col <BOARD_SIZE; col++){

            Cell *specificCell = &board->data[row][col];
            //exclude naked pairs or fixed or input sol
            if (specificCell->num_candidates <= 2) continue;
            //convert all candidates into list of which each candidate
            //has to appear twice in row/col/box
            //i = candidate1, j = candidate2
            for (int i =0; i <BOARD_SIZE; i++){
                if (specificCell->candidates[i] == 1){
                    for (int j = i +1; j <BOARD_SIZE; j ++){
                        if (specificCell-> candidates[j] == 1){
                           bool temp = hidden_pairable(board,specificCell,i,j);
                            if (temp){
                                num_hidden_pair++;
                                insert_hidden_pair(&HEAD,row,col,i,j);  
                            } 
                        }
                    }
                }
            }   
        }
    }
    transerse_hidden_pair(board,HEAD);
    free_linkedlist(HEAD);
    //bc there it were counting duplicate of 2, /2 to get real num
    return num_hidden_pair/2;
}

bool *check_occourence(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate){
    //check candidate appearing twice for row/col/box INDIVIDUALLY
    int resRow = 0, resCol = 0, resBox = 0;
    //0 = row, 1 = col, 2 = box 
    bool *list_appear_twice = malloc (sizeof(bool) * 3);
    // transerse through same col and check how many time a candidate appear
    for (int i =0; i <BOARD_SIZE; i++){
        Cell *specificCell =&board->data[i][idCol];
        if(specificCell->candidates[idCandidate] == 1){
            resCol++;
        }
    }
    // transerse through same row and check how many time a candidate appear
    for (int i =0; i <BOARD_SIZE; i++){
        Cell *specificCell =&board->data[idRow][i];
        if(specificCell->candidates[idCandidate] == 1){
            resRow++;
        }
    }
    int currCol = (idBox %3) *3;
    int currRow = (int)(idBox/3) *3;
    // transerse through same box and check how many time a candidate appear
    for (int i = currRow; i< currRow + 3; i++){
        for (int j =currCol; j<currCol + 3; j++){
            Cell *specificCell = &board->data[i][j];
            if (specificCell->candidates[idCandidate] == 1){
                resBox++;
            }
        }
    }
    // == 2 bc hidden pair candidates can only appear twice no more no less. 
    if (resRow == 2){
        list_appear_twice[0] = true;
    }
    else list_appear_twice[0] = false;

    if(resCol == 2 ){ 
        list_appear_twice[1] = true;
    }
    else list_appear_twice[1] = false;

    if(resBox == 2){
        list_appear_twice[2] = true;
    }
    else list_appear_twice[2] = false;

    return list_appear_twice;
}

bool hidden_pairable (SudokuBoard *board, Cell *specificCell, int idCandidate1, int idCandidate2){
    //compare 2 candidates of a cell for ROW/COL/BOX seperately
    bool *occourArr1 = check_occourence(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate1 );
    bool *occourArr2 = check_occourence(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate2 );
    bool is_hidden_pair = false;
    //if the sum of arr == 0 then it does not appear twice in either row/col/box, does not need to check
    if(sumArr2(occourArr1) == 0 || sumArr2(occourArr2) == 0){
        free(occourArr1);
        free(occourArr2);
        return false;
    }
    //return list from check_hidden_pair has 3 elements
    //0=row, 1=col, 2=box
    for (int i = 0; i <3 ; i++){
        //if both arr are same on an element and that element is true, and is_hidden_pair not yet verify
        if (occourArr1[i] == occourArr2[i] && occourArr1[i] == true && !is_hidden_pair) 
            is_hidden_pair = check_hidden_pair(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate1, idCandidate2,i);
    }
    free(occourArr1);
    free(occourArr2);
    return is_hidden_pair;
}


bool check_hidden_pair(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2, int n){
    //if unique > 1, means there appears more than 2 cells which hold the same 2 candidates
    // which mean that 2 candidate is not hidden pair.
    //CHECK SEPERATE ROW/COL/BOX
    // n is the case to check row/col/box
    int uniqueRow = 0, uniqueCol = 0, uniqueBox = 0;
    // transerse through same col and check how many time a candidate pair appear
    if (n == 1){
        for (int i =0; i <BOARD_SIZE; i++){
            if (i == idRow) continue;
            Cell *specificCell =&board->data[i][idCol];
            if(specificCell->num_candidates <= 2) continue;
            if (specificCell->candidates[idCandidate1] == 1
            && specificCell->candidates[idCandidate2] == 1)
                uniqueCol++;
        }
    }
    // transerse through same row and check how many time a candidate pair appear
    if (n == 0){
        for (int i =0; i <BOARD_SIZE; i++){
            if (i == idCol) continue;
            Cell *specificCell =&board->data[idRow][i];
            if(specificCell->num_candidates <= 2) continue;
            if (specificCell->candidates[idCandidate1] == 1
            && specificCell->candidates[idCandidate2] == 1)
                uniqueRow++;;
            
        }
    }
    int currCol = (idBox %3) *3;
    int currRow = (int)(idBox/3) *3;
    // transerse through same box and check how many time a candidate pair appear
    if (n == 2){
        for (int i = currRow; i< currRow + 3; i++){
            for (int j =currCol; j<currCol + 3; j++){
                if (i == idRow && j == idCol){
                    continue;
                }
                Cell *specificCell = &board->data[i][j];
                if(specificCell->num_candidates <= 2) continue;
                if (specificCell->candidates[idCandidate1] == 1
                && specificCell->candidates[idCandidate2] == 1)
                    uniqueBox++;
            }
        }
    }
    
    return (uniqueRow == 1) || (uniqueCol == 1) || (uniqueBox == 1);
}

int sumArr2(bool arr[]){
    int res=0;
    for (int i = 0; i<3;i++){
        res+= arr[i];
    }
    return res;
}

void insert_hidden_pair(struct hidden_pair_reserved **HEAD, int row, int col, int candidate1, int candidate2){
    struct hidden_pair_reserved *newNode = malloc(sizeof(struct hidden_pair_reserved));
    newNode->row = row;
    newNode->col = col;
    newNode->candidate1 = candidate1;
    newNode->candidate2 = candidate2;
    newNode->next = *HEAD;
    *HEAD = newNode;
}

void transerse_hidden_pair(SudokuBoard *board,struct hidden_pair_reserved *HEAD){
    struct hidden_pair_reserved *temp = HEAD;
    while(temp != NULL){
        Cell *specificCell = &board->data[temp->row][temp->col];
        int tempArr[2] ={temp->candidate1 + 1,temp->candidate2 + 1};
        reset_set_candidates(specificCell,tempArr,2); 
        temp = temp->next;
    }
}

void free_linkedlist(struct hidden_pair_reserved *HEAD){
    struct hidden_pair_reserved *current = HEAD;
    struct hidden_pair_reserved *next;
    while (current != NULL){
        next = current->next;
        free (current);
        current = next;
    }
}
