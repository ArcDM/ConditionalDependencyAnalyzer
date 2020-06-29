// ConditionalDependencyAnalyzer.h

/** Header file for the LogicalMatrix class.
 */

#ifndef __ConditionalDependencyAnalyzer_h_included__
#define __ConditionalDependencyAnalyzer_h_included__

#include "LogicalMatrix\LogicalMatrix.h"
#include "LogicalMatrix\LogicalMatrix.cpp"
#include <vector>

class ConditionalDependencyAnalyzer
{
    #define DELIMITER ";"
    #define COMMENT "#"
    #define INITIAL_FLAG "###INITIAL###"

    private:
        LogicalMatrix data;
        std::vector< std::string > identifiers;
        std::vector< std::string > initial_identifiers;
    public:
        ConditionalDependencyAnalyzer();
        ConditionalDependencyAnalyzer( const char filename[] );

        std::string analyze();

        bool empty() { return data.empty(); }
        friend std::ostream &operator<<( std::ostream &output, const ConditionalDependencyAnalyzer &object_arg );

};

#endif
