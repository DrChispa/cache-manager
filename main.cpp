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
 // Constructor por default
Student() : id(0), value(0), data("") {}

  Student(int _key, int _value, const char * _data): id (_key),value (_value),data (_data) {}
  
  friend ostream& operator<<(ostream& os, const Student& a) { 
   return os << a.id << " " << a.value << " " << a.data;
  }
  friend istream& operator>>(istream& is, Student& a) {
   return is >> a.id >> a.value >> a.data;
  }
  void print() const {
    cout << data; 
  }
  ~Student () {};
};

const string Student::class_name = "StudentClass";

int main () {
  CacheManager<Student> my_cache(3);

  my_cache.insert("a", Student(0,22,"Student1"));
  my_cache.show_cache();
  my_cache.insert("b", Student(1,33,"Student2"));
  my_cache.show_cache();
  my_cache.insert("c", Student(2,44,"Student3"));
  my_cache.show_cache();
  my_cache.insert("h", Student(0,22,"Student1"));
  my_cache.show_cache();
  my_cache.insert("f", Student(3,55,"Student4"));

  my_cache.show_cache();

  cout << "---------------------UPDATE-----------------" << endl;
  my_cache.insert("2", Student(2,222,"EZE"));
  my_cache.show_cache();

  cout << "---------------------GET-----------------" << endl;
  Student return_obj = my_cache.get("a");
 return 0;
}
