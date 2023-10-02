//Header guard to prevent multiple inclusion of the header file
#ifndef CARD_H
#define CARD_H

//Include necessary headers
#include "inttypes.h"                               //For integer types
#include <stdio.h>                                 //For standard input/output
#include <string.h>                               //For string operations

//Define constants for name length
#define MAX_NAME_LENGTH 24
#define MIN_NAME_LENGTH 20

// Define a structure to hold card data
typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];                    //Array to store card holder's name
    uint8_t primaryAccountNumber[20];             //Array to store primary account number
    uint8_t cardExpirationDate[6];                //Array to store card expiration date
} ST_cardData_t;

//Define an enumeration for card errors as required
typedef enum EN_cardError_t
{
    CARD_OK,                                      //Card is valid
    WRONG_NAME,                                   //Invalid name
    WRONG_EXP_DATE,                               //Invalid expiration date
    WRONG_PAN                                     //Invalid PAN (Primary Account Number)
} EN_cardError_t;

//Function to get the card holder's name
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);

//Function to get the card expiration date
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);

//Function to get the card PAN
EN_cardError_t getCardPAN(ST_cardData_t *cardData);

//Testing functions
EN_cardError_t isNameValid(uint8_t *name);
void getCardHolderNameTest(void);
void getCardExpiryDateTest(void);
void getCardPANTest(void);


//End of the header guard
#endif
