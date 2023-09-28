#include "terminal.h"

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