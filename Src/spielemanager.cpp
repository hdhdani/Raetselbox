
#include "spielemanager.h"

#include "spiel.h"

/*
Spielemanager, 
In diesem werden Anfangs alle Spiele hinzugefügt,
anschließend wird das ganze Spiel gestartet und jedes "unterspiel" in der hinzugefügten Reihenfolge aufgerufen
*/

/**
 * @brief  Spiel/Rätsel starten
 * 
 */
void Spielemanger::start() 
{   
    for(uint8_t i = 0; i< spiele.size(); i++) 
    {
        spiele[i]->run();     
    }
}
/**
 * @brief  Spiel/Rätsel hinzufügen
 * 
 * @param game 
 */
void Spielemanger::add_game(Spiel * game) 
{
    spiele.push_back(game); //Spiel in vektor schreiben
}

Spielemanger::Spielemanger(/* args */)
{
}

Spielemanger::~Spielemanger()
{
}