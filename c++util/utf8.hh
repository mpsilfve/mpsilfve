#include <string>
#include <vector>

typedef std::vector<std::string> StringVector;

struct InvalidUtf8
{};

void byte_str_to_utf_8(const std::string &str,
		       StringVector &v);
