#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <iterator>
#include "./radix_sort.h"

using namespace std;

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

void beneficio_actividades(vector<pair<pair<int, int>, int>>& actividades, pair<int, vector<int>>& res){
    int indice = 0;
    res.first = 1;
    res.second.push_back(actividades[0].second);

    for (int i = 1; i < actividades.size(); ++i) {
        //compara el inicio de la actividad que esta siendo iterada con el final de la ultima actividad agregada
        if(actividades[i].first.first > actividades[indice].first.second){
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

    pair<int, vector<int>> soluciones_1;
    vector<pair<pair<int, int>, int>> actividades_1 = radix_sort_modificado(instancia_1);
    beneficio_actividades(actividades_1, soluciones_1);

    cout << soluciones_1.first << endl;
    for (int posicion : soluciones_1.second) {
        cout << posicion << ", ";
    }

    pair<int, vector<int>> soluciones_2;
    vector<pair<pair<int, int>, int>> actividades_2 = radix_sort_modificado(instancia_2);
    beneficio_actividades(actividades_2, soluciones_2);

    cout << soluciones_2.first << endl;
    for (int posicion : soluciones_2.second) {
        cout << posicion << ", ";
    }

    return 0;
}