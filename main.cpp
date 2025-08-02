#include "cache.h"
#include <string>
#include <iostream>

using namespace std;

class Student {
  int id;
  int value;
  const char* data;
public:
  static const string class_name;

  Student(int _key, int _value, const char* _data): id (_key),value (_value),data (_data) {}
  void print(){
    cout << "Student Object: " << id << ", " << value << ", " << data << endl;
  }
  ~Student () {};
};

const string Student::class_name = "StudentClass";

int main () {
  CacheManager<Student> my_cache(3);

  my_cache.insert("0", Student(0, 22, "student1"));
  cout << "todo funka piola" << endl;
  return 0;
}
