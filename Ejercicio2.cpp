#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include "clique.h"
#include "main.cpp"
using namespace std;

struct grupoIndependiente {
    vector<actor> actores = {};
    int influencia = 0;
};

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
    for(grupoIndependiente grupoActual : gruposIndependientes){ 
        
        vector<int> actoresPorBorrar = {};
        //recorremos todos los actoresPorAgrupar restantes para ver si los podemos meter en el grupo actual 
        for(int actorActual = 0; actorActual < actoresPorAgrupar.size(); actorActual++ ){ 
            bool independientes = true;
            for(actor actorIndependiente : grupoActual.actores){
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
                actoresPorBorrar.push_back(actorActual);
            }
        }
        for(int a : actoresPorBorrar){ 
            //Borro de los actoresPorAgrupar los que ya agrupe (se puede optimizar cambiando el push_back del if de independientes por un actoresPorAgrupar.erase() y modificando el indice pero va a quedar más feo, cambiar si hay problemas de tiempo!)
            actoresPorAgrupar.erase(actoresPorAgrupar.begin() + a);
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
  
  int influenciaIndependiente = obtenerInfluenciaDeGruposIndependientes(generarGruposIndependientesMaximizandoActoresPorGrupo(actoresRestantes)); 
  if(influenciaIndependiente + influenciaParcial <= influenciaMax ){
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
    return cliqueMasInfluyenteBT2( // no agrego el actor actual al clique
      cliqueActual,
      actoresRestantes,
      influenciaParcial,
      sumaRestante - actorActual.influencia
    ); // No sigo recorriendo nada el arbol porque la suma ya no puede superar al max actual
  }

  nuevaSumaRestante -= actorActual.influencia; //le resto a la suma restante la influencia del actor actual ya que ahora voy a agregarlo o descartarlo

  clique loAgrego = cliqueMasInfluyenteBT2( //agrego el actor actual al clique (tengo cierta intuición de que si corro esta rama del arbol primero va a ser mas eficiente la poda de optimalidad cuando corra la otra rama)
    cliqueActual.addActor(actorActual),
    nuevoVectorDeActoresRestantes,
    influenciaParcial + actorActual.influencia,
    nuevaSumaRestante
  );

  cliqueActual.popActor();
  
  clique noLoAgrego = cliqueMasInfluyenteBT2( //no agrego el actor actual al clique
    cliqueActual,
    actoresRestantes,
    influenciaParcial,
    sumaRestante - actorActual.influencia
  );

  return noLoAgrego.getInfluencia() > loAgrego.getInfluencia() ? noLoAgrego : loAgrego; //Devuelvo el clique con mas influencia
}

int main(){


    return 0;
}