#include "clique.h"

using namespace std;

clique::clique() {
  this->influencia = 0;
  this->actores = {};
}

vector<actor> clique::getActores(){
  return this->actores; // expression must have class type but it has type "clique *"
}

clique clique::clone() {
    clique newClique = *new clique();
    int a = this->influencia;
    newClique.setInfluencia(a);
    for (int i = 0; i < this->actores.size(); ++i) {
        actor b = this->actores[i];
        newClique.actores.push_back(b);
    }

    return newClique;
}

// void clique::popActor() {
//   this->actores.pop_back();
// }

clique clique::addActor(actor newActor){
  this->actores.push_back(newActor);
  return *this;
}

int clique::setInfluencia(int newInfluencia){
  this->influencia = newInfluencia;
  return this->influencia;
}

int clique::getInfluencia(){
  return this->influencia;
}
