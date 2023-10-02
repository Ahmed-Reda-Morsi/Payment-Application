#include"../Card/card.h"
#include"../Server/server.h"
#include"../Terminal/terminal.h"
#include"app.h"
#define MAXIMUM_TRYS    3
#define MAX_AMOUNT 8000

extern ST_accountsDB_t *Global_accountReference;

void appStart(void)
{
    ST_cardData_t userCardData;
    ST_terminalData_t userTerminalData;

    EN_cardError_t errorCardStatus=CARD_OK;
    EN_terminalError_t errorTerminalStatus=TERMINAL_OK;
    EN_serverError_t errorServerStatus=SERVER_OK;
    uint8_t errorStatus;

   // set the maximum amount.
    setMaxAmount(&userTerminalData,MAX_AMOUNT);

    uint8_t trys=0;
    EN_cardError_t userInputStatus=CARD_OK;
    userInputStatus=getCardHolderName(&userCardData);
    switch (userInputStatus)
    {
    case CARD_OK:
        userInputStatus=getCardPAN(&userCardData);

        switch (userInputStatus)
        {
        case CARD_OK:
            userInputStatus=getCardExpiryDate(&userCardData);
            switch (userInputStatus)
            {
            case CARD_OK:
                userInputStatus=getTransactionDate(&userTerminalData);
                switch (userInputStatus)
                {
                case TERMINAL_OK:
                    trys=3;
                    break;
                case WRONG_DATE:
                    printf("your transation date is wrong please try again !!\n");
                    userInputStatus=WRONG_DATE;
                    break;
                default:
                    break;
                }
                break;
            case WRONG_EXP_DATE:
                printf("your expiry date is wrong please again.\n");
                userInputStatus=WRONG_EXP_DATE;
                break;
            default:
                break;
            }
            break;
        case WRONG_PAN:
            printf("your card PAN is wrong please again.\n");
            userInputStatus=WRONG_PAN;
            break;
        default:
            break;
        }

        break;
    case WRONG_NAME:
        printf("your card name is wrong please again.\n");
        userInputStatus=WRONG_NAME;
        break;
    default:
        break;
    }

    errorServerStatus = isValidAccount(&userCardData, &Global_accountReference);
    switch (errorServerStatus)
    {
    case SERVER_OK:
        printf("valid ok\n");
        printf(" state:%d | PAN: %s\n",Global_accountReference->state,Global_accountReference->primaryAccountNumber);
        errorServerStatus=isBlockedAccount(Global_accountReference);
        switch (errorServerStatus)
        {
        case SERVER_OK:
            printf("not blocked ok\n");
            errorCardStatus=isCardExpired(&userCardData,&userTerminalData);
            switch (errorCardStatus)
            {
            case SERVER_OK:
                printf("not expired ok\n");

                printf("your Max amount is %d\n",MAX_AMOUNT);
                errorTerminalStatus=getTransactionAmount(&userTerminalData);
                switch (errorTerminalStatus)
                {
                case TERMINAL_OK:
                    printf("amount valid ok\n");

                    errorTerminalStatus=isBelowMaxAmount(&userTerminalData);
                    switch (errorTerminalStatus)
                    {
                    case TERMINAL_OK:
                        printf("amount is below  max ok\n");
                        printf("your balance before transaction is %.2f\n", Global_accountReference->balance);
                        Global_accountReference->balance -= userTerminalData.transAmount;
                        printf("your balance after transaction is %.2f\n", Global_accountReference->balance);
                        break;
                    case EXCEED_MAX_AMOUNT:
                        printf("the entered value is greater than the maximum amount!!");
                        break;
                    default:
                        break;
                    }
                    break;
                case INVALID_AMOUNT:
                    printf("your amount is not valid !!\n");
                    break;
                default:
                    break;
                }
                break;
            case WRONG_EXP_DATE:
                printf("your card date is expired please get back to your bank. !!\n");
                break;
            default:
                break;
            }
            break;
        case BLOCKED_ACCOUNT:
            printf("your account info is blocked. !!\n");
            break;
        default:
            break;
        }
        break;
    case ACCOUNT_NOT_FOUND:
        printf("your account info is not valid. !!\n");
        break;

    default:
        break;
    }







}
