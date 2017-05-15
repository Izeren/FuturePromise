//
// Created by izeren on 5/15/17.
//

#include "MyExceptions.h"

const char *ValueAlreadySet::what( ) const throw( ) {
    return "Value has already been set\n";
}

const char *ExceptionAlreadySet::what( ) const throw( ) {
    return "Exception has already been set\n";
}


const char *FutureAlreadyGot::what( ) const throw( ) {
    return "Future has already been got\n";
}
