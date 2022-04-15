#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <iterator>
#include "clique.h"
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

bool descComp (const actor& a, const actor& b)
{
    return  a.influencia > b.influencia;
}

struct grupoIndependiente {
    vector<actor> actores = {};
    int influencia = 0;
};

int influenciaMax = 0; // Counter para la influencia máxima, voy a ir actualizando con la mejor que tenga hasta el momento

int obtenerInfluenciaDeGruposIndependientes(vector<grupoIndependiente> grupos){
    int res = 0;
    for(grupoIndependiente grupo : grupos){
        res += grupo.influencia;
    }
    return res;
}

vector<grupoIndependiente> generarGruposIndependientesMaximizandoActoresPorGrupo(vector<actor> actoresPorAgrupar){
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
    return gruposIndependientes;
}

//PRIMER APROACH FUNCIONA PERO NO RESPETA LA CONSIGNA
// vector<grupoIndependiente> generarGruposIndependientesMinimizandoGrupos(vector<actor> actores){
    
//     vector<grupoIndependiente> gruposIndependientes = {};
//     grupoIndependiente primerGrupo;
//     primerGrupo.influencia = actores[0].influencia;
//     primerGrupo.actores.push_back(actores[0]);
//     gruposIndependientes.push_back(primerGrupo);   

//     int actorActual = 1;
//     int grupoActual = 0;

//     for(actorActual; actorActual < actores.size(); actorActual++ ){

//         bool sonIndependientes = true;
//         for(actor actorIndependiente : gruposIndependientes[grupoActual].actores){
//             if(sonAmigos(actores[actorActual], actorIndependiente)){
//                sonIndependientes = false;
//                break;
//             }
//         }
//         if(sonIndependientes){
//             actoresIndependientes[grupoActual].actores.push_back(actores[actorActual]);
//             actoresIndependientes[grupoActual].influencia = max(actores[actorActual].influencia, actoresIndependientes[grupoActual].influencia);
//         }else{
//             grupoActual++;
//             grupoIndependiente nuevoGrupo;
//             nuevoGrupo.actores.push_back(actores[actorActual]);
//             nuevoGrupo.influencia = actores[actorActual].influencia;
//             actoresIndependientes[grupoActual].push_back(nuevoGrupo);    
//         }
//     }

//     return gruposIndependientes;
// }

clique cliqueMasInfluyenteConPodaGolosa(clique cliqueActual, vector<actor> actoresRestantes, int influenciaParcial, int sumaRestante){
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
    return cliqueActual; // No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
  }

  // Poda golosa
  int influenciaIndependiente = obtenerInfluenciaDeGruposIndependientes(generarGruposIndependientesMaximizandoActoresPorGrupo(actoresRestantes));
  if(influenciaParcial + influenciaIndependiente <= influenciaMax ){
      return cliqueActual;
  }

  bool sonTodosAmigos = false;

  for (actor a : actoresRestantes) {
    for (actor a2 : actoresRestantes) {
      if (a.id < a2.id) {
        sonTodosAmigos = sonAmigos(a, a2);
        if (!sonTodosAmigos) {
          break;
        }
      } else {
        break;
      }
    }
    if (!sonTodosAmigos) {
      break;
    }
  }

  if (sonTodosAmigos) { // esto significa que todos son amigos entre todos de K por lo que podemos meterlos a todos al clique
    cliqueActual.setInfluencia(sumaRestante + influenciaParcial); // la influencia es toda la que quedaba mas la que ya traía
    for (actor a : actoresRestantes) {
      cliqueActual.addActor(a);
    }
    if (sumaRestante + influenciaParcial > influenciaMax) {
      influenciaMax = sumaRestante + influenciaParcial;
    }
    return cliqueActual;
  }

  actor actorActual;
  size_t actorActualIndex = actoresRestantes.size() - 1;
  if (SORT_FLAG == SORT_ASCENDING) {
    // Para correr de menor a mayor influencia
    actorActualIndex = 0;
  } else {
    // Para correr de mayor a menor influencia
    actorActualIndex = actoresRestantes.size() - 1;
  }
  actorActual.id = actoresRestantes[actorActualIndex].id;
  actorActual.influencia = actoresRestantes[actorActualIndex].influencia;
  actoresRestantes.erase(actoresRestantes.begin() + actorActualIndex);
  
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


// TODO - Parsear desde un file
int main(int argc, char *argv[]) {
    if (argc < 1) {
        cerr << "Parametro faltante: " << endl << "Path al archivo de instancia." << endl;
        return 1;
    }

    if (argc == 3) {
        if (argv[SORT_INDEX] == SORT_ASCENDING) {
            SORT_FLAG = SORT_ASCENDING;
        } else if (argv[SORT_INDEX] == SORT_DESCENDING) {
            SORT_FLAG = SORT_DESCENDING;
        }
    }

    SORT_FLAG = SORT_ASCENDING;

    bool debug = true;
    if (argc == 4) {
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
                // TODO - Revisar este sort
                int i = 0;
                if (Actores.size() > 0) {
                    for (i; i <= Actores.size() - 1; i += 1) {
                        if (Actores[i].influencia > influenciaActor) {
                            break;
                        } else {
                            continue;
                        }
                    }
                }
                Actores.emplace(Actores.begin() + i, newActor);
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

    // Funcion BT Ej 1
    std::sort(Actores.begin(), Actores.end(), &descComp);
    auto start = chrono::steady_clock::now(); // Empieza el clock
    // clique res = cliqueMasInfluyenteBT(*new clique(), 0, 0, sumaInfluenciaTotal);
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