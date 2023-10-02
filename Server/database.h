#ifndef DATABASE_H_
#define DATABASE_H_
#include "server.h"

ST_accountsDB_t accountsDB[255] = {
    {20000.0, RUNNING, "8989374615436851"},
    {100000.0, BLOCKED, "5807007076043875"},
    {1500.0, RUNNING, "4532015112830366"},
    {8500.0, RUNNING, "371449635398431"},
    {6000.0, RUNNING, "6011111111111117"},
};


ST_transaction_t transactionDB[255] =
{
    {
        {"AhmedHazemKamalEldinAb", "8989374615436851", "09/29"},
        {10000.00, 1200.00, "30/09/2023"},
        APPROVED,
        1
    },
    {
        {"AmrWahba", "4111111111111111", "09/29"},
        {10000.00, 1200.00, "30/09/2023"},
        APPROVED,
        2
    },
};

#endif
