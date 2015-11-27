#ifndef JUGADOR_H
#define JUGADOR_H

#include <list>
#include "Personaje.h"
#include "Proyectil.h"

class Jugador : public Personaje
{
    public:
        int vidas;
        int proyectil_cooldown;
        Jugador(list<Entidad*>* entidades,SDL_Renderer* renderer);
        virtual ~Jugador();
        void logica();
        void logicaPush();
        void dibujar();
    protected:
    private:
};

#endif // JUGADOR_H
