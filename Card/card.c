#include"card.h"

uint8_t isNameValid(const char *name) {

    // Check if the entered characters is valid
    for (uint8_t i = 0; i < strlen(name) - 1; i++) {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] == ' '))) {
            return 0;
        }
    }
    return 1;
}

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    char holderName[MAX_NAME_LENGTH + 1]; // Buffer to hold the card holder's name
    uint8_t counter = 0; // Counter variable for iteration

    printf("Please Enter Your Name : \n"); // Prompt the user to enter their name
    fgets(holderName, sizeof(holderName), stdin); // Read the user's input and store it in holderName

     if (!isNameValid(holderName)) {
        return WRONG_NAME; // Name contains invalid characters
    }

    // Check various conditions to determine the validity of the name
    if ((holderName[0] == '\0') || (strlen(holderName) < MIN_NAME_LENGTH) || (strlen(holderName) > MAX_NAME_LENGTH))
    {
        return WRONG_NAME; // Return the error code for wrong name
    }
    else
    {
        strcpy(cardData->cardHolderName,holderName); // Copy the valid name to the cardData structure
        return CARD_OK; // Return the success code
    }

}

void getCardHolderNameTest(void)
{
    ST_cardData_t cardData;
    EN_cardError_t errorResult;

    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: getCardHolderName\n");
    printf("---------------------------\n");

    // Test Case 1: Valid Name
    printf("Test Case 1: Valid Name\n");
    printf("Expected Result: Card is Ok\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == CARD_OK)
        printf("Actual Result: Card is Ok\n");
    else
        printf("Actual Result: Wrong Input\n");
    printf("---------------------------\n");

    // Test Case 2: Short Name (less than 20 characters)
    printf("Test Case 2: Short Name (less than 20 characters)\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

    // Test Case 3: No input
    printf("Test Case 3: No input\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

    // Test Case 4: Name Contains invalid characters
    printf("Test Case 4: Name Contains invalid characters\n");
    printf("Expected Result: Wrong Name\n");
    errorResult = getCardHolderName(&cardData);
    if (errorResult == WRONG_NAME)
        printf("Actual Result: Wrong Name\n");
    else
        printf("Actual Result: Card is Ok\n");
    printf("---------------------------\n");

}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{

}
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{

}
