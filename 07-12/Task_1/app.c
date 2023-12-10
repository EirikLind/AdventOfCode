#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

int *fiveI;
int *fourI;
int *threeI;
int *pairI;
int *distinctI;
int *houseI;
int *twoPairI;
long *sum;

enum PokerValue
{
    FiveOfAKind = 7,
    FourOfAKind = 6,
    FullHouse = 5,
    ThreeOfAkind = 4,
    TwoPairs = 3,
    TwoOfAKind = 2,
    Distinct = 1,
    Nothing = 0
};

struct PokerHand
{
    enum PokerValue handValue; // Represents int handValue of valid poker values
    int bidValue;              // Multiply bid with hand rank
    int orgHandToInt[5];       // Origial hand but translated to 1-13
    int handRank;              // Global hand ranking, weakest is 1
};

struct PokerHand *FiveOfAKindArr;
struct PokerHand *FourOfAKindArr;
struct PokerHand *FullHouseArr;
struct PokerHand *ThreeOfAkindArr;
struct PokerHand *TwoPairsArr;
struct PokerHand *TwoOfAKindArr;
struct PokerHand *DistinctArr;

int valueArray[13]; // Used to rank each card in each hand
int *gi;

struct SplitString
{
    char *firstPart;
    char *lastPart;
};

void throwErr(char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

void *myMalloc(size_t s, char *varName)
{
    void *p = malloc(s);
    if (p == NULL)
    {
        char errMsg[100];
        sprintf(errMsg, "Failed to allocate memory for %s\n", varName);
        throwErr(errMsg);
    }
}

struct SplitString splitString(char *source, char splitOn)
{
    char *ptr = strchr(source, splitOn);
    if (ptr == NULL)
    {
        struct SplitString result = {'\0', '\0'};
        return result;
    }

    size_t index = ptr - source;
    *ptr = '\0';
    char *secondPart = ptr + 1;
    struct SplitString result = {source, secondPart};
    return result;
}

struct PokerHand *resizeArray(struct PokerHand *array, int newSize, char *varName)
{
    // To avoid Null pointer
    if (newSize == 0){
        newSize = 1;
    }

    struct PokerHand *newArray = realloc(array, newSize * sizeof(struct PokerHand));
    if (newArray == NULL)
    {
        char errMsg[100];
        sprintf(errMsg, "Failed to allocate memory for %s", varName);
        throwErr(errMsg);
    }
    return newArray;
}

// Sorts hand_a vs hand_b based on value of leftmost card, highest value becomes highest index, index 0 is weakest hand
int sortAlg(const void *a, const void *b)
{
    struct PokerHand hand_a = *((struct PokerHand *)a);
    struct PokerHand hand_b = *((struct PokerHand *)b);

    for (int i = 0; i < 5; i++)
    {
        if (hand_a.orgHandToInt[i] == hand_b.orgHandToInt[i])
            continue;
        if (hand_a.orgHandToInt[i] < hand_b.orgHandToInt[i])
            return -1;
        return 1;
    }
}

// start rank begins where the last ended
int assignGlobalHandRank(struct PokerHand *arr, int arrLen, int startRank)
{
    int i;
    for (i = 0; i < arrLen; i++)
    {
        arr[i].handRank = i + startRank;
        *sum = *sum + ((i + startRank) * arr[i].bidValue);
        printf("bid %d * rank %d = %d\nSum: %d\n", arr[i].bidValue, (i + startRank), ((i + startRank) * arr[i].bidValue), *sum);
    }
    return i + startRank;
}

int determinePokerHand(struct PokerHand *hand)
{
    int compositeHand[2];
    int chi = 0;
    int straightStreak = 0;
    int i;

    for (i = 0; i < sizeof(valueArray) / sizeof(int); i++)
    {
        int curVal = valueArray[i];
        switch (curVal)
        {
        case 5:
            hand->handValue = FiveOfAKind;
            FiveOfAKindArr[*fiveI] = *hand;
            (*fiveI)++;
            break;
        case 4:
            hand->handValue = FourOfAKind;
            FourOfAKindArr[*fourI] = *hand;
            (*fourI)++;
            break;
        case 3:
            compositeHand[chi] = ThreeOfAkind;
            chi++;
            break;
        case 2:
            compositeHand[chi] = TwoOfAKind;
            chi++;
            break;
        case 1:
            break;
        default:
            break;
        }
    }

    switch (chi)
    {
    // Composite Index being 0 means Distinct
    case 0:
        if(hand->handValue == 0){
            printf("Hand index %i was Distinct!\n", *gi);
            hand->handValue = Distinct;
            DistinctArr[*distinctI] = *hand;
            (*distinctI)++;
        }
        break;

    // Composite Index being 1 means either ThreeOfAKind or TwoOfAKind
    case 1:
        if (compositeHand[0] == 2)
        {
            printf("Hand index %i was TwoOfAKind\n", *gi);
            hand->handValue = TwoOfAKind;
            TwoOfAKindArr[*pairI] = *hand;
            (*pairI)++;
        }
        else if (compositeHand[0] == 4)
        {
            printf("Hand index %i was ThreeOfAkind\n", *gi);
            hand->handValue = ThreeOfAkind;
            ThreeOfAkindArr[*threeI] = *hand;
            (*threeI)++;
        }
        else
        {
            printf("Hand index %i was Not possible, good job you\n", *gi);
            throwErr("Impossible hand detected 🤔\n");
        }
        break;

    // Composite Index being 2 means either TwoPairs or FullHouse
    case 2:
        int compVal = compositeHand[0] + compositeHand[1];
        if (compVal == 4)
        {
            printf("Hand index %i was TwoPairs\n", *gi);
            hand->handValue = TwoPairs;
            TwoPairsArr[*twoPairI] = *hand;
            (*twoPairI)++;
        }
        else if (compVal == 6)
        {
            printf("Hand index %i was FullHouse\n", *gi);
            hand->handValue = FullHouse;
            FullHouseArr[*houseI] = *hand;
            (*houseI)++;
        }
        else
        {
            printf("Hand index %i was Not possible, good job you\n", *gi);
            throwErr("Impossible hand detected 🤔\n");
        }
        break;
    default:
        break;
    }
}

void parseLine(char *currentline)
{
    if (*currentline == '\n' || *currentline == '\000' || *currentline == '\r')
    {
        (*gi)--;
        return;
    }

    memset(valueArray, 0, sizeof(valueArray));
    int i;

    struct SplitString res = splitString(currentline, ' ');
    if(res.firstPart == '\0' || res.lastPart == '\0')     {
        (*gi)--;
        return;
    }
    int bid = atoi(res.lastPart);

    struct PokerHand hand;
    hand.bidValue = bid;

    for (i = 0; i < strlen(res.firstPart); i++)
    {
        char curVal = res.firstPart[i];
        switch (curVal)
        {
        case 'A':
            valueArray[12] = valueArray[12] + 1;
            hand.orgHandToInt[i] = 13;
            break;
        case 'K':
            valueArray[11] = valueArray[11] + 1;
            hand.orgHandToInt[i] = 12;
            break;
        case 'Q':
            valueArray[10] = valueArray[10] + 1;
            hand.orgHandToInt[i] = 11;
            break;
        case 'J':
            valueArray[9] = valueArray[9] + 1;
            hand.orgHandToInt[i] = 10;
            break;
        case 'T':
            valueArray[8] = valueArray[8] + 1;
            hand.orgHandToInt[i] = 9;
            break;
        case '9':
            valueArray[7] = valueArray[7] + 1;
            hand.orgHandToInt[i] = 8;
            break;
        case '8':
            valueArray[6] = valueArray[6] + 1;
            hand.orgHandToInt[i] = 7;
            break;
        case '7':
            valueArray[5] = valueArray[5] + 1;
            hand.orgHandToInt[i] = 6;
            break;
        case '6':
            valueArray[4] = valueArray[4] + 1;
            hand.orgHandToInt[i] = 5;
            break;
        case '5':
            valueArray[3] = valueArray[3] + 1;
            hand.orgHandToInt[i] = 4;
            break;
        case '4':
            valueArray[2] = valueArray[2] + 1;
            hand.orgHandToInt[i] = 3;
            break;
        case '3':
            valueArray[1] = valueArray[1] + 1;
            hand.orgHandToInt[i] = 2;
            break;
        case '2':
            valueArray[0] = valueArray[0] + 1;
            hand.orgHandToInt[i] = 1;
            break;
        default:
            break;
        }
    }

    determinePokerHand(&hand);
}

int main()
{
    FILE *file = fopen("document.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }

    gi = myMalloc(sizeof(gi), "gi");
    sum = myMalloc(sizeof(sum), "sum");
    fiveI = myMalloc(sizeof(fiveI), "fiveI");
    fourI = myMalloc(sizeof(fourI), "fourI");
    threeI = myMalloc(sizeof(threeI), "threeI");
    pairI = myMalloc(sizeof(pairI), "pairI");
    distinctI = myMalloc(sizeof(distinctI), "straightI");
    houseI = myMalloc(sizeof(houseI), "houseI");
    twoPairI = myMalloc(sizeof(twoPairI), "twoPairI");

    int startSize = 500;
    FiveOfAKindArr = myMalloc(sizeof(struct PokerHand) * startSize, "FiveOfAKindArr");
    FourOfAKindArr = myMalloc(sizeof(struct PokerHand) * startSize, "FourOfAKindArr");
    FullHouseArr = myMalloc(sizeof(struct PokerHand) * startSize, "FullHouseArr");
    ThreeOfAkindArr = myMalloc(sizeof(struct PokerHand) * startSize, "ThreeOfAkindArr");
    TwoPairsArr = myMalloc(sizeof(struct PokerHand) * startSize, "TwoPairsArr");
    TwoOfAKindArr = myMalloc(sizeof(struct PokerHand) * startSize, "TwoOfAKindArr");
    DistinctArr = myMalloc(sizeof(struct PokerHand) * startSize, "StraightArr");

    *gi = 0;
    *sum = 0;
    *fiveI = 0;
    *fourI = 0;
    *threeI = 0;
    *pairI = 0;
    *distinctI = 0;
    *houseI = 0;
    *twoPairI = 0;

    char buffer[11]; // Maximum line size is 10 characters + \n
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        size_t len = strlen(buffer);
        if(len > 0 && buffer[len-1] == '\n'){
            buffer[len-1] = '\0';
        }
        parseLine(buffer);
        (*gi)++;
    }

    qsort(DistinctArr, *distinctI, sizeof(struct PokerHand), sortAlg);
    qsort(TwoOfAKindArr, *pairI, sizeof(struct PokerHand), sortAlg);
    qsort(TwoPairsArr, *twoPairI, sizeof(struct PokerHand), sortAlg);
    qsort(ThreeOfAkindArr, *threeI, sizeof(struct PokerHand), sortAlg);
    qsort(FullHouseArr, *houseI, sizeof(struct PokerHand), sortAlg);
    qsort(FourOfAKindArr, *fourI, sizeof(struct PokerHand), sortAlg);
    qsort(FiveOfAKindArr, *fiveI, sizeof(struct PokerHand), sortAlg);

    int hr = 1; // HandRank
    hr = assignGlobalHandRank(DistinctArr, *distinctI, hr);
    hr = assignGlobalHandRank(TwoOfAKindArr, *pairI, hr);
    hr = assignGlobalHandRank(TwoPairsArr, *twoPairI, hr);
    hr = assignGlobalHandRank(ThreeOfAkindArr, *threeI, hr);
    hr = assignGlobalHandRank(FullHouseArr, *houseI, hr);
    hr = assignGlobalHandRank(FourOfAKindArr, *fourI, hr);
    assignGlobalHandRank(FiveOfAKindArr, *fiveI, hr);

    printf("Sum is: %d\n", *sum);

    fclose(file);
    free(gi);
    free(fiveI);
    free(fourI);
    free(threeI);
    free(pairI);
    free(distinctI);
    free(houseI);
    free(twoPairI);
    free(FiveOfAKindArr);
    free(FourOfAKindArr);
    free(FullHouseArr);
    free(ThreeOfAkindArr);
    free(TwoPairsArr);
    free(TwoOfAKindArr);
    free(DistinctArr);
    return 0;
}