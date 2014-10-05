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
#include <unordered_map>

#include "exceptions.hh"

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

bool homoendian(std::istream &in, unsigned int marker);

template<class T> T reverse_num(T numerical_val)
{
  char * start = reinterpret_cast<char *>(&numerical_val);
  char * stop  = start + sizeof(T);
  
  std::reverse(start, stop);

  return numerical_val;
}

template<class T> T read_val(std::istream &in, bool reverse_bytes)
{
  T t;
  in.read(reinterpret_cast<char *>(&t), sizeof(T));

  if (in.fail())
    { 
      throw ReadFailed(); 
    }

  return reverse_bytes ? reverse_num(t) : t;
}

template<class T> void write_val(std::ostream &out, const T &t)
{
  out.write(reinterpret_cast<const char *>(&t), sizeof(T));
  
  if (out.fail())
    { 
      throw WriteFailed();
    }
}

template<> std::string read_val(std::istream &in, bool reverse_bytes);
template<> void write_val(std::ostream &out, const std::string &str);

template<class T> std::vector<T> &read_vector(std::istream &in, 
					      std::vector<T> &v, 
					      bool reverse_bytes)
{
  unsigned int size = read_val<unsigned int>(in, reverse_bytes);
				
  for (unsigned int i = 0; i < size; ++i)
    {
      v.push_back(read_val<T>(in, reverse_bytes));
    }

  return v;
}

template<class T> void write_vector(std::ostream &out, 
				    const std::vector<T> &v)
{
  write_val<unsigned int>(out, v.size());
				
  for (unsigned int i = 0; i < v.size(); ++i)
    {
      write_val<T>(out, v.at(i));
    }
}

template<class T, class U> 
std::unordered_map<T, U> &read_map(std::istream &in,
				   std::unordered_map<T, U> &m,
				   bool reverse_bytes)
{
  unsigned int size = read_val<unsigned int>(in, reverse_bytes);

  for (unsigned int i = 0; i < size; ++i)
    {
      T t = read_val<T>(in, reverse_bytes);
      U u = read_val<U>(in, reverse_bytes);

      m[t] = u;
    }

  return m;
}

template<class T, class U> void write_map(std::ostream &out,
					  std::unordered_map<T, U> &m)
{
  write_val<unsigned int>(out, m.size());

  for (typename std::unordered_map<T, U>::const_iterator it = m.begin();
       it != m.end();
       ++it)
    {
      write_val<T>(out, it->first);
      write_val<T>(out, it->second);
    }
}

template<class T, class U> std::unordered_map<T, std::vector<U> > &
read_map(std::istream &in,
	 std::unordered_map<T, std::vector<U> > &m,
	 bool reverse_bytes)
{
  unsigned int size = read_val<unsigned int>(in, reverse_bytes);

  for (unsigned int i = 0; i < size; ++i)
    {
      T t = read_val<T>(in, reverse_bytes);

      std::vector<U> v;
      read_vector<U>(in, v, reverse_bytes);

      m[t] = v;
    }

  return m;
}

template<class T, class U> 
void write_map(std::ostream &out,
	       std::unordered_map<T, std::vector<U> > &m)
{
  write_val<unsigned int>(out, m.size());

  for (typename std::unordered_map<T, std::vector<U> >::const_iterator it = 
	 m.begin();
       it != m.end();
       ++it)
    {
      write_val<T>(out, it->first);
      write_vector<U>(out, it->second);
    }
}

template<class T, class U, class V> std::unordered_map<T, std::unordered_map<U, V> > &
read_map(std::istream &in,
	 std::unordered_map<T, std::unordered_map<U, V> > &m,
	 bool reverse_bytes)
{
  unsigned int size = read_val<unsigned int>(in, reverse_bytes);

  for (unsigned int i = 0; i < size; ++i)
    {
      T t = read_val<T>(in, reverse_bytes);

      std::unordered_map<U, V> mm;
      read_map<U, V>(in, mm, reverse_bytes);

      m[t] = mm;
    }

  return m;
}

template<class T, class U, class V> 
void write_map(std::ostream &out,
	       std::unordered_map<T, std::unordered_map<U, V> > &m)
{
  write_val<unsigned int>(out, m.size());

  for (typename std::unordered_map<T, std::vector<U> >::const_iterator it = 
	 m.begin();
       it != m.end();
       ++it)
    {
      write_val<T>(out, it->first);
      write_map<U, V>(out, it->second);
    }
}

#endif // HEADER_io_hh
