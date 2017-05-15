//
// Created by izeren on 5/15/17.
//
#pragma once

#include <memory>
#include <condition_variable>
#include "MyExceptions.h"

template<typename Type>
struct State {
    bool isValueGot;
    bool isExceptionGot;
    Type value;
    std::exception exception;

    State( ) : isValueGot( false ), isExceptionGot( false ) { }
};

class IsProcessedCheckable {
public:
    virtual bool IsProcessed( ) = 0;
};

template<typename Type>
class Future : public IsProcessedCheckable {
public:
    Future( std::shared_ptr<State<Type>> state, std::shared_ptr<std::mutex> mutex,
              std::shared_ptr<std::condition_variable> conditionVariable ) :
            state( state ),
            mutex( mutex ),
            conditionVariable( conditionVariable ) { };

    Type Get( ) {
        std::unique_lock<std::mutex> lock( *mutex );
        conditionVariable->wait( lock, [ & ]( ) { return state->isValueGot; } );
        if ( state->isExceptionGot ) {
            throw state->exception;
        }
        return state->value;
    }

    bool TryGet( Type &value ) {
        std::unique_lock<std::mutex> lock( *mutex );
        if ( state->isValueGot ) {
            if ( state->isExceptionGot ) {
                throw state->exception;
            }
            value = state->value;
            return true;
        }
        return false;
    }

    bool IsProcessed( ) {
        std::unique_lock<std::mutex> lock( *mutex );
        return state->isValueGot;
    }

private:
    std::shared_ptr<State<Type>> state;
    std::shared_ptr<std::mutex> mutex;
    std::shared_ptr<std::condition_variable> conditionVariable;
};