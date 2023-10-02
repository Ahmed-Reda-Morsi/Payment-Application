#include"server.h"
#include"database.h"
extern ST_accountsDB_t* Global_accountReference;

/******************************************* Start of recieveTransactionData Function **********************************************/
ST_accountsDB_t *Global_accountReference=&accountsDB[0];  // Declare the external global variable
uint32_t totalNumOfTransactions = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    // Extract relevant data from the transaction
    ST_cardData_t cardData = transData->cardHolderData;
    ST_terminalData_t terminalData = transData->terminalData;
    float transactionAmount = terminalData.transAmount;

    // Check if the account exists and retrieve its reference
    EN_serverError_t accountCheck = isValidAccount(&cardData,  &Global_accountReference);

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
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t **Valid_Local_accountReference)
{
    uint8_t counter;
    uint8_t enteredPAN[20];
    uint8_t completeFlag = 0;

    for (counter = 0; counter < 255; counter++)
    {
        // Compare enteredPAN with each account's primary account number in the accountsDB
        if (!(strcmp(cardData->primaryAccountNumber, accountsDB[counter].primaryAccountNumber)))
        {
            *Valid_Local_accountReference = &accountsDB[counter];
            completeFlag = 1; // Set the flag to indicate a successful match
            break; // Exit the loop since a match was found
        }
    }

    if (completeFlag)
    {

        return SERVER_OK; // Return SERVER_OK if a matching account was found
    }
    else
    {
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
EN_serverError_t isBlockedAccount(ST_accountsDB_t *Blocked_Local_accountRefrence)
{
    uint8_t errorStatus=SERVER_OK;                       // return variable for error statu
    // check if the account status is running or not
    if (Blocked_Local_accountRefrence->state!=RUNNING)
    {
        errorStatus=BLOCKED_ACCOUNT;
    }

    return errorStatus;
}

//_______________| implementation isBlockedAccountTest |______________________
void isBlockedAccountTest(void)
{
    ST_accountsDB_t accountDBTest[3]= {{0,RUNNING},{0,BLOCKED},{0}};
    uint8_t expectedResult[3]= {SERVER_OK,BLOCKED_ACCOUNT,SERVER_OK};
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
    termData.transAmount = 10000.0;
    resultFlag = isAmountAvailable(&termData,&accountRefrence);
    if (resultFlag == LOW_BALANCE)
        printf("Actual Result: Low Balance\n");
    else
        printf("Actual Result: Low Balance\n");
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
    transactionDB[totalNumOfTransactions].transactionSequenceNumber = totalNumOfTransactions;
    fputs(",", fptr);
    fprintf(fptr, "Transaction Date:%s", transData->terminalData.transactionDate);
    fputs(",", fptr);
    strcpy(transactionDB[totalNumOfTransactions].terminalData.transactionDate,transData->terminalData.transactionDate);
    fprintf(fptr, "Card Holder Name:%s", transData->cardHolderData.cardHolderName);
    fputs(",", fptr);
    strcpy(transactionDB[totalNumOfTransactions].cardHolderData.cardHolderName,transData->cardHolderData.cardHolderName);
    fprintf(fptr, "PAN:%s", transData->cardHolderData.primaryAccountNumber);
    fputs(",", fptr);
    strcpy(transactionDB[totalNumOfTransactions].cardHolderData.primaryAccountNumber,transData->cardHolderData.primaryAccountNumber);
    fprintf(fptr, "EXP Date:%s", transData->cardHolderData.cardExpirationDate);
    fputs(",", fptr);
    strcpy(transactionDB[totalNumOfTransactions].cardHolderData.cardExpirationDate,transData->cardHolderData.cardExpirationDate);
    fprintf(fptr, "Amount:%.2f", transData->terminalData.transAmount);
    fputs(",", fptr);
    transactionDB[totalNumOfTransactions].terminalData.transAmount = transData->terminalData.transAmount;
    fprintf(fptr, "Max Amount:%.2f", transData->terminalData.maxTransAmount);
    fputs(",", fptr);
    transactionDB[totalNumOfTransactions].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;

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

//******************************************* Start of listSavedTransactions Function **********************************************/

void listSavedTransactions(void)
{

    if (totalNumOfTransactions==0)
    {
        printf(" --> NO TRANSACTIONS EXIST <-- \n");
    }
    else
    {
        printf("__| List of Saved Transactions:\n");
        printf("___________________________\n");

        for (int i = 0; i < 255; i++)
        {
            if (transactionDB[i].transactionSequenceNumber != 0)
            {
                printf("_______________|\n");
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
                printf("_______________|\n");
            }
        }
    }
}
//******************************************* End listSavedTransactions Function ********************************************************/




//******************************************* Start of listSavedTransactionsTest Function **********************************************/

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

