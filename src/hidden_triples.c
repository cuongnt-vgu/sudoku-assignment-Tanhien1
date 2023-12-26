#include "hidden_triples.h"

#include "sudoku.h"

#include<stdlib.h>

int hidden_triples(SudokuBoard *board)
{
    //appear twice in same col/row/box in same cell
    //compare each candidate with one another for appearing twice in row/col/box SEPERATELY
    int num_hidden_triple =0;
    struct hidden_triple_reserved *HEAD = NULL;
    
    for (int row = 0; row < BOARD_SIZE; row++){
        for (int col =0; col <BOARD_SIZE; col++){
            
            Cell *specificCell = &board->data[row][col];
            //exclude naked pairs or fixed or input sol
            if (specificCell->num_candidates <= 3) continue;
            //convert all candidates into list of which each candidate
            //has to appear atleast twice in row/col/box
            //i = candidate1, j = candidate2, k = candidate3
            for (int i =0; i <BOARD_SIZE; i++){
                if (specificCell->candidates[i] == 1){
                    for (int j = i +1; j <BOARD_SIZE; j ++){
                        if (specificCell-> candidates[j] == 1){
                            for (int k = j + 1; k<BOARD_SIZE; k ++){
                                if (specificCell->candidates[k] == 1){
                                    bool temp = hidden_triplable(board,specificCell,i,j,k);
                                    if(temp){
                                        num_hidden_triple++;
                                        insert_hidden_triple(&HEAD,row,col,specificCell->box_index, i,j,k);
                                        goto newCell;
                                    }
                                }
                            }
                             
                        }
                    }
                }
            }
            newCell:   
        }
    }
    remove_remnant(HEAD,board);
    transverse_hidden_triple(board,HEAD);
    free_triple_linkedlist(HEAD);
    //bc there it were counting duplicate of 3, /3 to get real num
    if (num_hidden_triple %3 != 0){
        num_hidden_triple = (int)num_hidden_triple/3 + 1;
    }
    return num_hidden_triple;
}

bool *check_occourence3(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate){
    //check candidate appearing twice or thrice for row/col/box INDIVIDUALLY
    int resRow = 0, resCol = 0, resBox = 0;
    //0 = row, 1 = col, 2 = box 
    bool *list_appear_twice_thrice = malloc (sizeof(bool) * 3);
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
    // == 2 bc hidden triple candidates can only appear twice or thrice no more no less. 
    if (resRow == 2 || resRow == 3){
        list_appear_twice_thrice[0] = true;
    }
    else list_appear_twice_thrice[0] = false;

    if(resCol == 2 || resCol == 3){ 
        list_appear_twice_thrice[1] = true;
    }
    else list_appear_twice_thrice[1] = false;

    if(resBox == 2 || resBox == 3){
        list_appear_twice_thrice[2] = true;
    }
    else list_appear_twice_thrice[2] = false;

    return list_appear_twice_thrice;
}

bool hidden_triplable (SudokuBoard *board, Cell *specificCell, int idCandidate1, int idCandidate2, int idCandidate3){
    //compare 3 candidates of a cell for ROW/COL/BOX seperately
    bool *occourArr1 = check_occourence3(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate1 );
    bool *occourArr2 = check_occourence3(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate2 );
    bool *occourArr3 = check_occourence3(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate3 );
    bool is_hidden_triple = false;
    //if the sum of arr == 0 then it does not appear twice in either row/col/box, does not need to check
    if(is_empty_arr(occourArr1) || is_empty_arr(occourArr2) || is_empty_arr(occourArr3)){
        free(occourArr1);
        free(occourArr2);
        free(occourArr3);
        return false;
    }
    //return list from check_hidden_triple has 3 elements
    //0=row, 1=col, 2=box
    for (int i = 0; i <3 ; i++){
        //if 3 arr are same on an element and that element is true, and is_hidden_triple not yet verify
        if (occourArr1[i] == occourArr2[i] && occourArr2[i] == occourArr3[i] && occourArr1[i] == true && !is_hidden_triple) 
            is_hidden_triple = check_hidden_triple(board,specificCell -> row_index, specificCell->col_index, specificCell->box_index, idCandidate1, idCandidate2, idCandidate3,i);
    }
    free(occourArr1);
    free(occourArr2);
    free(occourArr3);
    return is_hidden_triple;
}


bool check_hidden_triple(SudokuBoard *board, int idRow, int idCol, int idBox, int idCandidate1, int idCandidate2, int idCandidate3, int n){
    //if unique == 2, there are 3 appearances of candidates
    // both of which are hidden triple
    //CHECK SEPERATE ROW/COL/BOX
    // n is the case to check row/col/box
    int uniqueRow = 0, uniqueCol = 0, uniqueBox = 0; 
    //keep track of how many time each hidden triple appear, has to atleast twice
    int candidateCounter1 = 0, candidateCounter2 = 0, candidateCounter3 = 0; 
    // transerse through same col and check how many time a candidate triple appear
    if (n == 1){
        for (int i =0; i <BOARD_SIZE; i++){
            if (i == idRow) continue;
            Cell *specificCell =&board->data[i][idCol];
            if(specificCell->num_candidates <= 3) continue;
            if (appear_atleast_twice(specificCell,idCandidate1,idCandidate2,idCandidate3, &candidateCounter1, &candidateCounter2, &candidateCounter3))
                uniqueCol++;
        }
        if (uniqueCol == 2
        && is_twice_thrice(candidateCounter1)
        && is_twice_thrice(candidateCounter2)
        && is_twice_thrice(candidateCounter3)) return true;
        else {
            //reseting
            candidateCounter1 = 0;
            candidateCounter2 = 0;
            candidateCounter3 = 0;
            return false;
        } 
    }
    // transerse through same row and check how many time a candidate triple appear
    if (n == 0){
        for (int i =0; i <BOARD_SIZE; i++){
            if (i == idCol) continue;
            Cell *specificCell =&board->data[idRow][i];
            if(specificCell->num_candidates <= 3) continue;
            if (appear_atleast_twice(specificCell,idCandidate1,idCandidate2,idCandidate3, &candidateCounter1, &candidateCounter2, &candidateCounter3))
                uniqueRow++;   
        }
        if (uniqueRow == 2
        && is_twice_thrice(candidateCounter1)
        && is_twice_thrice(candidateCounter2)
        && is_twice_thrice(candidateCounter3)) return true;
        else {
            //reseting
            candidateCounter1 = 0;
            candidateCounter2 = 0;
            candidateCounter3 = 0;
            return false;
        }
    }
    int currCol = (idBox %3) *3;
    int currRow = (int)(idBox/3) *3;
    // transerse through same box and check how many time a candidate triple appear
    if (n == 2){
        for (int i = currRow; i< currRow + 3; i++){
            for (int j =currCol; j<currCol + 3; j++){
                if (i == idRow && j == idCol){
                    continue;
                }
                Cell *specificCell = &board->data[i][j];
                if(specificCell->num_candidates <= 3) continue;
                if (appear_atleast_twice(specificCell,idCandidate1,idCandidate2,idCandidate3, &candidateCounter1, &candidateCounter2, &candidateCounter3))
                    uniqueBox++;
            }
        }
        if (uniqueBox == 2
        && is_twice_thrice(candidateCounter1)
        && is_twice_thrice(candidateCounter2)
        && is_twice_thrice(candidateCounter3)) return true;
        else {
            //reseting
            candidateCounter1 = 0;
            candidateCounter2 = 0;
            candidateCounter3 = 0;
            return false;
        }
    }
    
    return (uniqueRow == 2) || (uniqueCol == 2) || (uniqueBox == 2);
}

bool is_empty_arr(bool arr[]){
    for (int i = 0; i<3;i++){
        if (arr[i]){
            return false;
        }
    }
    return true;
}

bool appear_atleast_twice(Cell *cell, int idCandidate1, int idCandidate2, int idCandidate3, int *cc1, int *cc2, int *cc3){
    //check atleast 2 candidates appear in a cell
    // cc = candidateCounter
    //case: 110
    if (cell->candidates[idCandidate1] == 1
    && cell->candidates[idCandidate2] == 1
    && cell-> candidates[idCandidate3] == 0) {
        (*cc1)++;
        (*cc2)++;
        return true;
    }
    //case 101
    if (cell->candidates[idCandidate1] == 1
    && cell->candidates[idCandidate2] == 0
    && cell->candidates[idCandidate3] == 1) {
        (*cc1) ++;
        (*cc3)++;
        return true;
    }
    //case 011
    if (cell->candidates[idCandidate1] == 0
    && cell->candidates[idCandidate2] == 1
    && cell->candidates[idCandidate3] == 1) {
        (*cc2)++;
        (*cc3)++;
        return true;
    }
    //case 111
    if (cell->candidates[idCandidate1] == 1
    && cell->candidates[idCandidate2] == 1
    && cell->candidates[idCandidate3] == 1) {
        (*cc1)++;
        (*cc2)++;
        (*cc3)++;
        return true;
    }
    //if a cell exist only one of the candidate, -1 from total
    //coz then it considered falsch
    if(cell->candidates[idCandidate1]) (*cc1)--;
    if(cell->candidates[idCandidate1]) (*cc2)--;
    if(cell->candidates[idCandidate1]) (*cc3)--;
    
    return false;
}

bool is_twice_thrice (int n){
    // skiping the og cell so only check for one more or two more
    return (n == 1) || (n == 2);
}

void insert_hidden_triple(struct hidden_triple_reserved **HEAD, int row, int col,int box, int candidate1, int candidate2, int candidate3){
    struct hidden_triple_reserved *newNode = malloc(sizeof(struct hidden_triple_reserved));
    newNode->row = row;
    newNode->col = col;
    newNode->box = box;
    newNode->candidate1 = candidate1;
    newNode->candidate2 = candidate2;
    newNode->candidate3 = candidate3;
    newNode->next = *HEAD;
    *HEAD = newNode;
}

void transverse_hidden_triple(SudokuBoard *board,struct hidden_triple_reserved *HEAD){
    //NOT YET TRUE IN THIS CASE I THINK
    struct hidden_triple_reserved *temp = HEAD;
    while(temp != NULL){
        Cell *specificCell = &board->data[temp->row][temp->col];
        int size = sizeArr(specificCell,temp);
        int *tempArr = existing_candidate(specificCell,temp,size);
        reset_set_candidates(specificCell,tempArr,size); 
        temp = temp->next;
        free(tempArr);
    }
}

void free_triple_linkedlist(struct hidden_triple_reserved *HEAD){
    struct hidden_triple_reserved *current = HEAD;
    struct hidden_triple_reserved *next;
    while (current != NULL){
        next = current->next;
        free (current);
        current = next;
    }
}
void remove_remnant(struct hidden_triple_reserved *HEAD, SudokuBoard *board){
    // transverse through row/col/box in attemp to catch hidden triples that are 2/3
    for (int i = HEAD->col + 1; i <BOARD_SIZE; i++){
        Cell *cell = &board->data[HEAD->row][i];
        if (cell->candidates[HEAD->candidate1] == 0
        && cell->candidates[HEAD->candidate2] == 1
        && cell->candidates[HEAD->candidate3] == 1){
            int tempArr[2] = {HEAD->candidate2+1,HEAD->candidate3+1};
            reset_set_candidates(cell,tempArr,2);
        }
        if (cell->candidates[HEAD->candidate1] == 1
        && cell->candidates[HEAD->candidate2] == 0
        && cell->candidates[HEAD->candidate3] == 1){
            int tempArr[2] = {HEAD->candidate1+1,HEAD->candidate3+1};
            reset_set_candidates(cell,tempArr,2);
        }
        if (cell->candidates[HEAD->candidate1] == 1
        && cell->candidates[HEAD->candidate2] == 1
        && cell->candidates[HEAD->candidate3] == 0){
            int tempArr[2] = {HEAD->candidate1+1,HEAD->candidate2+1};
            reset_set_candidates(cell,tempArr,2);
        }
    }

}

int *existing_candidate(Cell *cell, struct hidden_triple_reserved *HEAD, int counter){
    //return arr of candidate of hidden triple of a cell
    
    int *arr = malloc(sizeof(int)*counter);
    int i = 0;
    if( cell->candidates[HEAD->candidate1] == 1){
        arr[i] = HEAD->candidate1 + 1;
        i++;
    }
    if( cell->candidates[HEAD->candidate2] == 1){
        arr[i] = HEAD->candidate2 + 1;
        i++;
    }
    if( cell->candidates[HEAD->candidate3] == 1){
        arr[i] = HEAD->candidate3 + 1;
        i++;
    }
    return arr;
}
int sizeArr(Cell *cell, struct hidden_triple_reserved *HEAD){
    //return size of candidate arr
    int size=0;
    if(cell->candidates[HEAD->candidate1] == 1) size++;
    if( cell->candidates[HEAD->candidate2] == 1) size++;
    if( cell->candidates[HEAD->candidate3] == 1) size++;
    return size;
}
