// ConditionalDependencyAnalyzer.cpp

/** Implementation file for the ConditionalDependencyAnalyzer class.
 */

#include "ConditionalDependencyAnalyzer.h"
#include <windows.h>
#include <fstream>
#include <iostream>

static inline bool trim_comment( std::string &input_string )
{
    size_t found = input_string.find( COMMENT );

    if( found != std::string::npos )
    {
        input_string.erase( found );
    }

    return trim_string( input_string );
}

static inline bool write_file( const char filename[], const std::string &content )
{
    std::ofstream write_file( filename );

    if( write_file.is_open() )
    {
        write_file << content;
        write_file.close();
        return true;
    }
    else
    {
        return false;
    }
}

int main( int argc, char const *argv[] )
{
    ConditionalDependencyAnalyzer cda;
    //ConditionalDependencyAnalyzer cda( "C:\\Users\\ArcDM\\github\\ConditionalDependencyAnalyzer\\TestDependency3.txt" );

    std::cout << cda << std::endl;

    std::string result = cda.analyze();

    std::cout << result;

    //write_file( "C:\\Users\\ArcDM\\github\\ConditionalDependencyAnalyzer\\output.txt", result );
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

                    if( trim_string( statement ) )
                    {
                        data += LogicalMatrix( statement );
                    }
                    else
                    {
                        data += LogicalMatrix( line + " || !" + line );
                    }
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
    std::vector< std::string > changed, unattained( identifiers );
    std::ostringstream output;
    size_t index, count = 0, iteration = 0, size = identifiers.size(), limit;
    bool complete, truth_value;
    std::string *identifier_reference;
    std::vector< std::string >::iterator found;

    if( size < 64 )
    {
        limit = 1 << size;
    }
    else
    {
        limit = -1;
    }

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

    if( !data.empty() )
    {
        do {
            result = data.evaluate( truth_table );

            complete = true;
            index = 0;

            for( const std::string& identifier : identifiers )
            {
                truth_value = result[ index ];
                complete &= truth_value;

                if( truth_table[ identifier ] ^ truth_value )
                {
                    truth_table[ identifier ] = truth_value;
                    changed.push_back( identifier );

                    found = find( unattained.begin(), unattained.end(), identifier );

                    if( found != unattained.end() )
                    {
                        unattained.erase( found );
                    }
                }

                ++index;
            }

            if( !changed.empty() )
            {
                output << "Step" << ++iteration << ":" << std::endl;

                for( const std::string& identifier : changed )
                {
                    output << ++count << ": " << identifier << ( truth_table[ identifier ]? "" : " ( falsified )" ) << std::endl;
                }
            }

            if( changed.empty() || iteration >= limit )
            {
                changed.clear();

                for( const std::string& identifier : identifiers )
                {
                    if( !truth_table[ identifier ] && find( unattained.begin(), unattained.end(), identifier ) == unattained.end() )
                    {
                        changed.push_back( identifier );
                    }
                }

                if( !changed.empty() )
                {
                    output << "Identifiers ended as False:" << std::endl;

                    for( const std::string& identifier : changed )
                    {
                        output << identifier << std::endl;
                    }
                }

                if( !unattained.empty() )
                {
                    output << "Unattainable Identifiers:" << std::endl;

                    for( const std::string& identifier : unattained )
                    {
                        output << identifier << std::endl;
                    }
                }

                break;
            }

            changed.clear();
        } while( !complete );
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
