#include "clique.h"

using namespace std;

clique::clique() {
  this->influencia = 0;
  this->actores = {};
}

vector<actor> clique::getActores(){
  return this->actores; // expression must have class type but it has type "clique *"
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
