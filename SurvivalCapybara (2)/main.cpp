#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<iostream>
#include<list>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include "Jugador.h"
#include "Enemigo.h"
#include "Proyectil.h"
#include "Vida.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character;
SDL_Rect rect_background,rect_character;
SDL_Surface *message = NULL;
//The event structure
SDL_Event event;

//The font that's going to be used
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 255, 255, 255 };
int deaths=0;
stringstream ss;


using namespace std;

int main( int argc, char* args[] )
{
       //Initialize SDL_ttf


    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }

     if( TTF_Init() == -1 )
    {
        return false;
    }

       //Open the font
    font = TTF_OpenFont( "lazy.ttf", 14 );

    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 500/*WIDTH*/, 250/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    character = IMG_LoadTexture(renderer, "personaje.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = 0;
    rect_character.y = 100;
    rect_character.w = w;
    rect_character.h = h;

    SDL_Texture* otro_personaje = IMG_LoadTexture(renderer,"otropersonaje.png");
    SDL_Rect otro_rect;
    otro_rect.x=150;
    otro_rect.y=50;
    otro_rect.w=32;
    otro_rect.h=32;


    list<Entidad*>entidades;
    entidades.push_back(new Jugador(&entidades,renderer));
    entidades.push_back(new Vida(&entidades,renderer));

    double last_frame=0;
    int frame=0;
    //Main Loop
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_d)
                    rect_character.x++;
            }
        }

        int spawn_frequency=60-(frame/100);
        if(spawn_frequency<5)
            spawn_frequency=5;
        if(frame%spawn_frequency==0)
            entidades.push_back(new Enemigo(&entidades,renderer));

        //SDL_Delay(17-(SDL_GetTicks()-last_frame));
        double diferencia = SDL_GetTicks()-last_frame;
        double ajuste = 17 - diferencia;
        if(ajuste>0)
            SDL_Delay(ajuste);
        last_frame=SDL_GetTicks();

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
            (*e)->logica();

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
        {
            if((*e)->delete_flag)
            {
                entidades.remove(*e);
                deaths++;
                ss << deaths;
                string str_deaths = ss.str();
                //Render the text
                message = TTF_RenderText_Solid( font,str , textColor );
                apply_surface( 0, 0, message, window );
                SDL_Flip( window );
                break;
            }
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(int i=0;i<2;i++)
        {
            for(list<Entidad*>::iterator e = entidades.begin();
                e!=entidades.end();
                e++)
            {
                if((*e)->capa==i)
                    (*e)->dibujar();
            }
        }

        SDL_RenderPresent(renderer);
        frame++;
    }

      SDL_FreeSurface( message );

    //Close the font that was used
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();
	return 0;
}
