#include"../Card/card.h"
#include"../Server/server.h"
#include"../Terminal/terminal.h"
#include <stdlib.h>
#include"app.h"
#define MAX_AMOUNT 8000

extern ST_accountsDB_t *Global_accountReference;

void appStart(void)
{
    ST_transaction_t userTransData;

    EN_terminalError_t errorTerminalStatus=TERMINAL_OK;
    EN_serverError_t errorServerStatus=SERVER_OK;
    setMaxAmount(&userTransData.terminalData,MAX_AMOUNT);

    EN_cardError_t userInputStatus=CARD_OK;
    userInputStatus=getCardHolderName(&userTransData.cardHolderData);
    switch (userInputStatus)
    {
    case CARD_OK:
        userInputStatus=getCardPAN(&userTransData.cardHolderData);

        switch (userInputStatus)
        {
        case CARD_OK:
            if(isValidCardPAN(&userTransData.cardHolderData))
            {
                userInputStatus=WRONG_PAN;
                printf("--> Operation Declined FRAUD_CARD !! <--\n");
                break;
            }
            userInputStatus=getCardExpiryDate(&userTransData.cardHolderData);
            switch (userInputStatus)
            {
            case CARD_OK:
                userInputStatus=getTransactionDate(&userTransData.terminalData);
                switch (userInputStatus)
                {
                case TERMINAL_OK:
                    break;
                case WRONG_DATE:
                    printf("--> WRONG TRANSACTION DATE (!_!) <--\n");
                    userInputStatus=WRONG_DATE;
                    break;
                default:
                    break;
                }
                break;
            case WRONG_EXP_DATE:
                printf("--> WRONG EXPIRATION DATE FORMAT (!_!) <--.\n");
                userInputStatus=WRONG_EXP_DATE;
                break;
            default:
                break;
            }
            break;
        case WRONG_PAN:
            printf("--> WRONG PAN FORAMT (!_!) <--.\n");
            userInputStatus=WRONG_PAN;
            break;
        default:
            break;
        }

        break;
    case WRONG_NAME:
        printf("--> WRONG NAME FORMAT (!_!) <--.\n");
        userInputStatus=WRONG_NAME;
        break;
    default:
        break;
    }
    if(userInputStatus==CARD_OK)
    {
        errorServerStatus = isValidAccount(&userTransData.cardHolderData, &Global_accountReference);
        switch (errorServerStatus)
        {
        case SERVER_OK:
           errorServerStatus=isBlockedAccount(Global_accountReference);
            switch (errorServerStatus)
            {
            case SERVER_OK:
                 userInputStatus=isCardExpired(&userTransData.cardHolderData,&userTransData.terminalData);
                switch (userInputStatus)
                {
                case SERVER_OK:
                   printf("your Max amount is %d\n",MAX_AMOUNT);
                    errorTerminalStatus=getTransactionAmount(&userTransData.terminalData);
                    switch (errorTerminalStatus)
                    {
                    case TERMINAL_OK:
                       errorTerminalStatus=isBelowMaxAmount(&userTransData.terminalData);
                        switch (errorTerminalStatus)
                        {
                        case TERMINAL_OK:
                            errorServerStatus=isAmountAvailable(&userTransData.terminalData,Global_accountReference);
                            switch(errorServerStatus)
                            {
                            case SERVER_OK:
                                printf("your balance before transaction is %.2f\n", Global_accountReference->balance);
                                Global_accountReference->balance -= userTransData.terminalData.transAmount;
                                printf("your balance after transaction is %.2f\n", Global_accountReference->balance);
                                saveTransaction(&userTransData);
                                break;
                            case LOW_BALANCE:
                                printf("--> LOW SUFFCIENT AMOUNT(!_!) <--\n ");
                                break;
                            default:
                                break;
                            }
                            break;
                        case EXCEED_MAX_AMOUNT:
                            printf("--> EXCEED MAXIMUM AMOUNT (!_!) <--");
                            break;
                        default:
                            break;
                        }
                        break;
                    case INVALID_AMOUNT:
                        printf(" --> INVALID AMOUNT FORMAT (!_!)\n");
                        break;
                    default:
                        break;
                    }
                    break;
                case EXPIRED_CARD:
                    printf(" --> EXPIRED CARD GET BACK TO BANK  (!_!) \n");
                    break;
                default:
                    break;
                }
                break;
            case BLOCKED_ACCOUNT:

                printf("-->  STOLEN CARD (!_!) <--\n");
                break;
            default:
                break;
            }
            break;
        case ACCOUNT_NOT_FOUND:
            printf("--> FRAUD CARD (!_!) \n");
            break;

        default:
            break;
        }
    }
    else
    {
        /*DO NOTHING.*/
    }

}
