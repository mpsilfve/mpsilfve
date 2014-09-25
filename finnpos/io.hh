#ifndef HEADER_io_hh
#define HEADER_io_hh

#include <string>
#include <vector>
#include <exception>
#include <iostream>

typedef std::vector<std::string> StringVector;

struct SyntaxError : public std::exception
{};

struct EmptyLine : public std::exception
{};

struct Entry
{
  Entry(void);

  std::string token;
  StringVector feat_templates;
  std::string lemma;
  StringVector labels;
  std::string annotations;
};

void split(const std::string &str, StringVector &target, char delim);

Entry get_next_line(std::istream &in);

#endif // HEADER_io_hh
