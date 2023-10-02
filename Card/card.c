
#include <stdio.h>                          //to use atoi function to covert string to int.
#include "card.h"
#include <time.h>                         // to get OS date
#include <stdlib.h>                      // to use atoi function to covert string to int.

static uint8_t cardExpiredDate[6];

EN_cardError_t isNameValid(uint8_t *name)
{
    EN_cardError_t errorStatus=CARD_OK;
    // Check if the entered characters is valid
    for (uint8_t i = 0; name[i]; i++)
    {
        if (!((name[i]>='a'|| name[i]<='z')||(name[i]>='A'|| name[i]<='Z')||(name[i]!=' ')))
        {
            printf("-%c",name[i]);
            errorStatus=WRONG_NAME;
            break;
        }
    }
    return errorStatus;
}

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    char holderName[MAX_NAME_LENGTH + 1];                       //Buffer to hold the card holder's name
    uint8_t counter = 0;                                        //Counter variable for iteration

    printf("Please Enter Your Name : \n");                      //Prompt the user to enter their name
    fgets(holderName, sizeof(holderName), stdin);               //Read the user's input and store it in holderName

    if (!isNameValid(holderName))
    {
        return WRONG_NAME;                                      //Name contains invalid characters
    }

    //Check various conditions to determine the validity of the name
    if ((holderName[0] == '\0') || (strlen(holderName) < MIN_NAME_LENGTH) || (strlen(holderName) > MAX_NAME_LENGTH))
    {
        return WRONG_NAME; // Return the error code for wrong name
    }
    else
    {
        strcpy(cardData->cardHolderName,holderName);            //Copy the valid name to the cardData structure
        return CARD_OK;                                         //Return the success code
    }

}

void getCardHolderNameTest(void)
{
    ST_cardData_t cardData;
    EN_cardError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: getCardHolderName\n");
    printf("---------------------------\n");

    //Test Case 1: Valid Name
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

    //Test Case 3: No input
    printf("Test Case 3: No input\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

    //Test Case 4: Name Contains invalid characters
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

//Static array to hold card expiration date
static uint8_t cardExpiredDate[6];

//Function to get the card expiration date
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t errorStatus = CARD_OK;                       //Return variable for error status

    uint8_t cardExpiredMonth[3] = {'\0', '\0', '\0'};           //String to hold card expiration month
    uint8_t cardExpiredYear[3] = {'\0', '\0', '\0'};            //String to hold card expiration year
    uint16_t cardExpiredYearInt;                                //Variable to hold card expiration month as int type
    uint8_t cardExpiredMonthInt;                                //Variable to hold card expiration year as int type

    uint8_t cardExpiredDate[6];                                 //Buffer to hold the user input for card expiration date
    printf("Enter your card expiration date (MM/YY): ");
    scanf(" %s[^\n] ", &cardExpiredDate);

    if (strlen(cardExpiredDate) != 5)
    {
        errorStatus = WRONG_EXP_DATE;                               //Update error status if input length is not as expected
    }
    else
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            // Check if the characters in the expiration date have valid format
            if (!(((cardExpiredDate[i] <= '9' && cardExpiredDate[i] >= '0')) || (cardExpiredDate[i] == '/')))
            {
                errorStatus = WRONG_EXP_DATE;                    //Update status error variable
                break;
            }
            else
            {
                /*DO NOTHING.*/
            }

            if (i < 2)
            {
                cardExpiredMonth[i] = cardExpiredDate[i];                //Hold card expiration month in string array
            }
            else if ((i == 2) && !(cardExpiredDate[i] == '/'))          //Check if the third character is '/'
            {
                errorStatus = WRONG_EXP_DATE;                           //Update status error variable
                break;
            }
            else if (i > 2)
            {
                cardExpiredYear[i - 3] = cardExpiredDate[i];        //Hold card expiration year in string array
            }
            else
            {
                /*DO NOTHING*/
            }
        }

        //Convert and save into variable to avoid calling atoi function in if condition
        cardExpiredMonthInt = atoi(cardExpiredMonth);                           //Convert card expiration month to int type

        //Check if card expiration month is in range or not
        if (cardExpiredMonthInt > 12)
        {
            errorStatus = WRONG_EXP_DATE;                                       //Update status error variable
        }

        if (errorStatus == CARD_OK)
        {
            strcpy(cardData->cardExpirationDate, cardExpiredDate);              //Update cardExpirationDate with user input
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

// Function to test getCardExpiryDate
void getCardExpiryDateTest(void)
{
    printf("//__________________|getCardExpiryDate function testCases. |________________ \n");
    printf("Tester Name: Ahmed Reda\nFunction Name: isCardExpired\n");

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t expectedResult = 0;
        printf("Enter the expected Result (CARD_OK -> 0 | WRONG_EXP_DATE -> 2): ");
        scanf("%d", &expectedResult);
        ST_cardData_t cardDataTest;
        uint8_t callingResult = getCardExpiryDate(&cardDataTest);                       //Get result of calling getCardExpiryDate

        if (expectedResult == CARD_OK && callingResult == CARD_OK)
        {
            printf("Test Case %d:\nInput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "CARD_OK", "CARD_OK");
        }
        else if (expectedResult == WRONG_EXP_DATE && callingResult == CARD_OK)
        {
            printf("Test Case %d:\nInput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "WRONG_EXP_DATE", "CARD_OK");
        }
        else if (expectedResult == WRONG_EXP_DATE && callingResult == WRONG_EXP_DATE)
        {
            printf("Test Case %d:\nInput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "WRONG_EXP_DATE", "WRONG_EXP_DATE");
        }
        else if (expectedResult == CARD_OK && callingResult == WRONG_EXP_DATE)
        {
            printf("Test Case %d:\nInput: %s\nExpected Result: %s\nActual Result:   %s\n", i, cardExpiredDate, "CARD_OK", "WRONG_EXP_DATE");
        }
        else
        {
            /*DO NOTHING.*/
        }
    }
}

//******************************************* getCardPAN Function **********************************************//

void pan_num_input(char *pan_buffer)
{
    printf("Enter the card's Primary Account Number (PAN): ");
    fflush(stdin);
    scanf("%20s", pan_buffer);                                    //Read up to 20 characters to avoid buffer overflow
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    char pan[20];                                             //Buffer to store the input PAN

    pan_num_input(pan);

    //Check if PAN is NULL or empty
    if (pan[0] == '\0')
    {
        return WRONG_PAN;
    }

    //Check if PAN length is within the allowed range
    char  panLength = strlen(pan);
    if (panLength < 16 || panLength > 19)
    {
        return WRONG_PAN;
    }

    //PAN is valid, copy it to cardData object
    strcpy(cardData->primaryAccountNumber, pan);

    return CARD_OK;
}


//******************************************* End of getCardPAN Function **********************************************/


//******************************************* getCardPAN Test Function **********************************************/


void getCardPANTest()
{
    //Test Case 1: Happy Case - Valid PAN (length = 16)
    printf("Test Case 1:\n");
    printf("Input Data: Valid PAN (16 digits)\n");
    printf("Expected Result: CARD_OK\n");
    ST_cardData_t cardData1;
    EN_cardError_t result1 = getCardPAN(&cardData1);
    printf("Actual Result: %s\n", result1 == CARD_OK ? "CARD_OK" : "WRONG_PAN");
    printf("\n");

    //Test Case 2: Invalid PAN (length < 16)
    printf("Test Case 2:\n");
    printf("Input Data: Invalid PAN (less than 16 digits)\n");
    printf("Expected Result: WRONG_PAN\n");
    ST_cardData_t cardData2;
    EN_cardError_t result2 = getCardPAN(&cardData2);
    printf("Actual Result: %s\n", result2 == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");

//Test Case 3: Invalid PAN (characters instead of numbers)
    printf("Test Case 3:\n");
    printf("Input Data: PAN with characters (not numbers)\n");
    printf("Expected Result: WRONG_PAN\n");
    ST_cardData_t cardData3;
    strcpy(cardData3.primaryAccountNumber, "ABCDE12345");    //Simulate entering characters instead of numbers

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