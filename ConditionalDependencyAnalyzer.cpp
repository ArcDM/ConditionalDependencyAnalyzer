// ConditionalDependencyAnalyzer.cpp

/** Implementation file for the ConditionalDependencyAnalyzer class.
 */

#include "ConditionalDependencyAnalyzer.h"
#include <windows.h>
//#include <stdio.h>
#include <fstream>
#include <iostream>

int main( int argc, char const *argv[] )
{
    std::cout << "Demo test" << std::endl;

    //ConditionalDependencyAnalyzer cda;
    ConditionalDependencyAnalyzer cda( "C:\\Users\\ArcDM\\github\\ConditionalDependencyAnalyzer\\TestDependency3.txt" );

    std::cout << cda << std::endl;

    std::string result = cda.analyze();

    std::cout << result << std::endl;
}


static inline bool trim_comment( std::string &input_string )
{
    size_t found = input_string.find( COMMENT );

    if( found != std::string::npos )
    {
        input_string.erase( found );
    }

    return trim_string( input_string );
}

ConditionalDependencyAnalyzer::ConditionalDependencyAnalyzer()
{
    OPENFILENAMEA openfilename = { sizeof( OPENFILENAMEA ) };
    char filename[ MAX_PATH ] = "";

    openfilename.lStructSize = sizeof( OPENFILENAME );
    openfilename.lpstrFile = filename;
    openfilename.nMaxFile = MAX_PATH;

    openfilename.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";

    if( GetOpenFileNameA( &openfilename ) )
    {
        *this = ConditionalDependencyAnalyzer( filename );
    }
    else
    {
        std::cout << "No file selected" << std::endl;
    }
}

ConditionalDependencyAnalyzer::ConditionalDependencyAnalyzer( const char filename[] )
{
    size_t found, delimiter_length = std::string( DELIMITER ).length();
    std::string line, statement;
    std::ifstream read_file( filename );

    if( read_file.is_open() )
    {
        while( getline( read_file, line ) )
        {
            if( line.find( INITIAL_FLAG ) == 0 )
            {
                break;
            }

            if( trim_comment( line ) )
            {
                found = line.find( DELIMITER );

                if( found == std::string::npos )
                {
                    statement = "";
                }
                else
                {
                    statement = line.substr( found + delimiter_length );
                    line.erase( found );
                }

                if( trim_string( line ) )
                {
                    identifiers.push_back( line );
                    LogicalMatrix temp_LogicalMatrix( trim_string( statement )? statement : "!" + line );
                    data += temp_LogicalMatrix;
                }
            }
        }

        while( getline( read_file, line ) )
        {
            if( trim_comment( line ) )
            {
                initial_identifiers.push_back( line );
            }
        }

        read_file.close();
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }
}

std::string ConditionalDependencyAnalyzer::analyze()
{
    std::map< std::string, bool > truth_table;
    std::vector< bool > result;
    std::vector< std::string > changed;
    std::ostringstream output;
    size_t index, count = 0, iteration = 0;

    for( const std::string& identifier : identifiers )
    {
        truth_table[ identifier ] = false;
    }

    output << ( initial_identifiers.empty()? "" : "Initial Identifiers:\n" );

    for( const std::string& identifier : initial_identifiers )
    {
        output << identifier << std::endl;

        truth_table[ identifier ] = true;
    }

    while( !data.empty() )
    {
        result = data.evaluate( truth_table );

        index = identifiers.size();

        do {
            if( result[ --index ] )
            {
                truth_table[ identifiers[ index ] ] = true;
                changed.push_back( identifiers[ index ] );

                identifiers.erase( identifiers.begin() + index );
                data.remove_statement( index );
            }
        } while( index > 0 );

        if( changed.empty() )
        {
            output << "Unattainable Identifiers:" << std::endl;

            for( const std::string& identifier : identifiers )
            {
                output << identifier << std::endl;
            }

            break;
        }
        else
        {
            output << "Step" << ++iteration << ":" << std::endl;

            do {
                output << ++count << ": " << changed.back() << std::endl;
                changed.pop_back();
            } while( !changed.empty() );
        }
    }

    return output.str();
}


std::ostream &operator<<( std::ostream &output, const ConditionalDependencyAnalyzer &object_arg )
{
    auto print_vector = [ &output ]( const std::vector< std::string > &input_vector )
    {
        std::string delim = "";

        for( std::string const& value : input_vector )
        {
            output << delim << value;
            delim = ", ";
        }
    };

    if( !object_arg.data.empty() )
    {
        output << "Conditions: " << object_arg.data << std::endl;
    }

    if( !object_arg.identifiers.empty() )
    {
        output << "Identifiers: ";

        print_vector( object_arg.identifiers );

        output << std::endl;
    }

    if( !object_arg.initial_identifiers.empty() )
    {
        output <<"Initial Identifiers: ";

        print_vector( object_arg.initial_identifiers );

        output << std::endl;
    }

    return output;
}
