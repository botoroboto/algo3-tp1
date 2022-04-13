#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include "clique.h"

using namespace std;

int N = 0;
int N_INDEX = 3;
int M = 0;
int M_INDEX = 4;
string ID_ACTOR = "n";
string ID_AMISTAD = "e";

vector<actor> Actores;
vector<vector<bool>> Amistades;

bool sonAmigos(actor a1, actor a2) {
  return Amistades[a1.id][a2.id];
};

int influenciaMax = 0; // Counter para la influencia máxima, voy a ir actualizando con la mejor que tenga hasta el momento

// TODO - Como podemos refactorizar esto para sacarlo del main? O deberiamos tener 4 main.cpp?
clique cliqueMasInfluyenteBT(clique cliqueActual, int actorActual, int influenciaParcial, int sumaRestante) {
  // Caso base
  if (actorActual > Actores.size() - 1) {
    cliqueActual.setInfluencia(influenciaParcial);
      if (influenciaMax < influenciaParcial) {
      influenciaMax = influenciaParcial;
    }
    return cliqueActual;
  }

  // Poda de optimalidad
  if (sumaRestante + influenciaParcial < influenciaMax) {
    return cliqueActual; // No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
  }

  // Poda de factibilidad
  bool hayAmistad = true;
  for (actor a : cliqueActual.getActores()) {
    if (!sonAmigos(a, Actores[actorActual])) {
      hayAmistad = false;
      break;
    }
  }

  if (!hayAmistad) {
    return cliqueMasInfluyenteBT(
      cliqueActual,
      actorActual + 1,
      influenciaParcial,
      sumaRestante - Actores[actorActual].influencia
    );
  }

  // Agrego el actor actual al clique (tengo cierta intuición de que si corro esta rama del arbol primero va a ser mas eficiente la poda de optimalidad cuando corra la otra rama)
  clique loAgrego = cliqueMasInfluyenteBT(
    cliqueActual.clone().addActor(Actores[actorActual]),
    actorActual + 1,
    influenciaParcial + Actores[actorActual].influencia,
    sumaRestante - Actores[actorActual].influencia
  );

  // No agrego el actor actual al clique
  clique noLoAgrego = cliqueMasInfluyenteBT(
    cliqueActual,
    actorActual + 1,
    influenciaParcial,
    sumaRestante - Actores[actorActual].influencia
  );

  // Devuelvo la clique con mas influencia
  return noLoAgrego.getInfluencia() > loAgrego.getInfluencia() ? noLoAgrego : loAgrego;
};

clique cliqueMasInfluyenteBT2(clique cliqueActual, vector<actor> actoresRestantes, int influenciaParcial, int sumaRestante){
  // caso base
  if (actoresRestantes.size() < 1) {
    cliqueActual.setInfluencia(influenciaParcial);
    if(influenciaMax < influenciaParcial){
      influenciaMax = influenciaParcial;
    }
    return cliqueActual;
  }
 
  //Poda de optimalidad
  if(sumaRestante + influenciaParcial < influenciaMax){
    return cliqueActual; //No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
  }

  bool sonTodosAmigos = false;

  for (actor a : actoresRestantes) {
    for (actor a2 : actoresRestantes) {
      if (a.id < a2.id) {
        if (sonAmigos(a, a2)) {
          sonTodosAmigos = true;
        } else {
          sonTodosAmigos = false;
          break;
        }
      }
    }
    if (!sonTodosAmigos) {
      break;
    }
  }

  if (sonTodosAmigos) { // esto significa que todos son amigos entre todos de K por lo que podemos meterlos a todos al clique
    cliqueActual.setInfluencia(sumaRestante + influenciaParcial); // la influencia es toda la que quedaba mas la que ya traía
    for(actor a : actoresRestantes){
      cliqueActual.addActor(a);
    }
    if (sumaRestante + influenciaParcial > influenciaMax) {
      influenciaMax = sumaRestante + influenciaParcial;
    }
    return cliqueActual;
  }
  
  actor actorActual;
  actorActual.id = actoresRestantes[0].id;
  actorActual.influencia = actoresRestantes[0].influencia;
  actoresRestantes.erase(actoresRestantes.begin());
  
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
    }
  }

  if (nuevaSumaRestante + influenciaParcial < influenciaMax) { // la suma restante todavía tiene al actor actual en cuenta...
    return cliqueMasInfluyenteBT2( // no agrego el actor actual al clique
      cliqueActual,
      actoresRestantes,
      influenciaParcial,
      sumaRestante - actorActual.influencia
    ); // No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
  }

  nuevaSumaRestante -= actorActual.influencia; //le resto a la suma restante la influencia del actor actual ya que ahora voy a agregarlo o descartarlo
  
  clique loAgrego = cliqueMasInfluyenteBT2( //agrego el actor actual al clique (tengo cierta intuición de que si corro esta rama del arbol primero va a ser mas eficiente la poda de optimalidad cuando corra la otra rama)
    cliqueActual.clone().addActor(actorActual),
    nuevoVectorDeActoresRestantes,
    influenciaParcial + actorActual.influencia,
    nuevaSumaRestante
  );

  clique noLoAgrego = cliqueMasInfluyenteBT2( //no agrego el actor actual al clique
    cliqueActual,
    actoresRestantes,
    influenciaParcial,
    sumaRestante - actorActual.influencia
  );

  return noLoAgrego.getInfluencia() > loAgrego.getInfluencia() ? noLoAgrego : loAgrego; //Devuelvo el clique con mas influencia
}

// TODO - Parsear desde un file
int main(long argc, char *argv[]) {
  // Funcion BT Ej 1
  int sumaInfluenciaTotal = 0; // Sumatoria de todas las influencias de todos los actores

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

  Amistades = vector<vector<bool>>(N + 1, vector<bool>(N + 1, false));

  for (int i = M_INDEX + 1; i < argc; i += 3) {
    if (argv[i] == ID_ACTOR) {
      // Descomentar si se necesita debuggear
      // cout << "Actor: " << stoi(argv[i+1]) << " - Influencia: " << stoi(argv[i+2]) << endl;
      actor newActor;
      newActor.id = stoi(argv[i+1]);
      newActor.influencia = stoi(argv[i+2]);
      // TODO - Podríamos sortear acá
      Actores.push_back(newActor);
      sumaInfluenciaTotal += newActor.influencia;
    } else if (argv[i] == ID_AMISTAD) {
      // Descomentar si se necesita debuggear
      // cout << "Actor " << stoi(argv[i+1]) << " es amigo de Actor " << stoi(argv[i+2]) << endl;
      Amistades[stoi(argv[i+1])][stoi(argv[i+2])] = true;
      Amistades[stoi(argv[i+2])][stoi(argv[i+1])] = true;
    }
  };

  auto start = chrono::steady_clock::now(); // Empieza el clock
  clique res = cliqueMasInfluyenteBT2(*new clique(), Actores, 0, sumaInfluenciaTotal);
  auto end = chrono::steady_clock::now(); // Termina el clock
  double total_time = chrono::duration<double, milli>(end - start).count();
  clog << total_time << " ms" << endl;

  cout << res.getInfluencia() << endl;
  for (actor a : res.getActores()) {
    cout << a.id << " ";
  }
  cout << endl;
  return 0;
}
