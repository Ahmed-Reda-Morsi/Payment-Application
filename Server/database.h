#ifndef DATABASE_H_
#define DATABASE_H_
#endif // SERVER_H_INCLUDED
#include "server.h"

ST_accountsDB_t accountsDB[255] = {
    {2000.0, RUNNING, "8989374615436851"},
    {100000.0, BLOCKED, "5807007076043875"},
    {1500.0, RUNNING, "4532015112830366"},
    {8500.0, RUNNING, "371449635398431"},
    {6000.0, RUNNING, "6011111111111117"},
    // Add more accounts as needed for testing
};


ST_transaction transactionDB[255] = {0};
