#ifndef SPIELEMANAGER_H
#define SPIELEMANAGER_H


#include <vector>


class Spiel;  //Platzhalter definition, gibts es irgendwo 

class Spielemanger
{
private:
   std::vector<Spiel*> spiele;
public:
    Spielemanger();
    ~Spielemanger();
    void add_game(Spiel * game);
    void start();
};


#endif
