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
    vector<actor> getActores();
    void popActor();
    clique addActor(actor newActor);
    int setInfluencia(int newInfluencia);
    int getInfluencia();
};
