///////////////////////////////////////////////////////////////////////////////
// @file    io.hh                                                            //
// @Author  Miikka Silfverberg                                               //
// @brief   Helpers for io.                                                  //
///////////////////////////////////////////////////////////////////////////////

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
