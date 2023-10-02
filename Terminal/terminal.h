// Header guard to prevent multiple inclusion of the header file
#ifndef TERMINAL_H
#define TERMINAL_H

#include "../Card/card.h"                   

typedef struct ST_terminalData_t
{
    float transAmount;                                        //Transaction amount
    float maxTransAmount;                                    //Maximum transaction amount
    uint8_t transactionDate[11];                            //Transaction date
} ST_terminalData_t;

typedef enum EN_terminalError_t
{
    TERMINAL_OK,                                                  //No error, operation successful
    WRONG_DATE,                                                  //Wrong date format or value
    EXPIRED_CARD,                                               //Card is expired
    INVALID_CARD,                                              //Invalid card
    INVALID_AMOUNT,                                           //Invalid transaction amount
    EXCEED_MAX_AMOUNT,                                       //Transaction amount exceeds max allowed
    INVALID_MAX_AMOUNT                                      //Invalid maximum transaction amount
} EN_terminalError_t;

//Function prototypes for terminal operations
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData); //Optional

//Testing functions Declaration
void isCardExpiredTest(void);
void getTransactionAmountTest(void);
void isBelowMaxAmountTest(void);
void setMaxAmountTest(void);
void getTransactionDateTest(void);
void isValidCardPANTest(void);

//End of the header guard
#endif
