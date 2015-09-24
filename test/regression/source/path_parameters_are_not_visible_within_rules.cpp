/*
 * Copyright (c) 2013, 2014, 2015 Corvusoft
 */

//System Includes
#include <thread>
#include <memory>
#include <functional>

//Project Includes
#include <restbed>
#include "http.hpp"

//External Includes
#pragma warning( push )
#pragma warning( disable:4702 )
#include <catch.hpp>
#pragma warning( pop )

//System Namespaces
using std::thread;
using std::function;
using std::shared_ptr;
using std::make_shared;

//Project Namespaces
using namespace restbed;

//External Namespaces

class TestRule : public Rule
{
    public:
        TestRule( void ) : Rule( )
        {
            return;
        }
        
        virtual ~TestRule( void )
        {
            return;
        }
        
        bool condition( const shared_ptr< Session > session ) final override
        {
            const auto request = session->get_request( );
            REQUIRE( "1a230096-928d-4958-90d1-a681bfff22b4" == request->get_path_parameter( "key" ) );
            return true;
        }
        
        void action( const shared_ptr< Session > session, const function< void ( const shared_ptr< Session > ) >& callback ) final override
        {
            const auto request = session->get_request( );
            REQUIRE( "1a230096-928d-4958-90d1-a681bfff22b4" == request->get_path_parameter( "key" ) );
            callback( session );
        }
};

void get_handler( const shared_ptr< Session > session )
{
    session->close( 200 );
}

TEST_CASE( "path parameters are not visible within rules", "[request]" )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/queues/{key: ^[0-9a-fA-F]{8}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{12}$}" );
    resource->set_method_handler( "GET", get_handler );
    
    auto settings = make_shared< Settings >( );
    settings->set_port( 1984 );
    
    shared_ptr< thread > worker = nullptr;
    
    Service service;
    service.publish( resource );
    service.add_rule( make_shared< TestRule >( ) );
    service.set_ready_handler( [ &worker ]( Service & service )
    {
        worker = make_shared< thread >( [ &service ] ( )
        {
            Http::Request request;
            request.method = "GET";
            request.port = 1984;
            request.host = "localhost";
            request.path = "/queues/1a230096-928d-4958-90d1-a681bfff22b4";
            
            auto response = Http::get( request );
            
            REQUIRE( 200 == response.status_code );
            
            service.stop( );
        } );
    } );
    service.start( settings );
    worker->join( );
}
