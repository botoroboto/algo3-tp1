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
vector<int> MA(N+1, UNDEFINED_VALUE);

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

vector<int> reconstruccion(vector<int> MA) {
    vector<int> res = {};
    for (int i = 0; i < MA.size() - 1; ++i) {
        if (MA[i] - MA[i+1] != 0) {
            res.push_back(i);
        }
    }

    if (MA[MA.size() -1] != 0){
        res.push_back(MA.size() -1);
    }

    return res;
}

int b_BOTTOM_UP(int i) {
    int ultimoS = UNDEFINED_VALUE;
    vector<int> M_BOTTOM_UP(N, 0);
    for (int j = N-1; j >= 0; --j) {
        bool ponerJesOptimo = ultimoS == UNDEFINED_VALUE || ultimoS > T[j];
        int k = j - 1;
        int ultimo = 0;
        if (j != N-1) {
            ultimo = M_BOTTOM_UP[j+1];
        }
        if (ponerJesOptimo){
            while (k > 0 && T[k] > S[j] && ponerJesOptimo) {
                if (ultimo + B[k] > ultimo + B[j]) {
                    ponerJesOptimo = false;
                }
                k--;
            }
        }

        if (ponerJesOptimo) {
            ultimoS = S[j];
            M_BOTTOM_UP[j] = ultimo + B[j];
        }else {
            M_BOTTOM_UP[j] = ultimo;
        }
    }

    return M_BOTTOM_UP[N-i];
}



int b_TOP_DOWN_2(int i, int ultimoT) {
    if (i==N) {
        MA[i] = 0;
    } else {
        if (ultimoT>=S[i]) {
            MA[i] = b_TOP_DOWN_2(i+1, ultimoT);
        } else if (MA[i] == UNDEFINED_VALUE) {
            int loAgrego =  b_TOP_DOWN_2(i+1, T[i]) + B[i];
            int noLoAgrego = b_TOP_DOWN_2(i+1, ultimoT);

            MA[i] = max(loAgrego, noLoAgrego);
        }
    }

    return MA[i];
}

resultado b_TOP_DOWN(int i, vector<actividad> actividadesAlcanzadas) {
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
            M[i][ultimoT] = b_TOP_DOWN(i+1, actividadesAlcanzadas);
        }else if (M[i][ultimoT].res == undefinedValue.res) {
            actividad nueva;
            nueva.s = S[i];
            nueva.t = T[i];
            resultado agregandolo = b_TOP_DOWN(i+1, actividadesAlcanzadas);
            vector<actividad> copia = actividadesAlcanzadas;
            copia.push_back(nueva);
            resultado noAgregandolo = b_TOP_DOWN(i+1, copia);
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

void printMatriz(std::vector<std::vector<resultado> > m) {
    for( int i=0; i<m.size() ; i++)
    {
        for( int j=0; j<m[0].size(); j++)
        {
            std::cout<<m[i][j].res<<"   ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
}

// TODO - Parsear desde un file
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
    // resultado res = b_TOP_DOWN(0, inicial);
    // cout << res.res << endl;

    int a = b_TOP_DOWN_2(0, 0);
    cout << a << " TOP DOWN" << endl;

    vector<int> e = reconstruccion(MA);
    cout  << "--reconstruccion--"<< endl;

    for (int i = 0; i < e.size(); ++i) {
        cout << e[i] << endl;
    }
    cout  << "-----------"<< endl;

    int bottomUp = b_BOTTOM_UP(N);
    cout << bottomUp << " BOTTOM UP"<< endl;

    // for (int i = 0; i < res.actividades.size(); ++i) {
       // cout << "S: " << res.actividades[i].s << "  T: " << res.actividades[i].t << endl;
    // }

    // printMatriz(M);

    cout << "Tiempo de ejecucion: " << total_time << " ms" << endl;


//    for (actor a : res.getActores()) {
//        cout << a.id << " ";
//    }
    cout << endl;
    return 0;
}