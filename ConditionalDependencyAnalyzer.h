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
    #define COMMENT = "#";
    #define DELIMITER = ";";

    private:
        LogicalMatrix data;
        std::vector< std::string > initial_identifiers;
    public:
        ConditionalDependencyAnalyzer();
        ConditionalDependencyAnalyzer( const char filename[] );

        void Analyze();

        bool empty() { return data.empty(); }
        friend std::ostream &operator<<( std::ostream &output, const ConditionalDependencyAnalyzer &object_arg );

};

#endif
