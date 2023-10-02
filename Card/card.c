
#include <stdio.h>// to use atoi function to covert string to int.
#include "card.h"
#include <time.h>   // to get OS date
#include <stdlib.h> // to use atoi function to covert string to int.

static uint8_t cardExpiredDate[6];

uint8_t isNameValid(const char *name)
{

    // Check if the entered characters is valid
    for (uint8_t i = 0; i < strlen(name) - 1; i++)
    {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
                (name[i] >= 'A' && name[i] <= 'Z') ||
                (name[i] == ' ')))
        {
            return 0;
        }
    }
    return 1;
}

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    char holderName[MAX_NAME_LENGTH + 1]; // Buffer to hold the card holder's name
    uint8_t counter = 0; // Counter variable for iteration

    printf("Please Enter Your Name : \n"); // Prompt the user to enter their name
    fgets(holderName, sizeof(holderName), stdin); // Read the user's input and store it in holderName

    if (!isNameValid(holderName))
    {
        return WRONG_NAME; // Name contains invalid characters
    }

    // Check various conditions to determine the validity of the name
    if ((holderName[0] == '\0') || (strlen(holderName) < MIN_NAME_LENGTH) || (strlen(holderName) > MAX_NAME_LENGTH))
    {
        return WRONG_NAME; // Return the error code for wrong name
    }
    else
    {
        strcpy(cardData->cardHolderName,holderName); // Copy the valid name to the cardData structure
        return CARD_OK; // Return the success code
    }

}

void getCardHolderNameTest(void)
{
    ST_cardData_t cardData;
    EN_cardError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: getCardHolderName\n");
    printf("---------------------------\n");

    // Test Case 1: Valid Name
    printf("Test Case 1: Valid Name\n");
    printf("Expected Result: Card is Ok\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == CARD_OK)
        printf("Actual Result: Card is Ok\n");
    else
        printf("Actual Result: Wrong Input\n");
    printf("---------------------------\n");

    // Test Case 2: Short Name (less than 20 characters)
    printf("Test Case 2: Short Name (less than 20 characters)\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

    // Test Case 3: No input
    printf("Test Case 3: No input\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

    // Test Case 4: Name Contains invalid characters
    printf("Test Case 4: Name Contains invalid characters\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

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
            {
                /*DO NOTHING.*/
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
            {
                /*DO NOTHING*/
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
        {
            /*DO NOTHING.*/
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
        {
            /*DO NOTHING.*/
        }
    }
}

//******************************************* getCardPAN Function **********************************************/

void pan_num_input(char *pan_buffer)
{
    printf("Enter the card's Primary Account Number (PAN): ");
    fflush(stdin);
    scanf("%20s", pan_buffer);  //Read up to 20 characters to avoid buffer overflow
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    char pan[20];                //Buffer to store the input PAN

    pan_num_input(pan);

    // Check if PAN is NULL or empty
    if (pan[0] == '\0')
    {
        return WRONG_PAN;
    }

    // Check if PAN length is within the allowed range
    char  panLength = strlen(pan);
    if (panLength < 16 || panLength > 19)
    {
        return WRONG_PAN;
    }

    // PAN is valid, copy it to cardData
    strcpy(cardData->primaryAccountNumber, pan);

    return CARD_OK;
}


//******************************************* End of getCardPAN Function **********************************************/


//******************************************* getCardPAN Test Function **********************************************/


void getCardPANTest()
{
    // Test Case 1: Happy Case - Valid PAN (length = 16)
    printf("Test Case 1:\n");
    printf("Input Data: Valid PAN (16 digits)\n");
    printf("Expected Result: CARD_OK\n");
    ST_cardData_t cardData1;
    EN_cardError_t result1 = getCardPAN(&cardData1);
    printf("Actual Result: %s\n", result1 == CARD_OK ? "CARD_OK" : "WRONG_PAN");
    printf("\n");

    // Test Case 2: Invalid PAN (length < 16)
    printf("Test Case 2:\n");
    printf("Input Data: Invalid PAN (less than 16 digits)\n");
    printf("Expected Result: WRONG_PAN\n");
    ST_cardData_t cardData2;
    EN_cardError_t result2 = getCardPAN(&cardData2);
    printf("Actual Result: %s\n", result2 == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");

// Test Case 3: Invalid PAN (characters instead of numbers)
    printf("Test Case 3:\n");
    printf("Input Data: PAN with characters (not numbers)\n");
    printf("Expected Result: WRONG_PAN\n");
    ST_cardData_t cardData3;
    // Simulate entering characters instead of numbers
    strcpy(cardData3.primaryAccountNumber, "ABCDE12345");
    EN_cardError_t result3 = getCardPAN(&cardData3);
    if(result3 == CARD_OK)
    {
        printf("Actual Result: CARD_OK \n\n");

    }
    else
    {
        printf("Actual Result: WRONG_PAN \n\n");
    }
    printf("\n");


}

//******************************************* End ofgetCardPAN Test Function **********************************************/