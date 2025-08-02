#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <string>
#include <limits>

using namespace std;

template<class T>
class CacheManager {
  //members (private)
  int capacity;
  map<string, pair<T, int>> cache_data; 
  // <Clave, <Obj, Indice de Uso>>
  //,
  bool write_file (string, T);

public:
  CacheManager(int);
  ~CacheManager();
  
  void insert (string key, T obj);
  T get (string key);

};

template <class T>
CacheManager<T>::CacheManager(int cap) {
  capacity = cap;
}

template <class T>
CacheManager<T>::~CacheManager() {}
template <class T>
bool CacheManager<T>::write_file (string key, T obj) {
  ofstream outfile("cache_data.txt", ios::app);
  if(!outfile){
    cerr << "Error al abrir el archivo para escritura" << endl;
    return false;
  }

  outfile << key << " " << obj << "\n";

  outfile.close();
return true;
}
template <class T>
void CacheManager<T>::insert (string key, T obj) {
  // Si la clave existe, actualizamos
  auto it = cache_data.find(key);
  if (it != cache_data.end()) {
    it -> second.first = obj;
    it -> second.second = 0;
  } else {
    // si la cache esta llena, eliminamos el LRU
    if (cache_data.size() >= capacity) {
      string lru_key;
      int max_mru = -1;
      //buscamos el elemento con el MRU mas alto(el menos reciente)
      for (const auto& pair : cache_data) {
        if (pair.second.second > max_mru) {
          max_mru = pair.second.second;
          lru_key = pair.first;
        }
      }
      cache_data.erase(lru_key);
    }
    //insertamos el nuevo elemento con MRU = 0 (el mas reciente)
cache_data[key] = make_pair(obj, 0);
}
  // actualizaos el MRU de todos los elementos
  for (auto& pair : cache_data) {
    pair.second.second++;
  }

  write_file(key, obj);
}
template<class T>
T CacheManager<T>::get (string key) {
  //Busquea  en la cache
  auto it = cache_data.find(key);
  if (it != cache_data.end()) {
    int old_mru = it -> second.second;
    it -> second.second = 0; // el mas reciente
  
    for (auto& pair : cache_data) {
      if (pair.first != key && pair.second.second < old_mru) {
        pair.second.second++;
        }
      }
    return it -> second.first;
  }
// Busqueda en archivo
ifstream infile("cache_data.txt");
if (!infile){
    throw runtime_error("Archivo no encontrado");
}

string file_key;
  T found_obj;
while(infile >> file_key) {
    if(file_key == key){
      infile >> found_obj;
      insert(key, found_obj);
      return found_obj;
    }
    infile.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  throw runtime_error("Clave no encontrada");
}

