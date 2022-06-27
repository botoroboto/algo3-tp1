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
vector<int> S = {};  // tiempo inicio
vector<int> T = {};  // tiempo fin
vector<int> B = {};  //beneficio
const int UNDEFINED_VALUE = -1;
vector<int> memoization = {};
vector<int> actividadSiguiente;

struct actividad {
    int inicio;
    int fin;
    int beneficio;
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

vector<actividad> ordenadoPorInicio = {};
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
        else if(ordenadoPorInicio[inicio].inicio <= ordenadoPorFin[fin].tarea.fin){
            inicio++;
        }else{
            res[ordenadoPorFin[fin].index] = inicio;
            fin++;
        }
    }
    return res;
}

vector<int> reconstruccion(int j, vector<int> &res) {
    if (j==N) {
        return {};
    }else if(ordenadoPorInicio[j].beneficio + memoization[actividadSiguiente[j]] > memoization[j+1]) {
        res.push_back(j);
        cout << j <<  " " << endl;
        reconstruccion(actividadSiguiente[j], res);
    } else {
        reconstruccion(j + 1, res);
    }

    return res;

}

int b_BOTTOM_UP() {
    memoization[N] = 0;

    for (int j=N-1; j >= 0; j--) {
        memoization[j] = max(ordenadoPorInicio[j].beneficio + memoization[actividadSiguiente[j]], memoization[j+1]);
    }

    return memoization[0];
}

int b_TOP_DOWN(int i) {
    if (i==N) {
        memoization[i] = 0;
    } else if (memoization[i] == UNDEFINED_VALUE ) {
        int loAgrego =  b_TOP_DOWN(actividadSiguiente[i]) + B[i];
        int noLoAgrego = b_TOP_DOWN(i + 1);

        memoization[i] = max(loAgrego, noLoAgrego);
    }

    return memoization[i];
}


// TODO - Parsear desde un file
int main() {
    cin >> N;
    B.resize(N);
    S.resize(N);
    T.resize(N);

    for (int i = 0; i < N; ++i) {
        actividad actividadDummy;
        int v,w,q;
        cin >> v >> w >> q;
        actividadDummy.beneficio = q;
        B[i] = q;
        S[i] = v;
        T[i] = w;
        actividadDummy.fin = w;
        actividadDummy.inicio = v;
        ordenadoPorInicio.push_back(actividadDummy);
    }

    vector<vector<actividadConIndice>> buckets(2*N + 1, vector<actividadConIndice> {});
    for(int a = 0;  a < ordenadoPorInicio.size(); a++){
        actividadConIndice dummy;
        dummy.tarea = ordenadoPorInicio[a];
        dummy.index = a;
        buckets[ordenadoPorInicio[a].fin].push_back(dummy);
    }
    for(vector<actividadConIndice> bucket : buckets){
        for(actividadConIndice a : bucket){
            ordenadoPorFin.push_back(a);
        }
    }

    auto start = chrono::steady_clock::now(); // Empieza el clock

    
    vector<actividad> inicial= {};
    vector<vector<resultado>> M(N + 1, vector<resultado>(2*N + 1, undefinedValue));
    memoization.resize(N + 1, UNDEFINED_VALUE);
    actividadSiguiente.resize(2*N, UNDEFINED_VALUE);

    actividadSiguiente = generarVectorDeSiguienteActividad();

    int a = b_TOP_DOWN(0);
    cout << endl;
    cout << endl;

    cout << "Resultado: "  << a << endl;

    // int bottomUp = b_BOTTOM_UP();
    // cout << bottomUp << " Bottom Up" << endl;
	
	auto end = chrono::steady_clock::now(); // Termina el clock
    double total_time = chrono::duration<double, milli>(end - start).count();

    cout << endl;
    vector<int> res = {};
    vector<int> r = reconstruccion(0, res);

    cout << "Tiempo de ejecucion: " << total_time << " ms" << endl;

    cout << endl;
    return 0;
}