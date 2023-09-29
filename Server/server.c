#include"server.h"

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{

}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)\
{

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

}
EN_serverError_t saveTransaction(ST_transaction_t *transData)
{

}
void listSavedTransactions(void)
{
    
}