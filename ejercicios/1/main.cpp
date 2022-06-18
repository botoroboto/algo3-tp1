#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include "../../common/clique.h"
#include <iterator>
#include <algorithm>

using namespace std;

int FILE_INDEX = 1;
int SORT_INDEX = 2;
int DEBUG_INDEX = 3;
string DEBUG_FLAG = "debug";
string SORT_ASCENDING = "asc";
string SORT_DESCENDING = "desc";
string SORT_FLAG = SORT_DESCENDING;

bool actorAsc (const actor& a, const actor& b){
    return  a.influencia < b.influencia;
}
bool actorDesc (const actor& a, const actor& b){
    return  a.influencia > b.influencia;
}

int N = 0;
int M = 0;
char ID_FIRST_LINE = 'p';
char ID_ACTOR = 'n';
char ID_AMISTAD = 'e';

vector<actor> Actores;
vector<vector<bool>> Amistades;

bool sonAmigos(actor a1, actor a2) {
  return Amistades[a1.id][a2.id];
};

int influenciaMax = 0; // Counter para la influencia máxima, voy a ir actualizando con la mejor que tenga hasta el momento

clique cliqueMasInfluyenteBT2(clique cliqueActual, vector<actor> actoresRestantes, int influenciaParcial, int sumaRestante){
  // Caso base
  if (actoresRestantes.size() < 1) {
    cliqueActual.setInfluencia(influenciaParcial);
    if (influenciaMax < influenciaParcial) {
      influenciaMax = influenciaParcial;
    }
    return cliqueActual;
  }
 
  // Poda de optimalidad
  if (sumaRestante + influenciaParcial <= influenciaMax) {
    return cliqueActual; // No sigo recorriendo nada del arbol porque la suma ya no puede superar al max actual
  }

  bool sonTodosAmigos = false;

  for (int a = 0; a > actoresRestantes.size(); a++) {
      bool esAmigoDeTodos = true;
      for (actor a2 : actoresRestantes) {
          if (actoresRestantes[a].id < a2.id) {
              sonTodosAmigos = sonAmigos(actoresRestantes[a], a2);
              if (!sonTodosAmigos) {
                  esAmigoDeTodos = false;
                  break;
              }
          } else {
              break;
          }
      }
      if(esAmigoDeTodos){
          cliqueActual.addActor(actoresRestantes[a]);
          influenciaParcial += actoresRestantes[a].influencia;
          sumaRestante -= actoresRestantes[a].influencia;
          actoresRestantes.erase(actoresRestantes.begin() + a);
          a--;
      }
  }

  if (sonTodosAmigos) { // Esto significa que todos son amigos entre todos de K, por lo que podemos meterlos a todos al clique
      cliqueActual.setInfluencia(influenciaParcial);
      if (influenciaParcial > influenciaMax) {
          influenciaMax = sumaRestante + influenciaParcial;
      }
      return cliqueActual;
  }

  actor actorActual;
  actorActual.id = actoresRestantes[0].id;
  actorActual.influencia = actoresRestantes[0].influencia;
  actoresRestantes.erase(actoresRestantes.begin() + 0);

  vector<actor> nuevoVectorDeActoresRestantes = {};
  int nuevaSumaRestante = sumaRestante;
  for (actor a : actoresRestantes) {
    if (sonAmigos(a, actorActual)) {
      actor actorTemporal;
      actorTemporal.id = a.id;
      actorTemporal.influencia = a.influencia;
      nuevoVectorDeActoresRestantes.push_back(actorTemporal);
    } else {
      nuevaSumaRestante -= a.influencia;
      if (nuevaSumaRestante + influenciaParcial < influenciaMax) {
        break;
      }
    }
  }

  if (nuevaSumaRestante + influenciaParcial < influenciaMax) { // Si agregando al actor actual no podemos superar la influencia máxima, no hace falta recorrer Q U {v}
    return cliqueMasInfluyenteBT2(
      cliqueActual,
      actoresRestantes,
      influenciaParcial,
      sumaRestante - actorActual.influencia
    );
  }

  // Recorriendo primero esta clique logramos podar antes, al tener más posibilidades de armar una clique con mayor influencia
  clique loAgrego = cliqueMasInfluyenteBT2( // Agrego el actor actual a la clique
    cliqueActual.addActor(actorActual),
    nuevoVectorDeActoresRestantes,
    influenciaParcial + actorActual.influencia, // Sumamos la influencia del actor actual que agregamos a la clique
    nuevaSumaRestante - actorActual.influencia // Restamos la influencia del actor actual ya que la sumamos a la influencia de la clique actual
  );

  cliqueActual.popActor();
  
  clique noLoAgrego = cliqueMasInfluyenteBT2( // No agrego el actor actual al clique
    cliqueActual,
    actoresRestantes,
    influenciaParcial,
    sumaRestante - actorActual.influencia // Restamos la influencia del actor actual ya que no será considerado para esta rama 
  );

  return noLoAgrego.getInfluencia() > loAgrego.getInfluencia() ? noLoAgrego : loAgrego; // Devuelvo el clique con mas influencia
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(0);
  if (argc < 1) {
    cerr << "Parametro faltante: " << endl << "Path al archivo de instancia." << endl;
    return 1;
  }
  
  if (argc >= 3) {
    if (argv[SORT_INDEX] == SORT_ASCENDING) {
      SORT_FLAG = SORT_ASCENDING;
    } else if (argv[SORT_INDEX] == SORT_DESCENDING) {
      SORT_FLAG = SORT_DESCENDING;
    }
  }
  
  // Correr con ./ejercicio1.exe <instancia> <sort> <debug>
  bool debug = false;
  if (argc >= 4) {
    debug = argv[DEBUG_INDEX] == DEBUG_FLAG;
  }

  ifstream archivo_instancia;

  archivo_instancia.open(argv[FILE_INDEX]);
  string linea_instancia;
  int sumaInfluenciaTotal = 0; // Sumatoria de todas las influencias de todos los actores

  if (archivo_instancia.is_open()) {
    while (archivo_instancia.good()) {
      getline(archivo_instancia, linea_instancia);
      if (linea_instancia[0] == ID_FIRST_LINE) {
        istringstream iss(linea_instancia);
        vector<std::string> results(
          istream_iterator<std::string>{iss},
          istream_iterator<std::string>()
        );
        M = stoi(results[results.size() - 1]);
        N = stoi(results[results.size() - 2]);
        if (N == 0) {
          break;
        }
        Amistades = vector<vector<bool>>(N + 1, vector<bool>(N + 1, false));
      } else if (linea_instancia[0] == ID_ACTOR) {
        istringstream iss(linea_instancia);
        vector<std::string> results(
          istream_iterator<std::string>{iss},
          istream_iterator<std::string>()
        );
        int influenciaActor = stoi(results[results.size() - 1]);
        int idActor = stoi(results[results.size() - 2]);
        // Descomentar en caso de querer printear
        // if (debug) {
        //   cout << "Actor: " << idActor << " - Influencia: " << influenciaActor << endl;
        // }
        actor newActor;
        newActor.id = idActor;
        newActor.influencia = influenciaActor;
        Actores.push_back(newActor);
        sumaInfluenciaTotal += influenciaActor;

      } else if (linea_instancia[0] == ID_AMISTAD) {
        istringstream iss(linea_instancia);
        vector<std::string> results(
          istream_iterator<std::string>{iss},
          istream_iterator<std::string>()
        );
        int idActor2 = stoi(results[results.size() - 1]);
        int idActor1 = stoi(results[results.size() - 2]);
        // Descomentar en caso de querer printear
        // if (debug) {
        //   cout << "Actor " << idActor1 << " es amigo de Actor " << idActor2 << endl;
        // }
        Amistades[idActor1][idActor2] = true;
        Amistades[idActor2][idActor1] = true;
      }
    }
  } else {
    cout << "No se pudo leer el archivo: " << argv[FILE_INDEX] << endl;
  }
  
  if (N == 0) {
    cout << "Los actores no pueden ser 0" << endl; 
    return 1;
  }

  if (debug) {
    cout << "Se leyeron " << N << " actores y " << M << " amistades." << endl;
    cout << "Procesando..." << endl;
  }

  auto start = chrono::steady_clock::now(); // Empieza el clock

  // Ordena la lista de actores;
  if (SORT_FLAG == SORT_ASCENDING) {
    sort(Actores.begin(), Actores.end(), &actorAsc);
  }else{
    sort(Actores.begin(), Actores.end(), &actorDesc);
  }

  // Funcion BT Ej 1
  clique res = cliqueMasInfluyenteBT2(*new clique(), Actores, 0, sumaInfluenciaTotal);
  auto end = chrono::steady_clock::now(); // Termina el clock
  double total_time = chrono::duration<double, milli>(end - start).count();
  if (debug) {
    cout << "Influencia: " << res.getInfluencia() << endl;
    cout << "Tiempo de ejecucion: " << total_time << " ms" << endl;
  } else {
    cout << res.getInfluencia() << endl;
  }

  for (actor a : res.getActores()) {
    cout << a.id << " ";
  }
  cout << endl;
  return 0;
}
