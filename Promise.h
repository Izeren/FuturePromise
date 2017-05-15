//
// Created by izeren on 5/15/17.
//
#pragma once

#include <condition_variable>

#include "Future.h"

template<typename Type>
class Promise
{
public:
    Promise()
            : mutex( new std::mutex ), state( new State<Type>() ),
              isValueGot( false ), conditionVariable( new std::condition_variable() )
    {};

    void SetValue( const Type& value )
    {
        std::unique_lock<std::mutex> lock( *mutex );
        if( state->isValueGot ) {
            auto e = std::exception();
            throw ValueAlreadySet();
        }
        state->isValueGot = true;
        state->value = value;
        conditionVariable->notify_one();
    }

    void SetException( const std::exception& exception )
    {
        std::unique_lock<std::mutex> lock( *mutex );
        if( state->isValueGot ) {
            throw ExceptionAlreadySet();
        }
        state->isValueGot = true;
        state->isExceptionGot = true;
        state->exception = exception;
        conditionVariable->notify_one();
    }

    std::shared_ptr<Future<Type>> GetFuture()
    {
        if( isValueGot ) {
            throw FutureAlreadyGot();
        }
        isValueGot = true;
        return std::shared_ptr<Future<Type>>( new Future<Type>( state, mutex, conditionVariable ) );
    }

private:
    std::shared_ptr<std::mutex> mutex;
    std::shared_ptr<std::condition_variable> conditionVariable;
    std::shared_ptr<State<Type>> state;
    bool isValueGot;
};