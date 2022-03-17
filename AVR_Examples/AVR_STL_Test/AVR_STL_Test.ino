#include  <ArduinoSTL.h>
#include  <iostream>
#include  <string>
//#include  <set>
#include  <time.h>

using namespace std;

void setup()
{
  Serial.begin( 115200UL );
  while( !Serial ) delay( 20UL );
  Serial.println( "\r\n\r\nAVR STL Test from Serial." );

  struct tm ot;  // 1999-09-16T12:41:54Z
  ot.tm_year = 1999 - 1900;
  ot.tm_mon = 9 - 1;
  ot.tm_mday = 16;
  ot.tm_hour = 12;
  ot.tm_min = 41;
  ot.tm_sec = 54;

#if 0
  cout << "old date time : " << t.tm_year + 1900 << "/" <<  t.tm_mon + 1 << "/" << t.tm_mday << " ";
  cout << t.tm_hour << ":" <<  t.tm_min << ":" << t.tm_sec << endl;
#else
  Serial.print( "old date time : " );
  Serial.print( ot.tm_year + 1900, DEC ); Serial.print("/");
  Serial.print( ot.tm_mon + 1, DEC );     Serial.print("/");
  Serial.print( ot.tm_mday , DEC );       Serial.print(" ");
  Serial.print( ot.tm_hour , DEC );       Serial.print(":");
  Serial.print( ot.tm_min , DEC );        Serial.print(":");
  Serial.print( ot.tm_sec , DEC );        Serial.println();
#endif
  time_t illegalTime = mktime( &ot );
  illegalTime += (time_t)(7 * 1024 * 24 * 3600L);  // 1024 week day

  struct tm *nt;  // 2019-05-05T22:11:56Z ???
  nt = localtime( &illegalTime );
#if 0
  cout << "new date time : " << correct_t->tm_year + 1900 << "/" <<  correct_t->tm_mon + 1 << "/" << correct_t->tm_mday << " ";
  cout << correct_t->tm_hour << ":" <<  correct_t->tm_min << ":" << correct_t->tm_sec << endl;
#else
  Serial.print( "new date time : " );
  Serial.print( nt->tm_year + 1900 , DEC ); Serial.print("/");
  Serial.print( nt->tm_mon + 1 , DEC );     Serial.print("/");
  Serial.print( nt->tm_mday , DEC );        Serial.print(" ");
  Serial.print( nt->tm_hour , DEC );        Serial.print(":");
  Serial.print( nt->tm_min , DEC );         Serial.print(":");
  Serial.print( nt->tm_sec , DEC );         Serial.println();
#endif
  Serial.println( ctime( illegalTime ) );



#if 0
  set<string> names;  // set のデータ構造を用意する。
  //  あたいを代入
  names.insert("Tom");
  names.insert("Mike");
  names.insert("Mike");   //  同じ名前をダブって代入させる
  names.insert("Bob");
  //  登録されている全データを表示
  set<string>::iterator it; //  イテレータを用意
  for(it = names.begin() ; it != names.end(); it++)
  {
    cout << *it << endl;
  }
  //  Bob,Steveがデータ内に存在するか調べる
  string n[] = {"Bob","Steve"};
  int i;
  for(i = 0; i < 2;i++)
  {
    it = names.find(n[i]);
    if(it == names.end())
    {
      //  データが、set内に存在しなしい
      cout << n[i] << " is not in a set." << endl;
    }
    else
    {
      //  データがset内に存在する。
      cout << n[i] << " is in a set." << endl;
    }
  }
#endif
}

void loop()
{
}
