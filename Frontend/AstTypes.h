#pragma once

#include "Helpers.h"

/*
binary operators
    assignments
        ident = expr NATIVE
        ident ?= expr -> if (!ident) ident = expr
        ident {symbol}= expr -> ident = ident {symbol} expr
        symbol:
            * / // + - << >> & | ^ && || ^^

    compare
        expr1 == expr2 NATIVE
        expr1 [><] expr2 NATIVE
        expr1 [><]= expr2 -> !(expr2 [><] expr1)
        expr1 != expr2 -> expr1 == {symbol}expr2 == 0

    arithmetic NATIVE

    bitwise NATIVE

    LOGICAL
        expr1 && expr2 NATIVE
        expr1 || expr2 NATIVE
*/
enum class BinaryOperator
{
    assign,
    cmp,
    mul,
    div,
    mod,
    add,
    sub,
    lshift,
    rshift,
    bit_and,
    bit_or,
    logical_and,
    logical_or,
    logical_xor,
};

enum class UnaryOperator
{
    negation,
    bit_not,
    logical_not,
};
