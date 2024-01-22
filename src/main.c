// prpf - psuedo random port forwarding
// Copyright (C) 2023 Maciej Sawka <maciejsawka@gmail.com> <msaw328@kretes.xyz>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include <stdio.h>
#include <stddef.h>
#include <string.h>

// The entry point of the program, this file mostly deals with the subcommands and passing execution to one of the subcommand main functions

// Type of a main function - used to return function ponters etc
typedef int (main_func_t) (int, char**);

// External main functions for subcommands
extern int main_help(int, char**);
extern int main_listen(int, char**);
extern int main_sequence(int, char**);

// Checks if subcommand string matches based on prefix - both "lis" and "l" match command "listen", but "lxst" does not
static int subcommand_check_match(char* str, char* cmd) {
    if(strlen(str) == 0) return 0; // By the algorithm an empty string matches any command, but we do not want it to match anything

    if(strlen(str) > strlen(cmd)) return 0; // If str is longer than the command there is no way it matches    

    size_t i = 0;
    while(str[i] != 0) {
        if(str[i] != cmd[i]) return 0; // If mismatch found return 0
        i++;
    }

    return 1;
}

// Static mapping of string to main function
static struct {
    char* subcmd_str;
    main_func_t* main_ptr;
} __subcommands[] = {
    {
        .subcmd_str = "listen",
        .main_ptr = main_listen
    },
    {
        .subcmd_str = "sequence",
        .main_ptr = main_sequence
    },
    {
        .subcmd_str = "help",
        .main_ptr = main_help
    }
};

// Chooses the subcommand main function from the ones defined above
#define STATUS_SUBCMD_UNKNOWN ((main_func_t*) -1)
#define STATUS_SUBCMD_AMBIGUOUS ((main_func_t*) -2)
static main_func_t* subcommand_choose(char* subcmd_str) {
    main_func_t* result = NULL;

    size_t subcmds_n = sizeof(__subcommands) / sizeof(__subcommands[0]);

    for(size_t i = 0; i < subcmds_n; i++) {
        if(subcommand_check_match(subcmd_str, __subcommands[i].subcmd_str)) {
            if(result != NULL)
                return STATUS_SUBCMD_AMBIGUOUS;
            
            result = __subcommands[i].main_ptr;
        }
    }

    if(result == NULL)
        return STATUS_SUBCMD_UNKNOWN;

    return result;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Error - not enough arguments, run as \"%s help\" to show possible options\n", argv[0]);
        return 0;
    }

    char* subcmd_str = argv[1];
    main_func_t* main_subcmd = subcommand_choose(subcmd_str);

    if(main_subcmd == STATUS_SUBCMD_AMBIGUOUS) {
        printf("Error - ambiguous subcommand \"%s\", try typing more\n", subcmd_str);
        return 0;
    }

    if(main_subcmd == STATUS_SUBCMD_UNKNOWN) {
        printf("Error - unknown subcommand \"%s\", run as \"%s help\" to show possible options\n", subcmd_str, argv[0]);
        return 0;
    }

    return main_subcmd(argc, argv);
}
