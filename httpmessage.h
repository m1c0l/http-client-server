#ifndef HTTPMESSAGE
#define HTTPMESSAGE
#include <string>
#include <iostream>
#include <stdlib>


using namespace std;

class HttpMessage{
 public:
  float  m_version;
  map<string, string> m_headers;
  string m_payload;
  
float getversion();
void setHeader(string key, string value);
string getHeader(string key);
void decodeHeaderLine(string line);
void setPayLoad(string blob);
string getPayload();

 private:


   ;
};



#endif
