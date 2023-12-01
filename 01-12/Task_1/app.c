#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int* getFirstAndLastInt(char *arr){
    int arrLength = strlen(arr);
    int firstInt;
    int firstIntIteratorValue = -1;
    int lastInt;
    int lastIntIteratorValue = -1;

    for(int i = 0; i < arrLength; i++){
        if(arr[i] >= 48 && arr[i] <= 57){
            if(firstIntIteratorValue == -1){
                firstInt = arr[i];
                firstIntIteratorValue = i;
            } else {
                lastInt = arr[i];
                lastIntIteratorValue = i;
            }
        }
    }

    if(lastIntIteratorValue == -1){
        lastInt = firstInt;
    }

    char firstChar = (char)firstInt;
    char lastChar = (char)lastInt;
    printf("-----\nCurrentLine: %s\n", arr);
    printf("firstChar: %c\n", firstChar);
    printf("lastChar:  %c\n", lastChar);

    char concatString[] = {firstChar, lastChar, '\0'};
    size_t totalSize = sizeof(concatString);
    char *pOutput = (char*)calloc(totalSize, sizeof(char));

    strcpy(pOutput, concatString);

    int* parsedInt = (int*)calloc(3, sizeof(int)); 
    *parsedInt = atoi(pOutput);
    free(pOutput);

    return parsedInt;
}

int main() {
    FILE *file = fopen("document.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    char buffer[100]; // Assuming each line has at most 100 characters

    int accumulator = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int* pParsedInt = getFirstAndLastInt(buffer);
        accumulator += *pParsedInt;
        free(pParsedInt);        
    }

    fclose(file);
    printf("-----\nFinal sum: %i\n", accumulator);
    return 0;
}
