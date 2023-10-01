#include"server.h"
#include"database.h"

/******************************************* Start of recieveTransactionData Function **********************************************/

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


/******************************************* End of recieveTransactionDataTest **********************************************/



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
/******************************************* End listSavedTransactionsTest Function **********************************************/
