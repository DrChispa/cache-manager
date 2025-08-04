#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <utility>

using namespace std;

template <class T> class CacheManager {
  // members (private)
private:
  size_t capacity;
  map<string, pair<T, int>> cache_data;
  int current_mru = 0;
  bool write_file(string, T);

public:
  CacheManager(int);
  ~CacheManager();

  void insert(string key, T obj);
  T get(string key);
  void show_cache() const;
};

template <class T> CacheManager<T>::CacheManager(int cap) { capacity = cap; }

template <class T> CacheManager<T>::~CacheManager() {}

template <class T>
bool CacheManager<T>::write_file(const string key, const T obj) {

  // actualizamos la el objeto dentro de la funcion
  map<string, T> file_content;
  ifstream infile("cache_data.txt");
  string current_key;
  T current_obj;

  while (infile >> current_key >> current_obj) {
    file_content[current_key] = current_obj;
  }
  infile.close();

  // insertar nuevo elemento
  file_content[key] = obj;
  ofstream outfile("cache_data.txt");
  if (!outfile) {
    cerr << "Error al abrir el archivo para escritura" << endl;
    return false;
  }

  for (const auto &item : file_content) {
    outfile << item.first << " " << item.second << endl;
  }

  outfile.close();
  return true;
}

template <class T> void CacheManager<T>::insert(string key, T obj) {
  // Si la clave existe, actualizamos
  auto it = cache_data.find(key);
  if (it != cache_data.end()) {
    it->second.first = obj;
    it->second.second = ++current_mru;
  } else {
    // si la cache esta llena, eliminamos el LRU
    if (cache_data.size() >= capacity) {
      auto lru_it = cache_data.begin();

      // buscamos el elemento con el MRU mas alto(el menos reciente)
      for (auto it = cache_data.begin(); it != cache_data.end(); ++it) {
        if (it->second.second < lru_it->second.second) {
          lru_it = it;
        }
      }
      cache_data.erase(lru_it);
    }
    cache_data[key] = make_pair(obj, ++current_mru);
  }
  // insertamos el nuevo elemento con MRU = 0 (el mas reciente)
  write_file(key, obj);
}

template <class T> T CacheManager<T>::get(string key) {
  // Busquea  en la cache
  auto it = cache_data.find(key);
  if (it != cache_data.end()) {
    it->second.second = ++current_mru;
    return it->second.first; // el mas reciente
  }
  // Busqueda en archivo
  ifstream file("cache_data.txt");
  if (!file) {
    throw runtime_error("Archivo no encontrado");
  }

  string file_key;
  T found_obj;
  while (file >> file_key) {
    if (file_key == key) {
      file >> found_obj;
      return found_obj;
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  throw runtime_error("Clave no encontrada");
}

template <class T> void CacheManager<T>::show_cache() const {
  cout << "\n=== Contenido de la Cache ===" << endl;
  cout << "Capacidad: " << capacity << endl;
  cout << "Indice | Dato | MRU/LRU\n";
  cout << "---------------------------" << endl;

  for (const auto &item : cache_data) {
    cout << item.first << " | ";
    item.second.first.print();
    cout << " | " << item.second.second << endl;
    cout << "---------------------------" << endl;
  }
}
