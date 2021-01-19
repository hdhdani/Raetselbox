#ifndef SPIEL_H
#define SPIEL_H

#include <string>


class Spiel
{
private:
    /* data */
    std::string name;
public:
    Spiel(std::string newname);
    
    virtual void run(void) = 0 ; //virtuelle Methode, jedes Spiel hat seine eigene run()
    std::string getname(void);

};


#endif
