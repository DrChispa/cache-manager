#include "cache.h"
#include <string>
#include <iostream>

using namespace std;

class Student {
  int id;
  int value;
  string data;
public:
  static const string class_name;

  Student(int _key, int _value, const char* _data): id (_key),value (_value),data (_data) {}
  void print(){
    cout << "Student Object: " << id << ", " << value << ", " << data << endl;
  }
  friend ostream& operator<<(ostream& os, const Student& a) { 
    os << "Student Object: " << a.id << ", " << a.value << ", " << a.data << endl;
    return os;
  }
  friend istream& operator>>(istream& is, Student& a) {
    static char buffer[256];
    is >> a.id >> a.value >> buffer;
    a.data = buffer;
    return is;
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
