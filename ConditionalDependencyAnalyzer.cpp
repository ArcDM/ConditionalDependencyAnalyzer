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

    std::cout << ConditionalDependencyAnalyzer() << std::endl;
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
    size_t found;
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
                //std::cout << "First set: " << line;

                // split line by ConditionalDependencyAnalyzer::DELIMITER
                found = line.find( DELIMITER );

                if( found != std::string::npos )
                {
                    statement = line.substr( found + std::string( DELIMITER ).length() );
                    line.erase( found );

                    if( trim_string( line ) && trim_string( statement ) )
                    {
                        //std::cout << "; Split: \"" << line << "\" and \"" << statement << "\"";
                        identifiers.push_back( line );
                        data += LogicalMatrix( statement );
                    }

                }

                //std::cout << std::endl;
            }
        }

        while( getline( read_file, line ) )
        {
            if( trim_comment( line ) )
            {
                //std::cout << "Second set: " << line << std::endl;
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

void ConditionalDependencyAnalyzer::Analyze()
{

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

        output << std::endl;
    };

    output << "Conditions: " << object_arg.data << "\nIdentifiers: ";

    print_vector( object_arg.identifiers );

    output << "Initial Identifiers: ";

    print_vector( object_arg.initial_identifiers );

    return output;
}
