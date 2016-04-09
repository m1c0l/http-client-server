#ifndef HTTPMESSAGE
#define HTTPMESSAGE
#include <string>
#include <cstdlib>
#include <map>

using namespace std;

class HttpMessage{
 public:
  //HttpMessage();
  virtual void decodeFirstLine(string line) = 0;
  string getVersion();
  string setVersion(string ver);
  void setHeader(string key, string value);
  string getHeader(string key);
  void decodeHeaderLine(string line);
  void setPayLoad(string blob);
  string getPayload();

 private:
  string m_version;
  map<string, string> m_headers;
  string m_payload;

   ;
};



#endif
