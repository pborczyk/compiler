%{
#include "global.h"
#include <stdlib.h>
%}

%define parse.error verbose
%token id
%token num
%token sign
%token relop
%token mulop
%token assignop
%token or

%token not

%token do
%token if
%token else
%token then
%token while
%token var
%token rangeop
%token array
%token of

%token integer
%token real

%token procedure
%token function

%token begin
%token end
%token program_token


%%
program:
program_token id '(' identifier_list ')' ';'
declarations
subprogram_declarations
compound_statement
'.'

identifier_list:
id
| identifier_list ',' id

declarations:
declarations var identifier_list : type ';'
| %empty

type:
standard_type
| array '[' num rangeop num ']' of standard_type

standard_type:
integer
| real

subprogram_declarations:
subprogram_declarations subprogram_declaration ';'
| %empty

subprogram_declaration:
subprogram_head declarations compound_statement

subprogram_head:
function id arguments : standard_type ';'
| procedure id arguments ';'

arguments:
'(' parameter_list ')'
| %empty

parameter_list:
identifier_list ':' type
| parameter_list ';' identifier_list ':' type

compound_statement:
begin
optional_statements
end

optional_statements:
statement_list
| %empty

statement_list:
statement
| statement_list ';' statement

statement:
variable assignop expression
| procedure_statement
| compound_statement
| if expression then statement else statement
| while expression do statement

variable:
id
| id '[' expression ']'

procedure_statement:
id
| id '(' expression_list ')'

expression_list:
expression
| expression_list ',' expression

expression:
simple_expression
| simple_expression relop simple_expression

simple_expression:
term
| sign term
| simple_expression sign term
| simple_expression or term

term:
factor
| term mulop factor

factor:
variable
| id '(' expression_list ')'
| num
| '(' expression ')'
| not factor

%%
