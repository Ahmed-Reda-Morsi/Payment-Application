
#include <time.h>
#include"Card/card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t errorStatus=CARD_OK ;
    uint8_t cardExpiredMonth[3]={'\0','\0','\0'};
    uint8_t cardExpiredYear[3]={'\0','\0','\0'};
    uint8_t cardExpiredMonthInt;
    uint16_t cardExpiredYearInt;
    // Get the current time
    time_t currentTime;
    time(&currentTime);

    // Convert the current time to a struct tm
    struct tm* localTime = localtime(&currentTime);

    // Extract the month and year 
    uint16_t currentMonth = localTime->tm_mon + 1;  // tm_mon is zero-based, so add 1
    uint16_t currentYear = localTime->tm_year + 1900;  // tm_year is years since 1900;
    for (uint8_t i = 0; i < 5; i++)
    {
        //check of expired date is valid or not 
        if(!(((cardData->cardExpirationDate[i]<='9'&&cardData->cardExpirationDate[i]>='0'))||(cardData->cardExpirationDate[2]=='/')))
        {
            errorStatus=WRONG_EXP_DATE;
            break;
        }
        else
        {
            //DONOTHING.
        }

        if(i<2)
        {
            cardExpiredMonth[i]=cardData->cardExpirationDate[i];

        }
        else if((i==2)&&!(cardData->cardExpirationDate[i]=='/'))
        {
            errorStatus=WRONG_EXP_DATE;
            break;
        }
        else if(i>2)
        {
            cardExpiredYear[i-3]=cardData->cardExpirationDate[i];

        }
        else
        {
            //DONOTHING.
        }
    }


    cardExpiredMonthInt=atoi(cardExpiredMonth);
    cardExpiredYearInt=(atoi(cardExpiredYear) +2000);

    if ( errorStatus==CARD_OK)
    {    
         if((currentYear>cardExpiredYearInt)||(cardExpiredMonthInt>12))
        {
            errorStatus=WRONG_EXP_DATE;
        }
        else if (currentYear==cardExpiredYearInt)
         {
           if ((currentMonth>cardExpiredMonthInt))
           {
                errorStatus=WRONG_EXP_DATE;
           }
         }
         else
         {
           /*DO NOTHING.*/
         }

         
    }

 return errorStatus;
}


void getCardExpiryDate_testCases (void)
{
    uint8_t testCase[5][6]={"50/78","04/25","12/23","95s78","14942"};
    uint8_t testCaseResult[5]={WRONG_EXP_DATE,CARD_OK,CARD_OK,WRONG_EXP_DATE,WRONG_EXP_DATE};
    ST_cardData_t cardData_test;
    printf("_________| test cases for  getCardExpiryDate function |____________ \n");

    for (uint8_t i = 0; i < 5; i++)
    {
        strcpy(cardData_test.cardExpirationDate,testCase[i]);
        printf("_________| 0%d_test case:  -> intput: %s | result  ",i,cardData_test.cardExpirationDate);
        uint8_t callingResult=getCardExpiryDate(&cardData_test);

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

int main() {

    //ST_cardData_t card1={"","","12/22"};

    getCardExpiryDate_testCases();
    return 0;
 }