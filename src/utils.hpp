
#include <string>
#include <vector>

using namespace std;
void my_split(const string& s, const string& c, vector<string>& v)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  v.clear();
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

float Max(float a, float b) {
    return (a > b ? a : b);
}
int Min(int a, int b) {
    return (a < b ? a: b);
}

