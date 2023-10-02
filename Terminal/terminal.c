#include "terminal.h"
#include "stdlib.h"
ST_terminalData_t termTest =
{
    500.0,
    1000.0,
    "28/09/2023"
};


//_________________| implementation of getTransactionDate |_____________________________________
static uint8_t transactionDate[11]= {'\0'};                             //string to hold transactionDate
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    EN_terminalError_t errorStatus = TERMINAL_OK;
    char transactionDate[11] = {'\0'}; // string to hold transaction date
    char transactionDay[3] = {'\0'}; // string to hold transaction day
    char transactionMonth[3] = {'\0'}; // string to hold transaction month
    char transactionYear[5] = {'\0'}; // string to hold transaction year
    uint8_t transactionDayValue; // variable to hold transaction day as decimal
    uint8_t transactionMonthValue; // variable to hold transaction month as decimal
    uint16_t transactionYearValue; // variable to hold transaction year as decimal

    printf("Enter your Transaction Date please (DD/MM/YYYY): ");
    scanf("%10s", transactionDate); // store user input

    if (strlen(transactionDate) != 10 || transactionDate[2] != '/' || transactionDate[5] != '/')  //  two digits days/two digits month/four digits year
    {
        errorStatus = WRONG_DATE;
    }
    else
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            // check if user input transaction date has a valid format or not
            if (i != 2 && i != 5 && !isdigit(transactionDate[i]))
            {
                errorStatus = WRONG_DATE; // update error status variable
                break;
            }
            else
            {
                if (i == 0 || i == 1)
                {
                    transactionDay[i] = transactionDate[i]; // hold transaction day in string array
                }
                else if (i == 3 || i == 4)
                {
                    transactionMonth[i - 3] = transactionDate[i]; // hold transaction month in string array
                }
                else if (i >= 6)
                {
                    transactionYear[i - 6] = transactionDate[i]; // hold transaction year in string array
                }
            }
        }
        // convert and save into variables to avoid calling atoi function in if condition
        transactionDayValue = atoi(transactionDay); // convert transaction day to decimal
        transactionMonthValue = atoi(transactionMonth); // convert transaction month to decimal
        transactionYearValue = atoi(transactionYear); // convert transaction year to decimal

        // check if transaction date is in a valid range or not
        if (transactionDayValue > 31 || transactionMonthValue > 12 || transactionYearValue > 2032 || transactionYearValue < 2000)
        {
            errorStatus = WRONG_DATE; // update error status variable
        }

        if (errorStatus == TERMINAL_OK)
        {
            strcpy(termData->transactionDate, transactionDate); // store the correct transaction date
            printf("Transaction date updated!\n");
        }
    }
    return errorStatus;
}
//_________________| implementation of getTransactionDateTest |_____________________________________
void getTransactionDateTest(void)
{
    ST_terminalData_t terminalDataTest;
    //   test card module.
    printf("//__________________|getCardExpiryDate function testCases. |________________ \n");
    printf("Tester Name: Ahmed Reda\nFunction Name: getTransactionDate\n");

    for (uint8_t i = 1; i <=5; i++)
    {
        uint8_t callingResult=getTransactionDate(&terminalDataTest);                                         // get result of calling getTransactionDate
        uint8_t expectedResult=TERMINAL_OK;
        if (callingResult==TERMINAL_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,transactionDate,"TERMINAL_OK","TERMINAL_OK");
        }
        else if (callingResult==WRONG_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,transactionDate,"WRONG_DATE","WRONG_DATE");
        }
        else
        {/*DO NOTHING.*/}

    }
}

//_________________| implementation of isCardExpired |_____________________________________
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{    uint8_t errorStatus=TERMINAL_OK;

    uint8_t cardExpiredMonth[3]={'\0'};                                       //string to hold card expired month
    uint8_t transactionMonth[3]={'\0'};                                       //string to hold transaction month
    uint8_t cardExpiredYear[3]={'\0'};                                        //string to hold card expired year
    uint8_t transactionYear[3]={'\0'};                                        //string to hold transaction year
    uint16_t cardExpiredYearValue;                                            //variable to hold card expired month as decimal
    uint8_t cardExpiredMonthValue;                                            //variable to hold card expired year as decimal
    uint16_t transactionYearValue;                                            //variable to hold transaction month as decimal
    uint8_t transactionMonthValue;                                            //variable to hold transaction year as decimal


    for (uint8_t i = 0; i < 5; i++)
    {

        if(i<2)
        {
            cardExpiredMonth[i]=cardData->cardExpirationDate[i];            //hold card expired month in string array
            transactionMonth[i]=termData->transactionDate[i+3];               //hold card expired month in string array
        }
        else if(i>2)
        {
            cardExpiredYear[i-3]=cardData->cardExpirationDate[i];                        //hold card expired year in string array
            transactionYear[i-3]=termData->transactionDate[i+5];                        //hold card expired year in string array
        }
        else
        {  /*DO NOTHING*/}
    }

        // convert and save into variable to avoid calling atoi function in if condition.
    cardExpiredMonthValue=atoi(cardExpiredMonth);                             //convert card expired month to decimal
    cardExpiredYearValue=(atoi(cardExpiredYear)+2000);                        //convert card expired year to decimal
    transactionMonthValue=atoi(transactionMonth);                             //convert  transactionMonth to decimal
    transactionYearValue=(atoi(transactionYear)+2000);                        //convert ctransactionYear to decimal

    // check if card expiration year is expired or not.
    if((transactionYearValue>cardExpiredYearValue))
    {
        errorStatus=EXPIRED_CARD;                                     //update status error variable
    }
    else if (transactionYearValue==cardExpiredYearValue)
    {
        //check if card expiration year is current ,check month is expired or not.
        if (transactionMonthValue>cardExpiredMonthValue)
        {
            errorStatus=EXPIRED_CARD;                                 //update status error variable
        }
    }
    else
    {/*DONOTHING*/}
    return errorStatus;
}
//_________________| implementation of isCardExpriedTest |_____________________________________

void isCardExpriedTest(void)
{

    uint8_t cardDataTestCases[5][6] = {"10/22", "12/23", "12/25", "05/20", "10/19"};                             //Test cases values
    uint8_t TerminalTestCases[5][11] = {"05/11/2022", "09/12/2022", "23/12/2020", "15/06/2020", "23/12/2019"};                           //  Test cases values
    uint8_t TestCasesResult[5] = {EXPIRED_CARD, TERMINAL_OK, TERMINAL_OK, EXPIRED_CARD, EXPIRED_CARD};           //Test cases expected result

    ST_cardData_t cardDataTest;
    ST_terminalData_t cardTerminalTest; //   test card module.
    printf("//__________________|isCardExpried function testCases. |________________ \n");
    printf("Tester Name: Ahmed Reda\nFunction Name: isCardExpried\n");

    for (uint8_t i = 0; i < 5; i++)
    {
        strcpy(cardDataTest.cardExpirationDate, cardDataTestCases[i]);                     //update test card module  with test case
        strcpy(cardTerminalTest.transactionDate, TerminalTestCases[i]);                   //update test card module  with test case

        uint8_t callingResult = isCardExpired(&cardDataTest, &cardTerminalTest);          //get result of calling getCardExpiryDate
        if (TestCasesResult[i] == TERMINAL_OK && callingResult == TERMINAL_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n", i, cardDataTestCases[i], "TERMINAL_OK", "TERMINAL_OK");
        }
        else if (TestCasesResult[i] == EXPIRED_CARD && callingResult == TERMINAL_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n", i, cardDataTestCases[i], "EXPIRED_CARD", "TERMINAL_OK");
        }
        else if (TestCasesResult[i] == EXPIRED_CARD && callingResult == EXPIRED_CARD)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n", i, cardDataTestCases[i], "EXPIRED_CARD", "EXPIRED_CARD");
        }
        else if (TestCasesResult[i] == TERMINAL_OK && callingResult == EXPIRED_CARD)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n", i, cardDataTestCases[i], "TERMINAL_OK", "EXPIRED_CARD");
        }
        else
        { /*DO NOTHING.*/
        }
    }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{

    float enteredAmount;                            //Variable to store the user's input

    printf("Enter Transaction Amount: \n");         //Prompt the user to enter the transaction amount
    fflush(stdin);
    scanf("%f", &enteredAmount);                    //Read the user's input and store it in enteredAmount

    if (enteredAmount <= 0.0)
    {
        //if the entered amount is less than or equal to zero Return Invalid Error
        return INVALID_AMOUNT;                       //Return the error code for invalid amount
    }
    else
    {
        //if the entered amount is valid
        termData->transAmount = enteredAmount;          //Store the valid amount
        return TERMINAL_OK;                             //Return the success code
    }

}
void getTransactionAmountTest(void)
{
    ST_terminalData_t termData;
    EN_terminalError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: getTransactionAmount\n");
    printf("---------------------------\n");

    //Test Case 1: check if the entered amount is Valid (more than 0.0)

    printf("Test Case 1: Valid Amount\n");
    printf("Expected Result: Amount is Valid\n");
    errorResult = getTransactionAmount(&termData);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    //Test Case 2: if the entered amount equals 0.0
    printf("Test Case 2: invalid Amount (equals 0.0)\n");
    printf("Expected Result: Amount is inValid\n");
    errorResult = getTransactionAmount(&termData);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    //Test Case 3: if the entered amount less 0.0
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
    uint8_t errorStatus=TERMINAL_OK;

    if((int)(termData->transAmount )> (int)(termData->maxTransAmount))      //Check if the entered amount is below max amount
       {
        errorStatus= EXCEED_MAX_AMOUNT;                                     //If it more than the max amount return Error Status
       }
                                                                            //else if it is below the max amount return the success code
    return errorStatus;
}
//Function to test isBelowMaxAmount function

void isBelowMaxAmountTest(void)
{
    EN_terminalError_t errorResult; // Variable to store the error result

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: isBelowMaxAmount\n");
    printf("Max amount is set here for testing = 1000.0\n");
    printf("---------------------------\n");

    //Test Case 1: check if the entered amount is Valid (Means is below the max amount)
    printf("Test Case 1: Valid Amount (below the max amount)\n");
    printf("Expected Result: Amount is Valid\n");
    errorResult = isBelowMaxAmount(&termTest);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    //Test Case 2: if the entered amount equals the max amount

    printf("Test Case 2: Valid Amount (equals the max amount)\n");
    printf("Expected Result: Amount is Valid\n");
    errorResult = isBelowMaxAmount(&termTest);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    //Test Case 3: if the entered amount exceeds the max amount

    printf("Test Case 3: invalid Amount (exceeds the max amount)\n");
    printf("Expected Result: Invalid Amount\n");
    errorResult = isBelowMaxAmount(&termTest);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");
}

// Function to set the maximum transaction amount
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    if (maxAmount <= 0.0)
        return INVALID_MAX_AMOUNT;                         //Return error if the max amount is invalid
    else
    {
        termData->maxTransAmount = maxAmount;            //Set the max transaction amount
        return TERMINAL_OK;                             //Return the operation is Successful
    }
}


//Function to test setMaxAmount
void setMaxAmountTest(void)
{
    float enteredMaxAmount;                 //Variable to store the entered max amount
    ST_terminalData_t termData;             //Structure to hold terminal data
    EN_terminalError_t errorResult;         //Variable to store the error result

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: setMaxAmount\n");
    printf("---------------------------\n");

    //Test Case 1: check if the entered amount is Valid (more than 0.0)
    printf("Test Case 1: Valid Amount (more than 0.0)\n");
    printf("Enter Max Amount: \n");                         //Prompt the user to enter the max amount
    scanf("%f", &enteredMaxAmount);                         //Read the user's input and store it in enteredMaxAmount
    printf("Expected Result: maxAmount is Valid\n");
    errorResult = setMaxAmount(&termData, enteredMaxAmount);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: maxAmount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 2: if the entered max amount equals 0.0
    printf("Test Case 2: Valid Amount (equals 0.0)\n");
    printf("Enter Max Amount: \n");                        //Prompt the user to enter the max amount
    scanf("%f", &enteredMaxAmount);                        //Read the user's input and store it in enteredMaxAmount
    printf("Expected Result: maxAmount is Invalid\n");
    errorResult = setMaxAmount(&termData, enteredMaxAmount);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: maxAmount is Invalid\n");
    printf("---------------------------\n");

    //Test Case 3: if the entered max amount is less than 0.0
    printf("Test Case 3: Invalid Amount (less than 0.0)\n");
    printf("Enter Max Amount: \n");                     //Prompt the user to enter the max amount
    scanf("%f", &enteredMaxAmount);                     //Read the user's input and store it in enteredMaxAmount
    printf("Expected Result: maxAmount is Invalid\n");
    errorResult = setMaxAmount(&termData, enteredMaxAmount);
    if (errorResult == TERMINAL_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: maxAmount is Invalid\n");
    printf("---------------------------\n");
}
/******************************************* Start isValidCardPAN Function **********************************************/

//Function to convert a character to its  integer value
int Char_to_int(char c)
{
    return (c - '0');
}

//Function to check if the provided card primary account number (PAN) is valid using the Luhn algorithm

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{

    char *pan = (char *)cardData->primaryAccountNumber;   //in this Step we Convert primary account number to a character array
    int len = strlen(pan);                               //Get the length of the PAN
    int Check_sum = 0;                                  //Initialize the checksum
    int isSecondDigit = 0;                              //Flag to indicate if the current digit is the second digit in the PAN
    int digit = 0;                                      //Variable to store the current digit

    // Iterate through the PAN digits in reverse order
    for (int i = len - 1; i >= 0; i--)
    {
        digit = Char_to_int(pan[i]);                    //Convert the current character to an integer

        if (isSecondDigit)
        {
            digit *= 2;                                 // Double every second digit
            if (digit > 9)
                digit -= 9;                             //If doubling results in a two-digit number, subtract 9
        }

        Check_sum += digit;                             // Add the digit to the checksum
        isSecondDigit = !isSecondDigit;                 //Toggle the flag for the next iteration
    }

    //Check if the total checksum is divisible by 10 (Luhn algorithm validation)
    if (Check_sum % 10 == 0)
    {
        return TERMINAL_OK;                              //PAN is valid according to the Luhn algorithm
    }
    else
    {
        return INVALID_CARD;                            //PAN is NOT VALID according to the Luhn algorithm
    }
}

/******************************************* End of isValidCardPAN Function **********************************************/



/******************************************* Start of isValidCardPAN Test Function **********************************************/

//Function to test isValidCardPAN function
void isValidCardPANTest(void)
{
    printf("Tester Name: [Amr WAHBA]\n");
    printf("Function Name: isValidCardPAN\n");

    // Test Case 1: Valid PAN
    printf("Test Case 1:\n");
    printf("Input Data: [User input]\n");
    printf("Expected Result: TERMINAL_OK\n");
    ST_cardData_t cardData1;                                            //Structure to hold card data
    EN_cardError_t panResult1 = getCardPAN(&cardData1);                 //Get the card PAN from user input
    if (panResult1 == CARD_OK)
    {
        EN_terminalError_t result1 = isValidCardPAN(&cardData1);        //Check if the PAN is valid
        if (result1 == TERMINAL_OK)
            printf("Actual Result: TERMINAL_OK\n\n");                   //Print the actual result based on validation
        else
            printf("Actual Result: INVALID_CARD\n\n");                  //Print the actual result based on validation
    }
    else
    {
        printf("Actual Result: Invalid PAN input. Please try again.\n\n");
    }

    // Test Case 2: Invalid PAN
    printf("Test Case 2:\n");
    printf("Input Data: [User input]\n");
    printf("Expected Result: INVALID_CARD\n");
    ST_cardData_t cardData2;                                        //Structure to hold card data
    EN_cardError_t panResult2 = getCardPAN(&cardData2);             //Get the card PAN from user input
    if (panResult2 == CARD_OK)
    {
        EN_terminalError_t result2 = isValidCardPAN(&cardData2);    //Check if the PAN is valid
        if (result2 == TERMINAL_OK)
            printf("Actual Result: TERMINAL_OK\n\n");               //Print the actual result based on validation
        else
            printf("Actual Result: INVALID_CARD\n\n");              //Print the actual result based on validation
    }
    else
    {
        printf("Actual Result: Invalid PAN input. Please try again.\n\n");
    }
}

/******************************************* End of isValidCardPAN Test Function **********************************************/

