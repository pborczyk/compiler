#include <iostream>
#include "global.h"

int main(int argc, char** argv) {
    ++argv, --argc;
    yyin = fopen("t3.pas", "r");
    yyparse ();
    exit (0);
}