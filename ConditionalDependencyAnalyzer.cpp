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
    std::string line;
    std::ifstream read_file( filename );

    if( read_file.is_open() )
    {
        while( getline( read_file, line ) )
        {
            if( !line.empty() )
            {
                // test if line == "###INITIAL###"

                // retain only before ConditionalDependencyAnalyzer::COMMENT
                // split line by ConditionalDependencyAnalyzer::DELIMITER
            }
        }

        read_file.close();
    }
    else
    {
        //std::cout << "Unable to open file" << std::endl;
    }
}

void ConditionalDependencyAnalyzer::Analyze()
{

}


std::ostream &operator<<( std::ostream &output, const ConditionalDependencyAnalyzer &object_arg )
{
    output << object_arg.data;
    return output;
}
