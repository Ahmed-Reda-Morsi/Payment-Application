#include"server.h"

uint32_t totalNumOfTransactions = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{

}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)\
{

}
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{

}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{

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
