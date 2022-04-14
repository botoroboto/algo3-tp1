#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <string>
#include "clique.h"

using namespace std;

struct grupoIndependiente {
    vector<actor> actores = {};
    influencia = 0;
};


vector<grupoIndependiente> generarGruposIndependientesMaximizandoActoresPorGrupo(vector<actor> actoresPorAgrupar){
    vector<grupoIndependiente> gruposIndependientes = {};
    //Generamos el primer grupo con el primer actor
    grupoIndependiente primerGrupo;
    primerGrupo.influencia = actoresPorAgrupar[0].influencia;
    primerGrupo.actores.push_back(actoresPorAgrupar[0]);
    gruposIndependientes.push_back(primerGrupo);   
    
    //Elminamos al actor del conjunto de actores restantes por meter en grupos porque ya lo metimos al primer grupo
    actoresPorAgrupar[0].erase(actoresPorAgrupar.begin());

    //vamos armando los grupos independientes a partir del primero, agregamos todos los que podemos y luego generamos un nuevo grupo y repetimos hasta quedarnos sin actores...
    for(grupoIndependiente grupoActual : gruposIndependientes){ 
        //recorremos todos los actoresPorAgrupar restantes para ver si los podemos meter en el grupo actual 
        for(int actorActual = 0; actorActual < actoresPorAgrupar.size(); actorActual++ ){ 
            bool independientes = true
            vector<int> actoresPorBorrar = {}
            for(actor actorIndependiente : grupoActual.actores){
                //Verificamos que el actor no sea amigo de ninguno del grupo actual  
                if(sonAmigos(actoresPorAgrupar[actorActual], actorIndependiente)){
                    //Si no es amigo podemos decartarlo como opcion para el grupo actual
                    idependientes = false;
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
            actoresPorAgrupar.erase(a);
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


int main(){


    return 0;
}