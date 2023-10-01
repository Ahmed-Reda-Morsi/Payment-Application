#include"server.h"

uint32_t totalNumOfTransactions = 0;
ST_accountsDB_t accountsDB[255] = {
    {2000.0, RUNNING, "8989374615436851"},
    {100000.0, BLOCKED, "5807007076043875"},
    {1500.0, RUNNING, "4532015112830366"},
    {8500.0, RUNNING, "371449635398431"},
    {6000.0, RUNNING, "6011111111111117"},
    // Add more accounts as needed for testing
};

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{

}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountReference)
{
    uint8_t counter;
    uint8_t enteredPAN[20];
    uint8_t completeFlag = 0;

    strcpy(enteredPAN, cardData->primaryAccountNumber); // Copy the primary account number to enteredPAN

    for (counter = 0; counter < 255; counter++)
    {
        // Compare enteredPAN with each account's primary account number in the accountsDB
        if (!(strcmp(enteredPAN, accountsDB[counter].primaryAccountNumber)))
        {
            // If a match is found, copy the account details to accountReference
            strcpy(accountReference->primaryAccountNumber, accountsDB[counter].primaryAccountNumber);
            accountReference->balance = accountsDB[counter].balance;
            accountReference->state = accountsDB[counter].state;
            completeFlag = 1; // Set the flag to indicate a successful match
            accountReference=&(accountsDB[counter]);
        }
    }

    if (completeFlag)
    {
        return SERVER_OK; // Return SERVER_OK if a matching account was found
    }
    else
    {
        accountReference = NULL; // Set accountReference to NULL since no matching account was found
        return ACCOUNT_NOT_FOUND; // Return ACCOUNT_NOT_FOUND error code
    }
}

void isValidAccountTest(void)
{
    ST_cardData_t cardData;
    ST_accountsDB_t *accountRefrence;
    EN_serverError_t errorResult;

    strcpy(cardData.primaryAccountNumber,"8989374615436851");
    printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: isValidAccount\n");
    printf("---------------------------\n");

    // Test Case 1: Valid Card PAN
    printf("Test Case 1: Valid Card PAN\n");
    printf("Expected Result: PAN is found\n");
    errorResult = isValidAccount(&cardData,&accountRefrence);
    if (errorResult == SERVER_OK)
        printf("Actual Result: PAN is found\n");
    else
        printf("Actual Result: Wrong PAN\n");
    printf("---------------------------\n");

    strcpy(cardData.primaryAccountNumber,"8989374655436855");
    // Test Case 2: Invalid Card PAN
    printf("Test Case 2: Invalid Card PAN\n");
    printf("Expected Result: PAN is not found\n");
    errorResult = isValidAccount(&cardData,&accountRefrence);
    if (errorResult == ACCOUNT_NOT_FOUND)
        printf("Actual Result: PAN is not found\n");
    else
        printf("Actual Result: PAN is found\n");
    printf("---------------------------\n");

}

//_______________| implementation isBlockAccount |____________________________
/* This function takes a reference to the account into the database and verifies
    if it is blocked or not. */
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    uint8_t errorStatus=SERVER_OK;                       // return variable for error statu
    // check if the account status is running or not
    if (accountRefrence->state!=RUNNING)
    {
        errorStatus=BLOCKED_ACCOUNT;
    }

    return errorStatus;
}

//_______________| implementation isBlockedAccountTest |______________________
void isBlockedAccountTest(void)
{
    ST_accountsDB_t accountDBTest[3]={{0,RUNNING},{0,BLOCKED},{0}};
    uint8_t expectedResult[3]={SERVER_OK,BLOCKED_ACCOUNT,SERVER_OK};
    //   test card module.
    printf("//__________________|isBlockedAccount function testCases. |________________ \n");
    printf("Tester Name: Ahmed Reda\nFunction Name: isBlockedAccount\n");
    printf("input meaning: 0-> running | 1-> Blocked\n");

    for (uint8_t i = 0; i < 3; i++)
    {
        uint8_t callingResult=isBlockedAccount(&accountDBTest[i]);                                         // get result of calling getTransactionDate
        if (expectedResult[i]==SERVER_OK && callingResult==SERVER_OK)
        {
            printf("Test Case %d:\nIntput: %d\nExpected Result:%s\nActual Result: %s\n",i,accountDBTest[i].state,"SERVER_OK","SERVER_OK");
        }
        else if (expectedResult[i]==BLOCKED_ACCOUNT && callingResult==SERVER_OK)
        {
            printf("Test Case %d:\nIntput: %d\nExpected Result:%s\nActual Result: %s\n",i,accountDBTest[i].state,"BLOCKED_ACCOUNT","SERVER_OK");
        }
        else if (expectedResult[i]==BLOCKED_ACCOUNT && callingResult==BLOCKED_ACCOUNT)
        {
            printf("Test Case %d:\nIntput: %d\nExpected Result:%s\nActual Result: %s\n",i,accountDBTest[i].state,"BLOCKED_ACCOUNT","BLOCKED_ACCOUNT");
        }
        else if (expectedResult[i]==SERVER_OK && callingResult==BLOCKED_ACCOUNT)
        {
            printf("Test Case %d:\nIntput: %d\nExpected Result:%s\nActual Result: %s\n",i,accountDBTest[i].state,"SERVER_OK","BLOCKED_ACCOUNT");
        }
        else
        {/*DO NOTHING.*/}

    }
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
    if((termData->transAmount) > (accountRefrence->balance))
    {
        return LOW_BALANCE;
    }
    else
    {
        return SERVER_OK;
    }
}

void isAmountAvailableTest(void)
{
    ST_terminalData_t termData;
    ST_accountsDB_t accountRefrence;
    // Account Balance = 2000
    accountRefrence = accountsDB[0];
    EN_serverError_t resultFlag;

	printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: isAmountAvailable\n");
    printf("---------------------------\n");

    // Test Case 1: check if the entered amount is less than the balance
    printf("Test Case 1: Valid Amount(less than the balance)\n");
    printf("Expected Result: Amount is Valid\n");
    termData.transAmount = 1000.0;
    resultFlag = isAmountAvailable(&termData,&accountRefrence);
    if (resultFlag == SERVER_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 2: check if the entered amount equals the balance
    printf("Test Case 2: Valid Amount(equals the balance)\n");
    printf("Expected Result: Amount is Valid\n");
    termData.transAmount = 2000.0;
    resultFlag = isAmountAvailable(&termData,&accountRefrence);
    if (resultFlag == SERVER_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 3: check if the entered amount more the balance
    printf("Test Case 3: Invalid Amount(more than the balance)\n");
    printf("Expected Result: Low Balance\n");
    termData.transAmount = 3000.0;
    resultFlag = isAmountAvailable(&termData,&accountRefrence);
    if (resultFlag == LOW_BALANCE)
        printf("Actual Result: Low Balance\n");
    else
        printf("Actual Result: Valid Amount\n");
    printf("---------------------------\n");

}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    FILE *fptr; // File pointer for transactions database
    fptr = fopen("Transactions DB.txt", "a"); // Open the file in append mode

    if (!fptr)
    {
        // If the file cannot be opened, display an error message
        printf("Transactions DB is not available.\n");
        return INTERNAL_SERVER_ERROR; // Return the error code for internal server error
    }

    transData->transactionSequenceNumber = totalNumOfTransactions + 1; // Assign the transaction sequence number
    totalNumOfTransactions++; // Increment the total number of transactions

    // Write transaction details to the file
    fprintf(fptr, "Transaction Number:%d", transData->transactionSequenceNumber);
    fputs(",", fptr);
    fprintf(fptr, "Transaction Date:%s", transData->terminalData.transactionDate);
    fputs(",", fptr);
    fprintf(fptr, "Card Holder Name:%s", transData->cardHolderData.cardHolderName);
    fputs(",", fptr);
    fprintf(fptr, "PAN:%s", transData->cardHolderData.primaryAccountNumber);
    fputs(",", fptr);
    fprintf(fptr, "EXP Date:%s", transData->cardHolderData.cardExpirationDate);
    fputs(",", fptr);
    fprintf(fptr, "Amount:%.2f", transData->terminalData.transAmount);
    fputs(",", fptr);
    fprintf(fptr, "Max Amount:%.2f", transData->terminalData.maxTransAmount);
    fputs(",", fptr);

    // Write the transaction state to the file
    if (transData->transState == APPROVED)
        fprintf(fptr,"Transaction State: Approved\n");
    else if (transData->transState == DECLINED_INSUFFECIENT_FUND)
        fprintf(fptr,"Transaction State: Declined Insufficient Fund\n");
    else if (transData->transState == DECLINED_STOLEN_CARD)
        fprintf(fptr,"Transaction State: Declined Stolen Card\n");
    else if (transData->transState == FRAUD_CARD)
        fprintf(fptr,"Transaction State: Fraud Card\n");
    else
        fprintf(fptr,"Transaction State: Internal Server Error\n");

    fprintf(fptr,"==================================================\n");

    fclose(fptr); // Close the file
    return SERVER_OK; // Return the success code
}

void saveTransactionTest(void)
{
    ST_transaction_t transData;
	printf("Tester Name: Ahmed Hazem Kamal\n");
    printf("Function Name: saveTransaction\n");
    printf("---------------------------\n");

    // Initialize transaction data with sample values
	strcpy(transData.cardHolderData.cardHolderName, "AhmedHazemKamalEldinAb");
	strcpy(transData.cardHolderData.primaryAccountNumber, "8989374615436851");
	strcpy(transData.cardHolderData.cardExpirationDate, "09/29");
	transData.terminalData.transAmount = 1000.0;
	transData.terminalData.maxTransAmount = 1200.0;
    strcpy(transData.terminalData.transactionDate, "30/09/2023");
    // Save the transaction to the database
	saveTransaction(&transData);

}

EN_serverError_t saveAccount()
{
    FILE *fptr; // File pointer for transactions database
    fptr = fopen("Accounts DB.txt", "a"); // Open the file in append mode
    uint8_t completeFlag = 0;
    ST_cardData_t cardData;
    if (!fptr)
    {
        // If the file cannot be opened, display an error message
        printf("Accounts DB is not available.\n");
        return INTERNAL_SERVER_ERROR; // Return the error code for internal server error
    }

    if(getCardPAN(&cardData) == CARD_OK)
    {
        // If the card PAN matches the valid PAN requirements, Increase the flag
        fprintf(fptr,"%d ",cardData.primaryAccountNumber);
        completeFlag++;
    }


    if(getCardHolderName(&cardData) == CARD_OK)
    {
        // If the card Holder Name matches the valid Name requirements, Increase the flag
        fprintf(fptr,"%s ",cardData.cardHolderName);
        completeFlag++;
    }

    if(getCardExpiryDate(&cardData) == CARD_OK)
    {
        // If the card Expire date matches the valid expire date requirements, Increase the flag
        fprintf(fptr,"%d ",cardData.cardExpirationDate);
        completeFlag++;
    }

    // If all the inputs are correct , return the success code
    if(completeFlag == 3)
        return SERVER_OK;
    else
        return SAVING_FAILED;
}

void listSavedTransactions(void)
{

}
