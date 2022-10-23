#include "BookAnalyzerProcessor.h"
#include<iostream>

using namespace bookanalyzer;

int main()
{
    BookAnalyzerProcessor processor;
    processor.processOrder("28800538 A b S 44.26 100");
    processor.processOrder("28800562 A c B 44.10 100");
    processor.processOrder("28800744 R b 100");
    processor.processOrder("28800758 A d B 44.18 157");
    return 0;
}