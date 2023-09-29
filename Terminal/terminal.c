#include "terminal.h"

ST_terminalData_t termTest =
{
    500.0,
    1000.0,
    "28/09/2023"
};


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{

}

//_________________| implementation of isCardExpired |_____________________________________
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    uint8_t errorStatus=CARD_OK;

    uint8_t cardExpiredMonth[3]={'\0','\0','\0'};                           // string to hold card expired month
    uint8_t cardExpiredYear[3]={'\0','\0','\0'};                            // string to hold card expired year
    uint16_t cardExpiredYearInt;                                           // variable to hold card expired month as int type
    uint8_t cardExpiredMonthInt;                                            // variable to hold card expired year as int type 

    time_t currentTime;                                                     // Get the current time
    time(&currentTime);
    struct tm* localTime = localtime(&currentTime);                         // Convert the current time to a struct tm
         // Extract the month and year 
    int currentMonth = localTime->tm_mon + 1;                               // tm_mon is zero-based, so add 1
    int currentYear = localTime->tm_year + 1900;                            // tm_year is years since 1900

    for (uint8_t i = 0; i < 5; i++)
    {
  
        if(i<2)
        {
            cardExpiredMonth[i]=cardData->cardExpirationDate[i];            // hold card expired month in string array 
        }
        else if(i>2)
        {
            cardExpiredYear[i-3]=cardData->cardExpirationDate[i];                        // hold card expired year in string array 
        }
        else
        {  /*DO NOTHING*/}
    }

        // convert and save into variable to avoid calling atoi function in if condition.
    cardExpiredMonthInt=atoi(cardExpiredMonth);                             // convert card expired month to int type
    cardExpiredYearInt=(atoi(cardExpiredYear)+2000);                        // convert card expired year to int type

    // check if card expiration year is expired or not.
    if((currentYear>cardExpiredYearInt)||(cardExpiredMonthInt>12))                                 
    {
        errorStatus=WRONG_EXP_DATE;                                     // update status error variable
    }
    else if (currentYear==cardExpiredYearInt)                           
    {
        // check if card expiration year is current ,check month is expired or not.
        if (currentMonth>cardExpiredMonthInt)
        {
            errorStatus=WRONG_EXP_DATE;                                 // update status error variable
        }
    }
    else
    {/*DONOTHING*/}
    return errorStatus;
}
//_________________| implementation of isCardExpired |_____________________________________
void isCardExpriedTest(void);
{

    uint8_t testCase[5][6]={"50/78","04/25","12/23","95s78","14942"};                               //  Test cases values
    uint8_t testCaseResult[5]={WRONG_EXP_DATE,CARD_OK,CARD_OK,WRONG_EXP_DATE,WRONG_EXP_DATE};       //  Test cases expected result
    ST_cardData_t cardData_test;                                                                    //   test card module.
    printf("Tester Name: Ahmed Reda\nFunction Name: isCardExpried\n");

    for (uint8_t i = 0; i < 5; i++)
    {
        strcpy(cardData_test.cardExpirationDate,testCase[i]);                                       // update test card module  with test case 
        uint8_t callingResult=isCardExpired(&cardData_test,NULL);                                   // get result of calling getCardExpiryDate
        if (testCaseResult[i]==CARD_OK && callingResult==CARD_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,testCase[i],"CARD_OK","CARD_OK");
        }
        else if (testCaseResult[i]==WRONG_EXP_DATE && callingResult==CARD_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,testCase[i],"WRONG_EXP_DATE","CARD_OK");
        }
        else if (testCaseResult[i]==WRONG_EXP_DATE && callingResult==WRONG_EXP_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,testCase[i],"WRONG_EXP_DATE","WRONG_EXP_DATE");
        }
        else if (testCaseResult[i]==CARD_OK && callingResult==WRONG_EXP_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,testCase[i],"CARD_OK","WRONG_EXP_DATE");
        }
        else
        {/*DO NOTHING.*/}
        
    }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    float enteredAmount; // Variable to store the user's input

    printf("Enter Transaction Amount: \n"); // Prompt the user to enter the transaction amount
    scanf("%f", &enteredAmount); // Read the user's input and store it in enteredAmount

    if (enteredAmount <= 0.0)
    {
        // If the entered amount is less than or equal to zero, input is invalid
        return INVALID_AMOUNT; // Return the error code for invalid amount
    }
    else
    {
        // If the entered amount is valid
        termData->transAmount = enteredAmount; // Store the valid amount
        return TERMINAL_OK; // Return the success code
    }

}
void getTransactionAmountTest(void)
{
    ST_terminalData_t termData;
    EN_terminalError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: getTransactionAmount\n");
    printf("---------------------------\n");

    // Test Case 1: check if the entered amount is Valid (more than 0.0)
    printf("Test Case 1: Valid Amount\n");
    printf("Expected Result: Amount is Valid\n");
    errorResult = getTransactionAmount(&termData);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 2: if the entered amount equals 0.0
    printf("Test Case 2: invalid Amount (equals 0.0)\n");
    printf("Expected Result: Amount is inValid\n");
    errorResult = getTransactionAmount(&termData);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 3: if the entered amount less 0.0
    printf("Test Case 3: invalid Amount (less than 0.0)\n");
    printf("Expected Result: Amount is inValid\n");
    errorResult = getTransactionAmount(&termData);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    float enteredAmount; // Variable to store the user's input

    printf("Enter Transaction Amount: \n"); // Prompt the user to enter the transaction amount
    scanf("%f", &enteredAmount); // Read the user's input and store it in enteredAmount

    if(enteredAmount > termData->maxTransAmount) // Check if the entered amount is below max amount
        return EXCEED_MAX_AMOUNT; // If it more than the max amount return Error Status
    else
        return TERMINAL_OK; // else if it is below the max amount return the success code

}
void isBelowMaxAmountTest(void)
{

    EN_terminalError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: isBelowMaxAmount\n");
    printf("Max amount is set here for testing = 1000.0\n");
    printf("---------------------------\n");

    // Test Case 1: check if the entered amount is Valid (below the max amount)
    printf("Test Case 1: Valid Amount (below the max amount)\n");
    printf("Expected Result: Amount is Valid\n");
    errorResult = isBelowMaxAmount(&termTest);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 2: if the entered amount equals the max amount
    printf("Test Case 2: Valid Amount (equals the max amount)\n");
    printf("Expected Result: Amount is Valid\n");
    errorResult = isBelowMaxAmount(&termTest);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 3: if the entered amount exceeds the max amount
    printf("Test Case 3: invalid Amount (exceeds the max amount)\n");
    printf("Expected Result: Invalid Amount\n");
    errorResult = isBelowMaxAmount(&termTest);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    if(maxAmount <= 0.0)
        return INVALID_MAX_AMOUNT;
    else
    {
        termData->maxTransAmount = maxAmount;
        return TERMINAL_OK;
    }

}
void setMaxAmountTest(void)
{
     float enteredMaxAmount;
     ST_terminalData_t termData;
     EN_terminalError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: setMaxAmount\n");
    printf("---------------------------\n");

    // Test Case 1: check if the entered amount is Valid (more than 0.0)
    printf("Test Case 1: Valid Amount (more than 0.0)\n");
    printf("Enter Max Amount: \n"); // Prompt the user to enter the max amount
    scanf("%f", &enteredMaxAmount); // Read the user's input and store it in enteredMaxAmount
    printf("Expected Result: maxAmount is Valid\n");
    errorResult = setMaxAmount(&termData,enteredMaxAmount);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: maxAmount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 2: if the entered max amount equals 0.0
    printf("Test Case 2: Valid Amount (equals 0.0)\n");
    printf("Enter Max Amount: \n"); // Prompt the user to enter the max amount
    scanf("%f", &enteredMaxAmount); // Read the user's input and store it in enteredMaxAmount
    printf("Expected Result: maxAmount is InValid\n");
    errorResult = setMaxAmount(&termData,enteredMaxAmount);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: maxAmount is InValid\n");
    printf("---------------------------\n");

    // Test Case 3: if the entered max amount is less than 0.0
    printf("Test Case 3: invalid Amount (less than 0.0)\n");
    printf("Enter Max Amount: \n"); // Prompt the user to enter the max amount
    scanf("%f", &enteredMaxAmount); // Read the user's input and store it in enteredMaxAmount
    printf("Expected Result: maxAmount is InValid\n");
    errorResult = setMaxAmount(&termData,enteredMaxAmount);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: maxAmount is InValid\n");
    printf("---------------------------\n");
}
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) // Optional
{

}
