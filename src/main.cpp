#include "CLI.h"
#include "CLI_utils.h"
#include "Database.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    CLI cli;

    if (argc > 2)
    {
        cli.loadDatabase(argv[1], argv[2]);
        if (argc > 3)
            cli.askDiagnosis(argv[3]);
    }
    else
        cli.execute();

    return 0;
}

