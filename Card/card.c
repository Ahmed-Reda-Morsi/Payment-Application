#include "card.h" 
#include <time.h>                                                           // to get OS date 
#include <stdlib.h>                                                         // to use atoi function to covert string to int.

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{

}
//____________| implementation of getCardExpiryDate function   |_________________________
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t errorStatus=CARD_OK ;                                    // return variable for error status 
    uint8_t cardExpiredMonth[3]={'\0','\0','\0'};                           // string to hold card expired month
    uint8_t cardExpiredYear[3]={'\0','\0','\0'};                            // string to hold card expired year
    uint8_t cardExpiredMonthInt;                                            // variable to hold card expired year as int type 
    uint16_t cardExpiredYearInt;                                             // variable to hold card expired month as int type
    
    time_t currentTime;                                                     // Get the current time
    time(&currentTime);
    struct tm* localTime = localtime(&currentTime);                         // Convert the current time to a struct tm

            // Extract the month and year 
    int currentMonth = localTime->tm_mon + 1;                               // tm_mon is zero-based, so add 1
    int currentYear = localTime->tm_year + 1900;                            // tm_year is years since 1900

    for (uint8_t i = 0; i < 5; i++)
    {
            //check of expired date is valid or not 
        if(!(((cardData->cardExpirationDate[i]<='9'&&cardData->cardExpirationDate[i]>='0'))||(cardData->cardExpirationDate[2]=='/')))
        {
            errorStatus=WRONG_EXP_DATE;                                     // update status error variable
            break;
        }
        else
        {   /*DO NOTHING.*/ }

        if(i<2)
        {
            cardExpiredMonth[i]=cardData->cardExpirationDate[i];            // hold card expired month in string array 
        }
        else if((i==2)&&!(cardData->cardExpirationDate[i]=='/'))            // check if third character is '/' or not
        {
            errorStatus=WRONG_EXP_DATE;                                     // update status error variable
            break;
        }
        else if(i>2)
        {
            cardExpiredYear[i-3]=cardData->cardExpirationDate[i];           // hold card expired year in string array 
        }
        else
        {  /*DO NOTHING*/}
    }
        // convert and save into variable to avoid calling atoi function in if condition.
    cardExpiredMonthInt=atoi(cardExpiredMonth);                             // convert card expired month to int type
    cardExpiredYearInt=(atoi(cardExpiredYear)+2000);                        // convert card expired year to int type

    if ( errorStatus==CARD_OK)
    {   
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

         
    }

 return errorStatus;
}

//____________| implementation of getCardExpiryDate testing function  |_________________________
void getCardExpiryDate_testCases (void)
{
    uint8_t testCase[5][6]={"50/78","04/25","12/23","95s78","14942"};                               //  Test cases values
    uint8_t testCaseResult[5]={WRONG_EXP_DATE,CARD_OK,CARD_OK,WRONG_EXP_DATE,WRONG_EXP_DATE};       //  Test cases expected result
    ST_cardData_t cardData_test;                                                                    //   test card module.
    printf("_________| test cases for  getCardExpiryDate function |____________ \n");

    for (uint8_t i = 0; i < 5; i++)
    {
        strcpy(cardData_test.cardExpirationDate,testCase[i]);                                       // update test card module  with test case 
        printf("_________| 0%d_test case:  -> intput: %s | result  ",i,cardData_test.cardExpirationDate);
        uint8_t callingResult=getCardExpiryDate(&cardData_test);                                    // get result of calling getCardExpiryDate

        if (callingResult==testCaseResult[i])
        {
            printf(" -> Passed !\n");
        }
        else 
        {
           printf(" -> Failed !\n");
        }
        
        
    }
    
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
  
}