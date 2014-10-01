#ifndef HEADER_io_hh
#define HEADER_io_hh

#include <string>
#include <vector>
#include <iostream>

typedef std::vector<std::string> StringVector;

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

bool check(std::string &fn, std::ostream &out, std::ostream &msg_out);
bool check(std::string &fn, std::istream &in, std::ostream &msg_out);

#endif // HEADER_io_hh
