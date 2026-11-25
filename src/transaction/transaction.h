#pragma once

struct Transaction {
    // We use account ids to avoid circular includes between this and account
    int reference_number;
    int from = -1; 
    int to = -1;
    float amount;
};