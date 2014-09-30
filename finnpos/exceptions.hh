#ifndef HEADER_exceptions_hh
#define HEADER_exceptions_hh

#include <exception>

struct SyntaxError : public std::exception
{};

struct EmptyLine : public std::exception
{};

struct NumericalRangeError : public std::exception
{};

struct IllegalLabel : public std::exception
{};

struct LabelNotSet : public std::exception
{};

struct UnknownSuffixPair : public std::exception
{};

struct UnknownClass : public std::exception
{};

struct UnlabeledData : public std::exception
{};

struct NoLabel : public std::exception
{};

struct WordNotSet : public std::exception
{};


#endif // HEADER_exceptions_hh
