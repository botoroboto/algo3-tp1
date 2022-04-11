#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

int N = 0;
int N_INDEX = 3;
int M = 0;
int M_INDEX = 4;
string ID_ACTOR = "n";
string ID_AMISTAD = "e";

int main(int argc, char *argv[]) {
  
  struct actor {
    int influencia;
    int id;
  }

  vector<actor> Actores;
  vector<vector<bool>> Amistades;
  
  bool sonAmigos(actor a1, actor a2){
    return Amistades[a1.id][a2.id];
  }

  struct clique {
    vector<actor> actores = {};
    int infleuncia = 0;
  }

  //Funcion BT Ej 1
  
  int sumaInfluenciaTotal = 0; //Sumatoria de todas las influencias de todos los actores
  for(actor a : Actores){
    sumaInfluenciaTotal += a.influencia;
  }

  int influenciaMax = 0; //holder para la influencia Máxima voy a ir actualizando con la mejor que tenga hasta el momento
  clique cliqueMasInfluyenteBT (clique cliqueActual, int actorActual, int influenciaParcial, int sumaRestante ) {
    
    //Caso base
    if(actorActual == Actores.size() - 1){
      cliqueActual.influencia = influenciaParcial;
      if(influenciaMax < influenciaParcial ){
        influenciaMax = influenciaParcial;
      }
      return cliqueActual;
    }

    //Poda de optimalidad
    if(sumaRestante + influenciaParcial < influenciaMax){
      return cliqueActual; //No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
    }

    //Poda de factibilidad 
    bool hayAmistad = true;
    for(actor a : cliqueActual.actores){
      if(!sonAmigos(a, Actores[actorActual])){
        hayAmistad = false;
        break;
      }
    }
    if(!hayAmistad){
      return cliqueMasInfluyenteBT(
        cliqueActual,
        actorActual + 1,
        influenciaParcial,
        sumaRestante - Actores[actorActual].influencia
      );
    }

    clique loAgrego = cliqueMasInfluyenteBT( //agrego el actor actual al clique (tengo cierta intuición de que si corro esta rama del arbol primero va a ser mas eficiente la poda de optimalidad cuando corra la otra rama)
        cliqueActual.actores.push(Actores[actorActual]),
        actorActual + 1,
        influenciaParcial + Actores[actorActual].influencia,
        sumaRestante - Actores[actorActual].influencia
      );
    clique noLoAgrego = cliqueMasInfluyenteBT( //no agrego el actor actual al clique
        cliqueActual,
        actorActual + 1,
        influenciaParcial,
        sumaRestante - Actores[actorActual].influencia
      );
    return noLoAgrego.influencia > loAgrego.influencia ? noLoAgrego : loAgrego; //Devuelvo el clique con mas influencia
  }
  //se correría asi -> cliqueMasInfluyentesBT(new clique(), 0, 0, sumaInfluenciaTotal);


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

  for (int i = M_INDEX + 1; i < argc; i += 3) {
    if (argv[i] == ID_ACTOR) {
      cout << "Actor: " << stoi(argv[i+1]) << " - Influencia: " << stoi(argv[i+2]) << endl;
    } else if (argv[i] == ID_AMISTAD) {
      cout << "Actor " << stoi(argv[i+1]) << " es amigo de Actor " << stoi(argv[i+2]) << endl;
    }
  };
  return 0;
}
