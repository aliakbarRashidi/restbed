/*
 * Copyright (c) 2013, 2014, 2015 Corvusoft
 */

//System Includes
#include <memory>

//Project Includes
#include <corvusoft/restbed/settings.hpp>
#include <corvusoft/restbed/ssl_settings.hpp>

//External Includes
#pragma warning( push )
#pragma warning( disable:4702 )
#include <catch.hpp>
#pragma warning( pop )

//System Namespaces
using std::shared_ptr;
using std::make_shared;

//Project Namespaces
using restbed::Settings;
using restbed::SSLSettings;

//External Namespaces

TEST_CASE( "validate default instance values", "[settings]" )
{
    const Settings settings;
    
    REQUIRE( settings.get_ssl_settings( ) == nullptr );
}

TEST_CASE( "validate setters modify default values", "[settings]" )
{
    auto ssl_settings = make_shared< SSLSettings >( );
    ssl_settings->set_port( 3434 );
    
    Settings settings;
    settings.set_ssl_settings( ssl_settings );
    
    REQUIRE( settings.get_ssl_settings( )->get_port( ) == 3434 );
}
