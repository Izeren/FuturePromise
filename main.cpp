#include <iostream>
#include <thread>

#include "Promise.h"
#include "Async.h"
//#include "Chain.h"

#include <vector>

void TestFuturePromiseGetValue( ) {
    std::cout << "Test 1(-------------------Simple future promise--------------------)\n";
    Promise<int> promise;
    std::shared_ptr<Future<int>> future = promise.GetFuture( );
    std::thread tempThread(
            [ ]( std::shared_ptr<Future<int>> localFuture ) {
                std::cout << "Value got: " << localFuture->Get( ) << "\n";
            },
            future
    );
    std::cout << "Value hasn't got yet\n";
    promise.SetValue( 42 );
    std::cout << "Value has already set\n";
    tempThread.join( );
}

void TestExceptions( ) {
    std::cout << "Test 1(-------------------Exceptions test--------------------)\n";
    Promise<int> promise;
    std::shared_ptr<Future<int>> future = promise.GetFuture( );
    std::thread tempThread(
            [ ]( std::shared_ptr<Future<int>> localFuture ) {
                try {
                    int index = 0;
                    while ( !localFuture->TryGet( index )) {
                        std::cout << "Waiting " << index++ << "\n";
                        std::this_thread::sleep_for( std::chrono::seconds( 1 ));
                    }
                } catch ( std::exception &e ) {
                    std::cout << "Exception: " << e.what( ) << "\n";
                }
            },
            future
    );
    std::cout << "Before setting of exception\n";
    std::this_thread::sleep_for( std::chrono::seconds( 3 ));
    promise.SetException( ExceptionAlreadySet( ));
    std::cout << "Exception has been set\n" << std::endl;
    tempThread.join( );
}

int sum( int max ) {
    int sum = 0;
    while ( max > 0 ) {
        sum += max--;
    }
    std::this_thread::sleep_for( std::chrono::seconds( rand( ) % 5 ));
    return sum;
}

void TestAsync( ) {
    std::cout << "Test 1(-------------------Exceptions test--------------------)\n";
    srand( 1 );
    AsyncExecutor executor( 5 );
    std::vector<std::shared_ptr<Future<int>>> results;
    std::cout << "Async tasks: " << std::endl;
    for ( int i = 0; i < 10; i++ ) {
        results.push_back( executor.Execute<int>( std::bind( sum, rand( ) % 1000)));
    }
    std::cout << "Sync tasks: " << std::endl;
    for ( int i = 0; i < 3; i++ ) {
        results.push_back( executor.Execute<int>( std::bind( sum, rand( ) % 1000), false ));
    }
    for ( int i = 0; i < results.size( ); i++ ) {
        std::cout << results[ i ]->Get( ) << std::endl;
    }
    std::cout << std::endl;
}

int main( ) {
    TestFuturePromiseGetValue( );
    TestExceptions( );
    TestAsync( );
    return 0;
}