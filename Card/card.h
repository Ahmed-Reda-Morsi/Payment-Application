#ifndef CARD_H
#define CARD_H

#include "inttypes.h"
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 24
#define MIN_NAME_LENGTH 20
typedef struct ST_cardData_t
{
     uint8_t cardHolderName[25];
     uint8_t primaryAccountNumber[20];
     uint8_t cardExpirationDate[6];
}ST_cardData_t;
typedef enum EN_cardError_t
{
     CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;


EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);

//_______________| testing functions |____________________
EN_cardError_t isNameValid(uint8_t *name);
void getCardHolderNameTest(void);
void getCardExpiryDateTest (void);
#endif
