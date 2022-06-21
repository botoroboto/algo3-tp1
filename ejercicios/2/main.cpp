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
string DEBUG_FLAG = "debug";
string SORT_ASCENDING = "asc";
string SORT_DESCENDING = "desc";
string SORT_FLAG = SORT_DESCENDING;
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

bool actorAsc (const actor& a, const actor& b){
    return  a.influencia < b.influencia;
}
bool actorDesc (const actor& a, const actor& b){
    return  a.influencia > b.influencia;
}

struct grupoIndependiente {
    vector<actor> actores = {};
    int influencia = 0;
};

int influenciaMax = 0; // Counter para la influencia máxima, voy a ir actualizando con la mejor que tenga hasta el momento

struct ParticionesIndependientes {
    vector<grupoIndependiente> gruposIndependientes;
    int influencia = 0;
};

bool calcularSonTodosAmigos(clique &cliqueActual, vector<actor> &actoresRestantes, int &influenciaParcial, int &sumaRestante) {
  bool sonTodosAmigos = false;

  for (int a = 0; a < actoresRestantes.size(); a++) {
      bool esAmigoDeTodos = true;
      for (actor a2 : actoresRestantes) {
          if (actoresRestantes[a].id != a2.id) {
              sonTodosAmigos = sonAmigos(actoresRestantes[a], a2);
              if (!sonTodosAmigos) {
                  esAmigoDeTodos = false;
                  break;
              }
          }
      }
      if (!sonTodosAmigos) {
        break;
      }
      if (esAmigoDeTodos){
          cliqueActual.addActor(actoresRestantes[a]);
          influenciaParcial += actoresRestantes[a].influencia;
          sumaRestante -= actoresRestantes[a].influencia;
          actoresRestantes.erase(actoresRestantes.begin() + a);
          a--;
      }
  }

  return sonTodosAmigos;
};

int calcularSumaRestante(actor &actorActual, vector<actor> &nuevoVectorDeActoresRestantes, vector<actor> &actoresRestantes, int influenciaParcial, int influenciaMax, int sumaRestante) {
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

  return nuevaSumaRestante;
};

ParticionesIndependientes generarGruposIndependientesMaximizandoActoresPorGrupo(vector<actor> actoresPorAgrupar){
    ParticionesIndependientes res;

    //reverse(actoresPorAgrupar.begin(), actoresPorAgrupar.end());
    vector<grupoIndependiente> gruposIndependientes = {};
    //Generamos el primer grupo con el primer actor
    grupoIndependiente primerGrupo;
    primerGrupo.influencia = actoresPorAgrupar[0].influencia;
    primerGrupo.actores.push_back(actoresPorAgrupar[0]);
    gruposIndependientes.push_back(primerGrupo);

    //Elminamos al actor del conjunto de actores restantes por meter en grupos porque ya lo metimos al primer grupo
    actoresPorAgrupar.erase(actoresPorAgrupar.begin());

    //vamos armando los grupos independientes a partir del primero, agregamos todos los que podemos y luego generamos un nuevo grupo y repetimos hasta quedarnos sin actores...
    for(int i=0; i<gruposIndependientes.size(); i++){
        grupoIndependiente &grupoActual = gruposIndependientes[i];

        //recorremos todos los actoresPorAgrupar restantes para ver si los podemos meter en el grupo actual
        for(int actorActual = 0; actorActual < actoresPorAgrupar.size(); actorActual++ ){
            bool independientes = true;
            for(actor &actorIndependiente : grupoActual.actores){
                //Verificamos que el actor no sea amigo de ninguno del grupo actual  
                if(sonAmigos(actoresPorAgrupar[actorActual], actorIndependiente)){
                    //Si no es amigo podemos decartarlo como opcion para el grupo actual
                    independientes = false;
                    break;
                }
            }
            if(independientes){ //en el caso de que haya sido independiente lo agregamos al grupo actual
                grupoActual.actores.push_back(actoresPorAgrupar[actorActual]);
                grupoActual.influencia = max(actoresPorAgrupar[actorActual].influencia, grupoActual.influencia);
                actoresPorAgrupar.erase(actoresPorAgrupar.begin() + actorActual);
                actorActual--;
            }
        }
        res.influencia += grupoActual.influencia;
        if(actoresPorAgrupar.empty()){
            //Si ya no quedan actores ya no hay que partir mas
            break;
        }
        //Si me quedan actores por agrupar, creo un nuevo grupo con un actor y la influencia de ese actor y lo agrego al array de grupos independientes
        grupoIndependiente nuevoGrupo;
        nuevoGrupo.actores.push_back(actoresPorAgrupar[0]);
        nuevoGrupo.influencia = actoresPorAgrupar[0].influencia;
        actoresPorAgrupar.erase(actoresPorAgrupar.begin());
        gruposIndependientes.push_back(nuevoGrupo);
    }

    res.gruposIndependientes = gruposIndependientes;
    return res;
}

clique cliqueMasInfluyenteConPodaGolosa(clique cliqueActual, vector<actor> actoresRestantes, int influenciaParcial, int sumaRestante){
    // Caso base
    if (actoresRestantes.empty()) {
        cliqueActual.setInfluencia(influenciaParcial);
        if (influenciaMax < influenciaParcial) {
            influenciaMax = influenciaParcial;
        }
        return cliqueActual;
    }

    // Poda de optimalidad
    if (sumaRestante + influenciaParcial <= influenciaMax) {
        return cliqueActual; // No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
    }

    // Poda golosa
    int influenciaIndependiente = generarGruposIndependientesMaximizandoActoresPorGrupo(actoresRestantes).influencia;
    if(influenciaParcial + influenciaIndependiente <= influenciaMax ){
        return cliqueActual;
    }

    bool sonTodosAmigos = calcularSonTodosAmigos(cliqueActual, actoresRestantes, influenciaParcial, sumaRestante);

    if (sonTodosAmigos) { // esto significa que todos son amigos entre todos de K por lo que podemos meterlos a todos al clique
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
    int nuevaSumaRestante = calcularSumaRestante(actorActual, nuevoVectorDeActoresRestantes, actoresRestantes, influenciaParcial, influenciaMax, sumaRestante);

    if (nuevaSumaRestante + influenciaParcial < influenciaMax) { // la suma restante todavía tiene al actor actual en cuenta...
        return cliqueMasInfluyenteConPodaGolosa( // no agrego el actor actual al clique
                cliqueActual,
                actoresRestantes,
                influenciaParcial,
                sumaRestante - actorActual.influencia
        ); // No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
    }

    nuevaSumaRestante -= actorActual.influencia; //le resto a la suma restante la influencia del actor actual ya que ahora voy a agregarlo o descartarlo

    clique loAgrego = cliqueMasInfluyenteConPodaGolosa( //agrego el actor actual al clique (tengo cierta intuición de que si corro esta rama del arbol primero va a ser mas eficiente la poda de optimalidad cuando corra la otra rama)
            cliqueActual.addActor(actorActual),
            nuevoVectorDeActoresRestantes,
            influenciaParcial + actorActual.influencia,
            nuevaSumaRestante
    );

    cliqueActual.popActor();

    clique noLoAgrego = cliqueMasInfluyenteConPodaGolosa( //no agrego el actor actual al clique
            cliqueActual,
            actoresRestantes,
            influenciaParcial,
            sumaRestante - actorActual.influencia
    );

    return noLoAgrego.getInfluencia() > loAgrego.getInfluencia() ? noLoAgrego : loAgrego; //Devuelvo el clique con mas influencia
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

    // Correr con ./ejercicio2.exe <instancia> <sort> <debug>
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

    //Sort the actor list;
    if (SORT_FLAG == SORT_ASCENDING) {
        sort(Actores.begin(), Actores.end(), &actorAsc);
    }else{
        sort(Actores.begin(), Actores.end(), &actorDesc);
    }

    // Funcion BT Con Poda Golosa EJ2
    auto start = chrono::steady_clock::now(); // Empieza el clock
    clique res = cliqueMasInfluyenteConPodaGolosa(*new clique(), Actores, 0, sumaInfluenciaTotal);
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