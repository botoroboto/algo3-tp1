#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <iterator>
#include "../../common/clique.h"
using namespace std;

int FILE_INDEX = 1;
int SORT_INDEX = 2;
int DEBUG_INDEX = 3;
int N;
const int UNDEFINED_VALUE = -1;
vector<int> actividadSiguiente;
vector<int> resGreedy;

struct actividad {
    int inicio;
    int fin;
};
struct actividadConIndice{
    actividad tarea;
    int index;
};
struct resultado {
    int res = -1;
    vector<actividad> actividades = {};
};
resultado undefinedValue;

vector<actividadConIndice> actividades = {};
vector<actividadConIndice> ordenadoPorInicio = {};
vector<actividadConIndice> ordenadoPorFin= {};

vector<int> generarVectorDeSiguienteActividad(){
    vector<int> res(N , N);
    int inicio = 0;
    int fin = 0;
    while(inicio<N || fin<N){
        if(inicio==N){
            while(fin<N) {
                res[ordenadoPorFin[fin].index] = N;
                fin++;
            }
        }
        else if(ordenadoPorInicio[inicio].tarea.inicio <= ordenadoPorFin[fin].tarea.fin){
            inicio++;
        }else{
            res[ordenadoPorFin[fin].index] = inicio;
            fin++;
        }
    }
    return res;
}

vector<int> greedy() {
    vector<int> res = {};
    int k=0;
    res.push_back(ordenadoPorFin[k].index);
    for (int j = 1; j <= N ; ++j) {
        if(ordenadoPorFin[j].tarea.inicio > ordenadoPorFin[k].tarea.fin) {
            res.push_back(ordenadoPorFin[j].index);
            k = j;
        }

    }

    return res;
}


// TODO - Parsear desde un file
int main() {
    cin >> N;
    for (int i = 0; i < N; ++i) {
        actividadConIndice actividadDummy;
        int v,w;
        cin >> v >> w;
        actividadDummy.tarea.inicio = v;
        actividadDummy.tarea.fin = w;
        actividadDummy.index = i;
        actividades.push_back(actividadDummy);
    }

    vector<vector<actividadConIndice>> bucketsFin(2 * N + 1, vector<actividadConIndice>{});
    vector<vector<actividadConIndice>> bucketsInicio(2 * N + 1, vector<actividadConIndice>{});


    for (int a = 0; a < actividades.size(); a++) {
        actividadConIndice dummy;
        dummy.tarea = actividades[a].tarea;
        dummy.index = actividades[a].index;
        bucketsFin[actividades[a].tarea.fin].push_back(dummy);
        bucketsInicio[actividades[a].tarea.inicio].push_back(dummy);
    }
    for (vector<actividadConIndice> bucket : bucketsFin) {
        for (actividadConIndice a : bucket) {
            ordenadoPorFin.push_back(a);
        }
    }
    for (vector<actividadConIndice> bucket : bucketsInicio) {
        for (actividadConIndice a : bucket) {
            ordenadoPorInicio.push_back(a);
        }
    }
    if (N == 0) {
        cout << "Las actividades no pueden ser 0" << endl;
        return 1;
    }

    auto start = chrono::steady_clock::now(); // Empieza el clock
    
    vector<vector<resultado>> M(N + 1, vector<resultado>(2 * N + 1, undefinedValue));
    actividadSiguiente.resize(2 * N, UNDEFINED_VALUE);

    actividadSiguiente = generarVectorDeSiguienteActividad();
    resGreedy = greedy();
	
	auto end = chrono::steady_clock::now(); // Termina el clock
    double total_time = chrono::duration<double, milli>(end - start).count();
    cout << endl;

    cout << "Resultado: "<<endl;
    cout << resGreedy.size() <<endl;

    for (int i = 0; i < resGreedy.size(); ++i) {
        cout << resGreedy[i] << "  ";
    }

    cout << endl;
    cout << endl;

    cout << "Tiempo de ejecucion: " << total_time << " ms" << endl;

    cout << endl;
    return 0;
}