#include  <ArduinoSTL.h>
#include  <vector>
#include  <string>
#include  <iostream>

using namespace std;

void setup()
{
  Serial.begin( 115200UL );
  while( !Serial ) delay( 20UL );
  Serial.println( "\r\n\r\nAVR STL Test from Serial." );

  vector<int> v1;
  vector<string> v2;
  v1.push_back(1);
  v1.push_back(2);
  v1.push_back(3);
  v2.push_back("ABC");
  v2.push_back("DEF");
  unsigned int i;
  for (i = 0; i < v1.size(); i++)
  {
      cout << "v1[" << i << "]=" << v1[i] << endl;
  }
  for (i = 0; i < v2.size(); i++)
  {
      cout << "v2[" << i << "]=" << v2[i] << endl;
  }
}

void loop()
{
}
