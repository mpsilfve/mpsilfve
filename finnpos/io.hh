/**
 * @file    io.hh                                                            
 * @Author  Miikka Silfverberg                                               
 * @brief   Helpers for io.                                                  
 */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// (C) Copyright 2014, University of Helsinki                                //
// Licensed under the Apache License, Version 2.0 (the "License");           //
// you may not use this file except in compliance with the License.          //
// You may obtain a copy of the License at                                   //
// http://www.apache.org/licenses/LICENSE-2.0                                //
// Unless required by applicable law or agreed to in writing, software       //
// distributed under the License is distributed on an "AS IS" BASIS,         //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  //
// See the License for the specific language governing permissions and       //
// limitations under the License.                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef HEADER_io_hh
#define HEADER_io_hh

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

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

template<class T> T reverse(T numerical_val)
{
  char * start = reinterpret_cast<char *>(&numerical_val);
  char * stop  = start + sizeof(T);
  
  std::reverse(start, stop);

  return numerical_val;
}

template<class T> T read_numerical_val(std::istream &in, bool reverse_bytes)
{
  T t;
  in.read(reinterpret_cast<char *>(&t), sizeof(T));

  return reverse_bytes ? reverse(t) : t;
}

template<class T> read_vector(std::istream &in, 
			      std::vector<T> &v, 
			      bool reverse_bytes)
{
  unsigned int size = read_numerical_val<unsigned int>(in, reverse_bytes);
				
  for (unsigned int i = 0; i < v.size(); ++i)
    {
      v.push_back(read_numerical_val(in, reverse_bytes));
    }
}

bool has_same_endianness(std::istream &in, unsigned int marker);



#endif // HEADER_io_hh
