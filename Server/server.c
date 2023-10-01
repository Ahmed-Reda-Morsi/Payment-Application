#include"server.h"
#include"database.h"

/******************************************* Start of recieveTransactionData Function **********************************************/

uint32_t totalNumOfTransactions = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    // Extract relevant data from the transaction
    ST_cardData_t cardData = transData->cardHolderData;
    ST_terminalData_t terminalData = transData->terminalData;
    float transactionAmount = terminalData.transAmount;

    // Check if the account exists and retrieve its reference
    EN_serverError_t accountCheck = isValidAccount(&cardData, accountsDB);

    if (accountCheck == ACCOUNT_NOT_FOUND)
    {
        return FRAUD_CARD;  // Account does not exist
    }

    if (accountCheck == BLOCKED_ACCOUNT)
    {
        return DECLINED_STOLEN_CARD;  // Account is blocked
    }

    // Check if the account has sufficient funds
    EN_serverError_t amountCheck = isAmountAvailable(&terminalData, accountsDB);

    if (amountCheck == LOW_BALANCE)
    {
        return DECLINED_INSUFFECIENT_FUND;  // Insufficient funds
    }

    // Update the database with the new balance
    for (int i = 0; i < 255; i++)
    {
        if (strcmp(cardData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
        {
            accountsDB[i].balance -= transactionAmount;
            break;
        }
    }

    // Save the transaction to the transaction database
    for (int i = 0; i < 255; i++)
    {
        if (transactionDB[i].transactionSequenceNumber == 0)
        {
            transactionDB[i] = *transData;  // Copy the transaction data
            break;
        }
    }

    return APPROVED;  // Transaction approved
}


/******************************************* End recieveTransactionData Function **********************************************/


/******************************************* Start of recieveTransactionDataTest Function **********************************************/

void recieveTransactionDataTest(void)
{
    printf("Tester Name: Amr WAHBA \n");
    printf("Function Name: recieveTransactionData\n\n");

    // Test Case 1: Happy-case scenario (valid account, sufficient funds)
    printf("Test Case 1:\n");
    printf("Input Data:\n");
    printf("  Account exists, sufficient funds\n");
    // Create a dummy transaction for testing
    ST_transaction_t happyCaseTransaction =
    {
        .cardHolderData = {
            .primaryAccountNumber = "8989374615436851",
        },
        .terminalData = {
            .transAmount = 500.0,
        },
    };
    printf("Expected Result: Transaction approved!\n");
    EN_transState_t result1 = recieveTransactionData(&happyCaseTransaction);
    printf("Actual Result: ");
    if (result1 == APPROVED)
    {
        printf("Transaction approved!\n");
    }
    else
    {
        printf("Transaction not approved.\n");
    }
    printf("\n");


    // Test Case 2: Account doesn't exist
    printf("Test Case 2:\n");
    printf("Input Data:\n");
    printf("  Account does not exist\n");
    ST_transaction_t accountNotFoundTransaction =
    {
        .cardHolderData = {
            .primaryAccountNumber = "1234567890123456",  // Invalid account number
        },
        .terminalData = {
            .transAmount = 1000.0,
        },
    };
    printf("Expected Result: Transaction declined: Fraudulent card.\n");
    EN_transState_t result2 = recieveTransactionData(&accountNotFoundTransaction);
    printf("Actual Result: ");
    if (result2 == FRAUD_CARD)
    {
        printf("Transaction declined: Fraudulent card.\n");
    }
    else
    {
        printf("Transaction not declined as expected.\n");
    }
    printf("\n");

}


//******************************************* End of recieveTransactionDataTest **********************************************/



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

/******************************************* Start of listSavedTransactions Function **********************************************/

void listSavedTransactions(void)
{
    printf("List of Saved Transactions:\n");
    printf("#########################\n");

    for (int i = 0; i < 255; i++)
    {
        if (transactionDB[i].transactionSequenceNumber != 0)
        {
            printf("#########################\n");
            printf("Transaction Sequence Number: %u\n", transactionDB[i].transactionSequenceNumber);
            printf("Transaction Date: %s\n", transactionDB[i].terminalData.transactionDate);
            printf("Cardholder Name: %s\n", transactionDB[i].cardHolderData.cardHolderName);
            printf("PAN: %s\n", transactionDB[i].cardHolderData.primaryAccountNumber);
            printf("Card Expiration Date: %s\n", transactionDB[i].cardHolderData.cardExpirationDate);
            printf("Transaction Amount: %.2f\n", transactionDB[i].terminalData.transAmount);
            printf("Terminal Max Amount: %.2f\n", transactionDB[i].terminalData.maxTransAmount);

            printf("Transaction State: ");
            switch (transactionDB[i].transState)
            {
            case APPROVED:
                printf("APPROVED\n");
                break;
            case DECLINED_INSUFFECIENT_FUND:
                printf("DECLINED_INSUFFECIENT_FUND\n");
                break;
            case DECLINED_STOLEN_CARD:
                printf("DECLINED_STOLEN_CARD\n");
                break;
            case FRAUD_CARD:
                printf("FRAUD_CARD\n");
                break;
            case INTERNAL_SERVER_ERROR:
                printf("INTERNAL_SERVER_ERROR\n");
                break;
            default:
                printf("Unknown state\n");
            }
            printf("#########################\n");
        }
    }
}
/******************************************* End listSavedTransactions Function ********************************************************/




/******************************************* Start of listSavedTransactionsTest Function **********************************************/

void listSavedTransactionsTest(void)
{
    printf("Tester Name: Your Name\n");
    printf("Function Name: listSavedTransactions\n\n");

    // Test Case 1: Happy-case scenario (transactions present in the database)
    printf("Test Case 1:\n");
    printf("Input Data:\n");
    printf("  Transaction database contains transactions\n");
    // Assuming the transaction database has valid transactions
    printf("Expected Result: Transactions should be printed in the specified format\n");
    printf("Actual Result:\n");
    listSavedTransactions();
    printf("\n");

    // Test Case 2: No transactions in the database
    printf("Test Case 2:\n");
    printf("Input Data:\n");
    printf("  Transaction database is empty\n");
    // Clear the transaction database
    for (int i = 0; i < 255; i++)
    {
        transactionDB[i].transactionSequenceNumber = 0;
    }
    printf("Expected Result: No transactions should be printed\n");
    printf("Actual Result:\n");
    listSavedTransactions();
    printf("\n");


}
//******************************************* End listSavedTransactionsTest Function **********************************************/


}
