//
// Created by izeren on 5/15/17.
//
#pragma once

#pragma once

#include <list>
#include <iostream>

#include "Promise.h"
#include "Future.h"

template<typename Type>
class IFuturePackagedTask {
public:
    virtual void operator()( ) = 0;

    virtual std::shared_ptr<Future<Type>> GetFuture( ) = 0;
};

template<typename Type>
class AsyncTask : public IFuturePackagedTask<Type> {
public:
    AsyncTask( std::function<Type( )> function ) : function( function ) { }

    void operator()( ) {
        try {
            Type value = function( );
            promise.SetValue( value );
        } catch ( std::exception &e ) {
            promise.SetException( e );
        }
    }

    std::shared_ptr<Future<Type>> GetFuture( ) {
        if ( !isProcessed ) {
            future = promise.GetFuture( );
        }
        isProcessed = true;
        return future;
    }

private:
    std::function<Type( )> function;
    Promise<Type> promise;
    std::shared_ptr<Future<Type>> future;
    bool isProcessed = false;
};


class AsyncExecutor {
public:
    AsyncExecutor( int numThreads ) : numThreads( numThreads ) { }

    ~AsyncExecutor( ) {
        while ( threads.size( ) > 0 ) {
            CheckFinishedThreads( );
        }
    }

    template<typename Type>
    std::shared_ptr<Future<Type>> Execute(
            std::function<Type( )> function,
            bool isAsync = true
    ) {
        AsyncTask<Type> task( function );
        std::shared_ptr<Future<Type>> future = task.GetFuture( );

        if ( isAsync && threads.size( ) < numThreads ) {
            std::thread thread( task );
            std::cout << "Thread with id: " << thread.get_id( ) << " is successfully started\n";
            threads.push_back( Thread( std::move( thread ), future ));
        } else {
            task( );
            std::cout << "Task are executing syncronically\n";
        }
        CheckFinishedThreads( );
        return future;
    }

    void CheckFinishedThreads( ) {
        for ( auto it = threads.begin( ); it != threads.end( ); ++it ) {
            if ( it->future->IsProcessed( )) {
                std::cout << "Thread with id: " << it->thread.get_id( ) << " has already finished";
                it->thread.join( );
                threads.erase( it );
                it = threads.begin( );
            } else {
                it++;
            }
        }
    }

private:
    struct Thread {
        std::thread thread;
        std::shared_ptr<IsProcessedCheckable> future;

        Thread( std::thread thread, std::shared_ptr<IsProcessedCheckable> future ) : thread( std::move( thread )),
                                                                                     future( future ) { }
    };

    int numThreads;
    std::list<Thread> threads;
};

