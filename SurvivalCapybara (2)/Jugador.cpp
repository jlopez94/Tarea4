#include "Jugador.h"
#include<SDL2/SDL_mixer.h>
Mix_Chunk *low = NULL;


Jugador::Jugador(list<Entidad*>* entidades,SDL_Renderer* renderer)
{
       //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {

    }

    low = Mix_LoadWAV( "Personaje/low.wav" );

    tipo = "Jugador";
    this->renderer = renderer;
    this->textures["down"].push_back(IMG_LoadTexture(renderer, "Personaje/down1.png"));
    this->textures["down"].push_back(IMG_LoadTexture(renderer, "Personaje/down2.png"));
    this->textures["up"].push_back(IMG_LoadTexture(renderer, "Personaje/up1.png"));
    this->textures["up"].push_back(IMG_LoadTexture(renderer, "Personaje/up2.png"));
    this->textures["left"].push_back(IMG_LoadTexture(renderer, "Personaje/left1.png"));
    this->textures["left"].push_back(IMG_LoadTexture(renderer, "Personaje/left2.png"));
    this->textures["right"].push_back(IMG_LoadTexture(renderer, "Personaje/right1.png"));
    this->textures["right"].push_back(IMG_LoadTexture(renderer, "Personaje/right2.png"));

    SDL_QueryTexture(this->textures["down"][0], NULL, NULL, &rect.w, &rect.h);
    x = 200;
    y = 100;
    rect.x = x;
    rect.y = x;

    velocity=1.5;
    animation_velocity=20;

    current_texture=0;

    state="down";

    this->entidades = entidades;

    proyectil_cooldown=0;

    vidas = 3;
}

Jugador::~Jugador()
{
    //dtor
}

void Jugador::logicaPush()
{


    if(push_orientation=="right")
    {
        x+=5;
        push_amount-=1;
        Mix_PlayChannel( -1, low, 0 );
             SDL_Delay(5);
             Mix_FreeChunk( low );
        if(push_amount<=0)
        {
            Mix_HaltMusic();
            push_orientation="";
        }

    }
    if(push_orientation=="left")
    {
        x-=5;
        push_amount-=1;
        Mix_PlayChannel( -1, low, 0 );
             SDL_Delay(5);
             Mix_FreeChunk( low );
          if(push_amount<=0)
        {
            Mix_HaltMusic();
            push_orientation="";
        }
    }
    if(push_orientation=="up")
    {
        y-=5;
        push_amount-=1;
        Mix_PlayChannel( -1, low, 0 );
             SDL_Delay(5);
             Mix_FreeChunk( low );
          if(push_amount<=0)
        {
            Mix_HaltMusic();
            push_orientation="";
        }
    }
    if(push_orientation=="down")
    {
        y+=5;
        push_amount-=1;
        Mix_PlayChannel( -1, low, 0 );
             SDL_Delay(5);
             Mix_FreeChunk( low );
         if(push_amount<=0)
        {
            Mix_HaltMusic();
            push_orientation="";
        }
    }
}

void Jugador::logica()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    logicaPush();

    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        x+=velocity;
        state="right";
    }
    if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        x-=velocity;
        state="left";
    }
    if( currentKeyStates[ SDL_SCANCODE_UP ] )
    {
        y-=velocity;
        state="up";
    }
    if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
    {
        y+=velocity;
        state="down";
    }

    if(proyectil_cooldown>0)
        proyectil_cooldown--;

    if( currentKeyStates[ SDL_SCANCODE_Z ] && proyectil_cooldown==0)
    {
        Proyectil *p = new Proyectil(entidades,renderer,x,y,state);
        entidades->push_back(p);
        proyectil_cooldown=20;
    }

    if(frames%animation_velocity==0)
    {
        current_texture++;
        if(current_texture>=textures[state].size())
            current_texture=0;
    }

    frames++;
}

void Jugador::dibujar()
{
    Personaje::dibujar();
}
