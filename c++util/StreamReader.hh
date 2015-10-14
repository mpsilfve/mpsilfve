#ifndef HEADER_StreamReader_hh
#define HEADER_StreamReader_hh

#include <iostream>
#include <fstream>
#include <string>

class StreamReader
{
public:
  StreamReader(void);
  StreamReader(const std::string &fn);
  StreamReader(std::istream &in);

  bool readline(void);
  const std::string &getline(void) const;

private:
  std::ifstream in;
  std::istream * pin;
  std::string line;
};

#endif // HEADER_StreamReader_hh
