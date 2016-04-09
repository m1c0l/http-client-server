#include "HttpMessage.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

string HttpMessage::getVersion(){
  return m_version;
}

void HttpMessage::setVersion(string ver){
  m_version = ver;
}

void HttpMessage::setHeader(string key, string value)
{
  m_headers[key] = value;
  return;
}

string HttpMessage::getHeader(string key)
{
  return m_headers[key];
}

void HttpMessage::decodeHeaderLine(string line)
//May need error checking if header line does not end with carriage return
{
  string key, value;
  int i=0;
  while(line[i]!= ':')
    {
      key += line[i];
      i++;
    }
  i++;
  while(line[i] == ' ')
    i++;
  while(line[i] != '\r' && line[i+1] != '\n')
    {
      value += line[i];
    }
  setHeader(key, value);
  return;
}

void HttpMessage::setPayLoad(string blob)
{
  m_payload = blob;
  return;
}

string HttpMessage::getPayload()
{
  return m_payload;
}
