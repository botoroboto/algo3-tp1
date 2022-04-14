#include <vector>
#include <string>

using namespace std;

struct actor {
  int influencia;
  int id;
};

class clique {
  private:
    vector<actor> actores = {};
    int influencia = 0;

  public:
    clique();
    clique clone();
    vector<actor> getActores();
    clique addActor(actor newActor);
    clique popActor();
    int setInfluencia(int newInfluencia);
    int getInfluencia();
};
