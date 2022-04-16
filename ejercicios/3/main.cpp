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
int N = 4;
vector<int> S = {1,1,2,3};  // tiempo inicio
vector<int> T = {2,3,4,6};  // tiempo fin
vector<int> B = {2,3,2,4};  //beneficio
const int UNDEFINED_VALUE = -1;
struct actividad {
    int s;
    int t;
};
struct resultado {
    int res = -1;
    vector<actividad> actividades = {};
};
resultado undefinedValue;
vector<vector<resultado>> M(N + 1, vector<resultado>(2*N + 1, undefinedValue));


resultado beneficioMaximo(int i, vector<actividad> actividadesAlcanzadas) {
    int ultimoT = 0;
    if (actividadesAlcanzadas.size() != 0) {
        ultimoT = actividadesAlcanzadas[actividadesAlcanzadas.size() - 1].t;
    }

    if (i==N) {
        resultado base;
        base.res = 0;
        base.actividades = actividadesAlcanzadas;
        M[i][ultimoT] = base;
    } else {
        if (ultimoT>=S[i]) {
            M[i][ultimoT] = beneficioMaximo(i+1, actividadesAlcanzadas);
        }else if (M[i][ultimoT].res == undefinedValue.res) {
            actividad nueva;
            nueva.s = S[i];
            nueva.t = T[i];
            resultado agregandolo = beneficioMaximo(i+1, actividadesAlcanzadas);
            vector<actividad> copia = actividadesAlcanzadas;
            copia.push_back(nueva);
            resultado noAgregandolo = beneficioMaximo(i+1, copia);
            noAgregandolo.res = noAgregandolo.res + B[i];
            if (noAgregandolo.res > agregandolo.res) {
                M[i][ultimoT] = noAgregandolo;
            }else {
                M[i][ultimoT] = agregandolo;
            }
        }
    }

    return M[i][ultimoT];
}

// TODO - Parsear desde un file
int main(int argc, char *argv[]) {

//    ios::sync_with_stdio(false);
//    cin.tie(0);
//    if (argc < 1) {
//        cerr << "Parametro faltante: " << endl << "Path al archivo de instancia." << endl;
//        return 1;
//    }
//
//    ifstream archivo_instancia;
//
//    archivo_instancia.open(argv[FILE_INDEX]);
//    string linea_instancia;
//
//    if (archivo_instancia.is_open()) {
//        while (archivo_instancia.good()) {
//            getline(archivo_instancia, linea_instancia);
//            N = stoi(linea_instancia);
//            cout << "n:" << N << endl;
//            while (getline(archivo_instancia, linea_instancia, '\0')) {
//                S.push_back(linea_instancia[0]);
//                T.push_back(linea_instancia[1]);
//                B.push_back(linea_instancia[2]);
//            }
//            archivo_instancia.close();
//        }
//    }

//    if (N == 0) {
//        cout << "Los actores no pueden ser 0" << endl;
//        return 1;
//    }

//    if (debug) {
//        cout << "Se leyeron " << N << " actores y " << M << " amistades." << endl;
//        cout << "Procesando..." << endl;
//    }

//    //Sort the actor list;
//    if (SORT_FLAG == SORT_ASCENDING) {
//        sort(Actores.begin(), Actores.end(), &actorAsc);
//    }else{
//        sort(Actores.begin(), Actores.end(), &actorDesc);
//    }

    //particionesIndependientesGlobal = generarGruposIndependientesMaximizandoActoresPorGrupo(Actores);

    // Funcion BT Con Poda Golosa EJ2
    auto start = chrono::steady_clock::now(); // Empieza el clock

    auto end = chrono::steady_clock::now(); // Termina el clock
    double total_time = chrono::duration<double, milli>(end - start).count();
    vector<actividad> inicial= {};
    resultado res = beneficioMaximo(0, inicial);
    cout << res.res << endl;
    for (int i = 0; i < res.actividades.size(); ++i) {
        cout << "S: " << res.actividades[i].s << "  T: " << res.actividades[i].t << endl;

    }
    cout << "Tiempo de ejecucion: " << total_time << " ms" << endl;


//    for (actor a : res.getActores()) {
//        cout << a.id << " ";
//    }
    cout << endl;
    return 0;
}