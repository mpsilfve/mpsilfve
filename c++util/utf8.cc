#include "utf8.hh"

#ifndef TEST_utf8_cc

void byte_str_to_utf_8(const std::string &str,
		       StringVector &v)
{  
  int i = 0;

  while(i < str.size())
    {
      if (( str[i] & 0x80 ) == 0 )
	{ 
	  v.push_back(str.substr(i,1));
	  i += 1;
	}
      else if (( str[i] & 0xE0 ) == 0xC0 )
	{ 
	  v.push_back(str.substr(i,2));
	  i += 2;
	}
      else if (( str[i] & 0xF0 ) == 0xE0 )
	{ 
	  v.push_back(str.substr(i,3));
	  i += 3;
	}
      else if (( str[i] & 0xF8 ) == 0xF0 )
	{ 
	  v.push_back(str.substr(i,4));
	  i += 4;
	}
      else
	{ throw InvalidUtf8(); }
    }
}

#else // TEST_utf8_cc

#include <cassert>
#include <iostream>

int main(void)
{
  StringVector v;

  byte_str_to_utf_8("this is a ÄÅÖ äöå Î û ffgt.öl", v);
  const char * arr[] = {"t", "h", "i", "s", " ", "i", "s", " ", 
			"a", " ", "Ä", "Å", "Ö", " ", "ä", "ö", 
			"å", " ", "Î", " ", "û", " ", "f", "f" ,
			"g", "t", ".", "ö", "l"};

  for (int i = 0; i < 21; ++i)
    {
      assert(arr[i] == v[i]);
      std::cout << arr[i] << " " << v[i] << std::endl;
    }
}

#endif // TEST_utf8_cc
