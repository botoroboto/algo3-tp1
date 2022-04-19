#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <iterator>
#include "./radix_sort.h"

using namespace std;

int FILE_INDEX = 1;
int SORT_INDEX = 2;
int DEBUG_INDEX = 3;
int N;
vector<int> S = {};  // tiempo inicio
vector<int> T = {};  // tiempo fin
vector<int> B = {};  //beneficio
const int UNDEFINED_VALUE = -1;


//TODO levantar del archivo
vector<pair<int, int>> instancia_1 = {
        make_pair(4, 5),
        make_pair(3, 7),
        make_pair(0, 3),
        make_pair(2, 8),
        make_pair(6, 10)
};

vector<pair<int, int>> instancia_2 = {
        make_pair(6, 10),
        make_pair(1, 5),
        make_pair(8, 10),
        make_pair(7, 9),
        make_pair(2, 5),
        make_pair(3, 4),
        make_pair(5, 9)
};

void beneficio_actividades(vector<pair<actividad, int>>& actividades, pair<int, vector<int>>& res){
    int indice = 0;
    res.first = 1;
    res.second.push_back(actividades[0].second);

    for (int i = 1; i < actividades.size(); ++i) {
        //compara el inicio de la actividad que esta siendo iterada con el final de la ultima actividad agregada
        if(actividades[i].first.s > actividades[indice].first.t){
            res.first++;
            res.second.push_back(actividades[i].second);
            indice = i;
        }
    }
    //return res;
}
/*TODO
 * IDEA de implementacion para radix_sort_modificado (sin terminar aun)
 * Usar un algoritmo de sorting tipo radix-sort con bucket-sort en la entrada ordenando por los horarios de finalizacion de menor a mayor:
    - Al recorrer el arreglo y separar en buckets, guardar el indice de la posicion original que tenía cada elemento en el arreglo original
    - Ordenar de esta forma costaría O(n) ya que se recorre una unica vez el arreglo.
 */
int main(int argc, char *argv[]) {

    ios::sync_with_stdio(false);
    cin.tie(0);
    if (argc < 1) {
        cerr << "Parametro faltante: " << endl << "Path al archivo de instancia." << endl;
        return 1;
    }
    ifstream archivo_instancia;

    archivo_instancia.open(argv[FILE_INDEX]);
    string linea_instancia;

    if (archivo_instancia.is_open()) {
        while (archivo_instancia.good()) {
            getline(archivo_instancia, linea_instancia);
            N = stoi(linea_instancia);
            int number;
            cout << "n:" << N << endl;
            B.resize(N);
            S.resize(N);
            T.resize(N);
            int i = 1;
            while (archivo_instancia >> number) {
                if (i % 3 == 0) {
                    B[(i/3) - 1] = number;
                } else if (i % 3 == 2) {
                    T[i/3] = number;
                }else {
                    S[i/3] = number;
                }
                i++;
            }
            archivo_instancia.close();
        }}

    if (N == 0) {
        cout << "Los actores no pueden ser 0" << endl;
        return 1;
    }

    pair<int, vector<int>> soluciones_1;
    vector<pair<actividad, int>> actividades_1 = radix_sort_modificado(instancia_1);
    beneficio_actividades(actividades_1, soluciones_1);

    cout << soluciones_1.first << endl;
    for (int posicion : soluciones_1.second) {
        cout << posicion << ", ";
    }

    pair<int, vector<int>> soluciones_2;
    vector<pair<actividad, int>> actividades_2 = radix_sort_modificado(instancia_2);
    beneficio_actividades(actividades_2, soluciones_2);

    cout << soluciones_2.first << endl;
    for (int posicion : soluciones_2.second) {
        cout << posicion << ", ";
    }

    return 0;
}