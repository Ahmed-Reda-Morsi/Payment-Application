#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{

}
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{

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
/******************************************* Start isValidCardPAN Function **********************************************/
int Char_to_int(char c)
{
    return c - '0';
}


EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    // Implement the Luhn algorithm using the provided terminal data

    char *pan = (char *)cardData->primaryAccountNumber;
    int len = strlen(pan);
    int Check_sum = 0;
    int isSecondDigit = 0;
    int digit=0;

    for (int i = len - 1; i >= 0; i--)
    {
        digit = Char_to_int(pan[i]);

        if (isSecondDigit)
        {
            digit *= 2;
            if (digit > 9)
                digit -= 9;
        }

        Check_sum += digit;
        isSecondDigit = !isSecondDigit;
    }

    if (Check_sum % 10 == 0)
    {
        return TERMINAL_OK;

    }
    else
    {
        return INVALID_CARD;

    }
}

/******************************************* End of isValidCardPAN Function **********************************************/




/******************************************* isValidCardPAN Test Function **********************************************/
void isValidCardPANTest(void)
{
    printf("Tester Name: [Amr WAHBA]\n");
    printf("Function Name: isValidCardPAN\n");

    // Test Case 1: Valid PAN
    printf("Test Case 1:\n");
    printf("Input Data: [User input]\n");
    printf("Expected Result: TERMINAL_OK\n");
    ST_cardData_t cardData1;
    EN_cardError_t panResult1 = getCardPAN(&cardData1);
    if (panResult1 == CARD_OK)
    {
        EN_terminalError_t result1 = isValidCardPAN(&cardData1);
        if (result1 == TERMINAL_OK)
            printf("Actual Result: TERMINAL_OK\n\n");
        else
            printf("Actual Result: INVALID_CARD\n\n");
    }
    else
    {
        printf("Actual Result: Invalid PAN input. Please try again.\n\n");
    }

    // Test Case 2: Invalid PAN
    printf("Test Case 2:\n");
    printf("Input Data: [User input]\n");
    printf("Expected Result: INVALID_CARD\n");
    ST_cardData_t cardData2;
    EN_cardError_t panResult2 = getCardPAN(&cardData2);
    if (panResult2 == CARD_OK)
    {
        EN_terminalError_t result2 = isValidCardPAN(&cardData2);
        if (result2 == TERMINAL_OK)
            printf("Actual Result: TERMINAL_OK\n\n");
        else
            printf("Actual Result: INVALID_CARD\n\n");
    }
    else
    {
        printf("Actual Result: Invalid PAN input. Please try again.\n\n");
    }

}

/******************************************* End of isValidCardPAN Test Function **********************************************/
