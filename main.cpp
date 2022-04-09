#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

int N = 0;
int N_INDEX = 3;
int M = 0;
int M_INDEX = 4;
string ID_ACTOR = "n";
string ID_AMISTAD = "e";

int main(int argc, char *argv[]) {
  if (argc < 5) {
    cerr << "Parámetros faltantes: " << endl << "Cantidad de actores y amistades" << endl;
    return 1;
  }

  N = stoi(argv[N_INDEX]);
  M = stoi(argv[M_INDEX]);
  if (N == 0) {
    cout << "Los actores no pueden ser 0" << endl; 
    return 1;
  }

  for (int i = M_INDEX + 1; i < argc; i += 3) {
    if (argv[i] == ID_ACTOR) {
      cout << "Actor: " << stoi(argv[i+1]) << " - Influencia: " << stoi(argv[i+2]) << endl;
    } else if (argv[i] == ID_AMISTAD) {
      cout << "Actor " << stoi(argv[i+1]) << " es amigo de Actor " << stoi(argv[i+2]) << endl;
    }
  };
  return 0;
}
