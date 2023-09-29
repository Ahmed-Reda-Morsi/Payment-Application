#include "terminal.h"




//_________________| implementation of getTransactionDate |_____________________________________
static uint8_t transactionDate[11]={'\0'};                      // string to hold transactionDate
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    uint8_t errorStatus=TERMINAL_OK;                                    
    uint8_t transactionDay[3]={'\0' };                                  // string to hold transaction day
    uint8_t transactionMonth[3]={'\0'};                                 // string to hold transaction month
    uint8_t transactionYear[5]={'\0'};                                  // string to hold transaction year
    uint8_t transactionDayValue;                                        // variable to hold transaction day as decimal
    uint8_t transactionMonthValue;                                      // variable to hold transaction month as decimal
    uint16_t transactionYearValue;                                      // variable to hold transaction year as decimal

    printf("enter you Transaction Date please (DD/MM/YYYY):");
    scanf(" %s[^\n] ", &transactionDate);                               //store user input 

    if (strlen(transactionDate)!=10)
    {
        errorStatus=WRONG_DATE;
    }
    else
    {
        for (uint8_t i = 0; i < 10; i++)
        {       
                // check of user  input tranaction date has valid format or not. 
            if (!(((transactionDate[i] <= '9' && transactionDate[i] >= '0')) || (transactionDate[2] == '/')|| (transactionDate[5] == '/')))
            {
                errorStatus = WRONG_DATE; // update status error variable
                break;
            }
            else
            {
                if (i==0 || i == 1)
                {
                    transactionDay[i] = transactionDate[i];                             // hold transaction day in string array
                }
                else if (i == 3 || i == 4)
                {
                    transactionMonth[i - 3] = transactionDate[i];                       // hold transaction month in string array
                }
                else if (i >= 6)
                {
                    transactionYear[i - 6] = transactionDate[i];                        // hold transaction year in string array
                }
                else
                {
                    /*DO NOTHING.*/
                }   
            }
        }
            // convert and save into variable to avoid calling atoi function in if condition.
        transactionDayValue=atoi(transactionDay);                           // convert transaction day to decimal
        transactionMonthValue=atoi(transactionMonth);                       // convert ctransaction month to decimal
        transactionYearValue=atoi(transactionYear);                         // convert transaction year to decimal
        
             // check if  transaction date is valid range or not
        if (transactionDayValue>31 ||transactionMonthValue>12||transactionYearValue>2025||transactionYearValue<2000)
        {
            errorStatus=WRONG_DATE;                                     // update status error variable
        }
        else
        {
            /*DO NOTHING.*/
        }

        if (errorStatus==TERMINAL_OK)
        {
            strcpy(termData->transactionDate,transactionDate);           // store the correct transaction date.
            printf(" transaction date updated !");
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

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t callingResult=getTransactionDate(&terminalDataTest);                                         // get result of calling getTransactionDate
        uint8_t expectedResult=0;
        printf("enter the expected Result Please (TERMINAL_OK -> 0 | WRONG_DATE -> 1): ");
        scanf("%d", &expectedResult);                                                                       //store user expected result.                             
        if (expectedResult==TERMINAL_OK && callingResult==TERMINAL_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,transactionDate,"TERMINAL_OK","TERMINAL_OK");
        }
        else if (expectedResult==WRONG_DATE && callingResult==TERMINAL_OK)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,transactionDate,"WRONG_DATE","TERMINAL_OK");
        }
        else if (expectedResult==WRONG_DATE && callingResult==WRONG_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,transactionDate,"WRONG_DATE","WRONG_DATE");
        }
        else if (expectedResult==TERMINAL_OK && callingResult==WRONG_DATE)
        {
            printf("Test Case %d:\nIntput: %s\nExpected Result:%s\nActual Result: %s\n",i,transactionDate,"TERMINAL_OK","WRONG_DATE");
        }
        else
        {/*DO NOTHING.*/}
        
    }
}


//_________________| implementation of isCardExpired |_____________________________________
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    uint8_t errorStatus=TERMINAL_OK;

    uint8_t cardExpiredMonth[3]={'\0'};                                     // string to hold card expired month
    uint8_t transactionMonth[3]={'\0'};                                     // string to hold transaction month
    uint8_t cardExpiredYear[3]={'\0'};                                      // string to hold card expired year
    uint8_t transactionYear[3]={'\0'};                                      // string to hold transaction year
    uint16_t cardExpiredYearValue;                                            // variable to hold card expired month as decimal
    uint8_t cardExpiredMonthValue;                                            // variable to hold card expired year as decimal 
    uint16_t transactionYearValue;                                            // variable to hold transaction month as decimal
    uint8_t transactionMonthValue;                                            // variable to hold transaction year as decimal 

/*     time_t currentTime;                                                     // Get the current time
    time(&currentTime);
    struct tm* localTime = localtime(&currentTime);                         // Convert the current time to a struct tm
         // Extract the month and year 
    int currentMonth = localTime->tm_mon + 1;                               // tm_mon is zero-based, so add 1
    int currentYear = localTime->tm_year + 1900;    */                         // tm_year is years since 1900

    for (uint8_t i = 0; i < 5; i++)
    {
  
        if(i<2)
        {
            cardExpiredMonth[i]=cardData->cardExpirationDate[i];            // hold card expired month in string array 
            transactionMonth[i]=termData->transactionDate[i+3];               // hold card expired month in string array 
        }
        else if(i>2)
        {
            cardExpiredYear[i-3]=cardData->cardExpirationDate[i];                        // hold card expired year in string array 
            transactionYear[i-3]=termData->transactionDate[i+5];                        // hold card expired year in string array 
        }
        else
        {  /*DO NOTHING*/}
    }

        // convert and save into variable to avoid calling atoi function in if condition.
    cardExpiredMonthValue=atoi(cardExpiredMonth);                             // convert card expired month to decimal
    cardExpiredYearValue=(atoi(cardExpiredYear)+2000);                        // convert card expired year to decimal
    transactionMonthValue=atoi(transactionMonth);                             // convert  transactionMonth to decimal 
    transactionYearValue=(atoi(transactionYear)+2000);                        // convert ctransactionYear to decimal

    // check if card expiration year is expired or not.
    if((transactionYearValue>cardExpiredYearValue))                                 
    {
        errorStatus=EXPIRED_CARD;                                     // update status error variable
    }
    else if (transactionYearValue==cardExpiredYearValue)                           
    {
        // check if card expiration year is current ,check month is expired or not.
        if (transactionMonthValue>cardExpiredMonthValue)
        {
            errorStatus=EXPIRED_CARD;                                 // update status error variable
        }
    }
    else
    {/*DONOTHING*/}
    return errorStatus;
}
//_________________| implementation of isCardExpriedTest |_____________________________________
void isCardExpriedTest(void)
{

    uint8_t cardDataTestCases[5][6] = {"10/22", "12/23", "12/25", "05/20", "10/19"};                           //  Test cases values
    uint8_t TerminalTestCases[5][11] = {"05/11/2022", "09/12/2022", "23/12/2020", "15/06/2020", "23/12/2019"};                           //  Test cases values
    uint8_t TestCasesResult[5] = {EXPIRED_CARD, TERMINAL_OK, TERMINAL_OK, EXPIRED_CARD, EXPIRED_CARD}; //  Test cases expected result
    
    ST_cardData_t cardDataTest;
    ST_terminalData_t cardTerminalTest; //   test card module.
    printf("//__________________|isCardExpried function testCases. |________________ \n");
    printf("Tester Name: Ahmed Reda\nFunction Name: isCardExpried\n");

    for (uint8_t i = 0; i < 5; i++)
    {
        strcpy(cardDataTest.cardExpirationDate, cardDataTestCases[i]); // update test card module  with test case
        strcpy(cardTerminalTest.transactionDate, TerminalTestCases[i]); // update test card module  with test case

        uint8_t callingResult = isCardExpired(&cardDataTest, &cardTerminalTest); // get result of calling getCardExpiryDate
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

}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{

}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{

}
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) // Optional
{
    
}