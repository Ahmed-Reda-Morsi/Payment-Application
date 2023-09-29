#include "card.h"
#include <time.h>   // to get OS date
#include <stdlib.h> // to use atoi function to covert string to int.



EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
}
//____________| implementation of getCardExpiryDate function   |_________________________
static uint8_t cardExpiredDate[6];                     // to hold cardExpiredDate 
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t errorStatus = CARD_OK; // return variable for error status

    uint8_t cardExpiredMonth[3] = {'\0', '\0', '\0'}; // string to hold card expired month
    uint8_t cardExpiredYear[3] = {'\0', '\0', '\0'};  // string to hold card expired year
    uint16_t cardExpiredYearInt;                      // variable to hold card expired month as int type
    uint8_t cardExpiredMonthInt;                      // variable to hold card expired year as int type

    uint8_t cardExpiredDate[6];
    printf("enter you card expiration date please:");
    scanf(" %s[^\n] ", &cardExpiredDate);

    if (strlen(cardExpiredDate) != 5)
    {
        errorStatus = WRONG_EXP_DATE;
    }
    else
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            // check of expired date has valid format or not
            if (!(((cardExpiredDate[i] <= '9' && cardExpiredDate[i] >= '0')) || (cardExpiredDate[i] == '/')))
            {
                errorStatus = WRONG_EXP_DATE; // update status error variable
                break;
            }
            else
            { /*DO NOTHING.*/
            }

            if (i < 2)
            {
                cardExpiredMonth[i] = cardExpiredDate[i]; // hold card expired month in string array
            }
            else if ((i == 2) && !(cardExpiredDate[i] == '/')) // check if third character is '/' or not
            {
                errorStatus = WRONG_EXP_DATE; // update status error variable
                break;
            }
            else if (i > 2)
            {
                cardExpiredYear[i - 3] = cardExpiredDate[i]; // hold card expired year in string array
            }
            else
            { /*DO NOTHING*/
            }
        }

        // convert and save into variable to avoid calling atoi function in if condition.
        cardExpiredMonthInt = atoi(cardExpiredMonth); // convert card expired month to int type
        // cardExpiredYearInt=(atoi(cardExpiredYear)+2000);                        // convert card expired year to int type

        // check if card expiration month is in range or not.
        if (cardExpiredMonthInt > 12)
        {
            errorStatus = WRONG_EXP_DATE; // update status error variable
        }

        if (errorStatus == CARD_OK)
        {
            strcpy(cardData->cardExpirationDate, cardExpiredDate); // update cardExpirationDate with user input
            printf("-> cardExpiredData is updated !\n");
        }
        else
        { /*DO NOTHING.*/
        }
    }

    return errorStatus;
}

//____________| implementation of getCardExpiryDate testing function  |_________________________
void getCardExpiryDateTest(void)
{
    //   test card module.
    printf("//__________________|getCardExpiryDate function testCases. |________________ \n");
    printf("Tester Name: Ahmed Reda\nFunction Name: isCardExpried\n");

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t expectedResult = 0;
        printf("enter the expected Result Please (CARD_OK -> 0 | WRONG_EXP_DATE -> 2): ");
        scanf("%d", &expectedResult);
        ST_cardData_t cardDataTest;
        uint8_t callingResult = getCardExpiryDate(&cardDataTest); // get result of calling getCardExpiryDate

        if (expectedResult == CARD_OK && callingResult == CARD_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "CARD_OK", "CARD_OK");
        }
        else if (expectedResult == WRONG_EXP_DATE && callingResult == CARD_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "WRONG_EXP_DATE", "CARD_OK");
        }
        else if (expectedResult == WRONG_EXP_DATE && callingResult == WRONG_EXP_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "WRONG_EXP_DATE", "WRONG_EXP_DATE");
        }
        else if (expectedResult == CARD_OK && callingResult == WRONG_EXP_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "CARD_OK", "WRONG_EXP_DATE");
        }
        else
        { /*DO NOTHING.*/
        }
    }
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
}