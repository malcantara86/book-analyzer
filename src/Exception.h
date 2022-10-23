#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

namespace bookanalyzer {

class Exception {
};

// Parsing exception
class ParseException : public Exception {
};

// Parsing exception
class updateBooksException : public Exception {
};


// executeOrder() exception
class executeOrderException : public Exception {
};


}  // bookanalyzer namespace

#endif //PARSER_EXCEPTION_H