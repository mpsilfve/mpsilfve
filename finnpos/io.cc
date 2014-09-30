#include "io.hh"
#include "exceptions.hh"

#ifndef TEST_io_cc

Entry::Entry(void) {}

void split(const std::string &str, StringVector &target, char delim)
{
  size_t old_delim_pos = -1;
  size_t new_delim_pos = str.find(delim, old_delim_pos + 1);

  do
    {
      std::string field = str.substr(old_delim_pos + 1, 
				     new_delim_pos - old_delim_pos - 1);

      target.push_back(field);

      old_delim_pos = new_delim_pos;
      new_delim_pos = str.find(delim, old_delim_pos + 1);
    }
  while (old_delim_pos != std::string::npos);
}

Entry get_next_line(std::istream &in)
{
  std::string line;
  std::getline(in, line);

  if (line.empty())
    { throw EmptyLine(); }

  StringVector fields;
  split(line, fields, '\t');

  Entry res;

  if (fields.size() != 5)
    { 
      throw SyntaxError();
    }

  for (unsigned int i = 0; i < fields.size(); ++i)
    {
      if (fields[i].empty())
	{
	  throw SyntaxError();
	}
    }

  res.token = fields.at(0);

  std::string feat_template_string = fields.at(1);
  split(feat_template_string, res.feat_templates, ' ');

  if (fields.at(2) == "_")
    { 
      res.lemma = "";
    }
  else
    { 
      res.lemma = fields.at(2); 
    }

  if (fields.at(3) != "_")
    {
      std::string label_string = fields.at(3);
      split(label_string, res.labels, ' ');      
    } 

  res.annotations = fields.at(4);

  return res;
}

#else // TEST_io_cc

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

int main(void)
{
  // Empty input string.
  // Splitting gives ("")
  // get_next_line throws EmptyLine.

  std::string str1;

  StringVector fields1;
  split(str1, fields1, '\t');
  assert(fields1.size() == 1);

  std::istringstream in1(str1);

  try
    {
      Entry entry = get_next_line(in1);
      static_cast<void>(entry); 

      assert(0);
    }
  catch (const EmptyLine &e)
    { 
      /* EXPECTED FAIL */ 
    }

  // Two field string "foo\tbar".
  // Splitting gives ("foo", "bar")
  // get_next_line throws SyntaxError due to
  // insufficient firld count.

  std::string str2 = "foo\tbar";

  StringVector fields2;
  split(str2,fields2,'\t');

  assert(fields2.size() == 2);
  assert(fields2[0] == "foo");
  assert(fields2[1] == "bar");

  std::istringstream in2(str2);

  try
    {
      Entry entry = get_next_line(in2);
      static_cast<void>(entry); 

      assert(0);
    }
  catch (const SyntaxError &e)
    { 
      /* EXPECTED FAIL */ 
    }

  // Five field string "foo\tbar\tfoo\tbar\tfoo".
  // Splitting gives ("foo", "bar", "foo", "bar", "foo")
  // get_next_line gives Entry with
  // token == "foo"
  // feat_templates == ("bar")
  // lemma == "foo"
  // labels == ("bar")
  // annotations == foo

  std::string str3 = "foo\tbar\tfoo\tbar\tfoo";

  StringVector fields3;
  split(str3,fields3,'\t');

  assert(fields3.size() == 5);
  assert(fields3[0] == "foo");
  assert(fields3[1] == "bar");
  assert(fields3[2] == "foo");
  assert(fields3[3] == "bar");
  assert(fields3[4] == "foo");

  std::istringstream in3(str3);

  try
    {
      Entry entry = get_next_line(in3);

      assert(entry.token == "foo");
      assert(entry.feat_templates.size() == 1);
      assert(entry.feat_templates[0] == "bar");
      assert(entry.lemma == "foo");
      assert(entry.labels.size() == 1);
      assert(entry.labels[0] == "bar");
      assert(entry.annotations == "foo");
    }
  catch (const EmptyLine &e)
    { 
      assert(0);
    }
  catch (const SyntaxError &e)
    {
      assert(0);
    }

  // Five field string "foo\tbar baz\tfoo\tbar quux\tfoo".
  // Splitting gives ("foo", "bar baz", "foo", "bar quux", "foo")
  // get_next_line gives Entry with
  // token == "foo"
  // feat_templates == ("bar", "baz")
  // lemma == "foo"
  // labels == ("bar", "quux")
  // annotations == foo

  std::string str4 = "foo\tbar baz\tfoo\tbar quux\tfoo";

  StringVector fields4;
  split(str4,fields4,'\t');

  assert(fields4.size() == 5);
  assert(fields4[0] == "foo");
  assert(fields4[1] == "bar baz");
  assert(fields4[2] == "foo");
  assert(fields4[3] == "bar quux");
  assert(fields4[4] == "foo");

  std::istringstream in4(str4);

  try
    {
      Entry entry = get_next_line(in4);

      assert(entry.token == "foo");
      assert(entry.feat_templates.size() == 2);
      assert(entry.feat_templates[0] == "bar");
      assert(entry.feat_templates[1] == "baz");
      assert(entry.lemma == "foo");
      assert(entry.labels.size() == 2);
      assert(entry.labels[0] == "bar");
      assert(entry.labels[1] == "quux");
      assert(entry.annotations == "foo");
    }
  catch (const EmptyLine &e)
    { 
      assert(0);
    }
  catch (const SyntaxError &e)
    {
      assert(0);
    }

  // Five field string "foo\tbar\t_\t_\t_".
  // get_next_line gives Entry with
  // token == "foo"
  // feat_templates == ("bar")
  // lemma == ""
  // labels == ()
  // annotations == "_"

  std::string str5 = "foo\tbar\t_\t_\t_";

  std::istringstream in5(str5);

  try
    {
      Entry entry = get_next_line(in5);

      assert(entry.token == "foo");
      assert(entry.feat_templates.size() == 1);
      assert(entry.feat_templates[0] == "bar");
      assert(entry.lemma == "");
      assert(entry.labels.size() == 0);
      assert(entry.annotations == "_");
    }
  catch (const EmptyLine &e)
    { 
      assert(0);
    }
  catch (const SyntaxError &e)
    {
      assert(0);
    }

}

#endif // TEST_io_cc
