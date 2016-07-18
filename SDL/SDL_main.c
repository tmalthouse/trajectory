//
//  SDL_main.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/13/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "SDL_main.h"
#include <SDL2/SDL.h>
#include "../body.h"
#include "../types.h"

Vector2dPair min_max_xy_coords(Body *sys, uint64_t count)
{
    Vector2d min = {0,0};
    Vector2d max = {0,0};
    
    for (uint64_t i=0; i<count; i++) {
        Vector3d pos = sys[i].pos;
        
        if (pos.x < min.x) {
            min.x = pos.x;
        }
        if (pos.y < min.y) {
            min.y = pos.y;
        }
        
        if (pos.x > max.x) {
            max.x = pos.x;
        }
        if (pos.y > max.y) {
            max.y = pos.y;
        }
    }
    
    return (Vector2dPair){min, max};
}

Vector2d screenpos(Vector3d body_pos, Vector2dPair orb_min_max, Vector2d screensize)
{
    double x_prop = (body_pos.x-orb_min_max.a.x+10)/(orb_min_max.b.x-orb_min_max.a.x);
    double y_prop = (orb_min_max.b.y-body_pos.y)/(orb_min_max.b.y-orb_min_max.a.y);
    
    return (Vector2d){x_prop*screensize.x, y_prop*screensize.y};
}

void render_system(Body *sys, uint64_t body_count, SDL_Renderer *renderer, Vector2d screen_size)
{
    Vector2dPair refcoords = min_max_xy_coords(sys, body_count);
    
    // First, we need to clear the screen and paint it black.
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    
    // Then, we set the color to planet color (in this case, white)
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (uint64_t i=0; i<body_count; i++) {
        Vector2d spos = screenpos(sys[i].pos, refcoords, screen_size);
        printf("Screenpos of body %llu is %f, %f\n", i, spos.x, spos.y);
        SDL_RenderDrawPoint(renderer, spos.x, spos.y);
    }
    
    //Finally, we push our changes to the screen
    SDL_RenderPresent(renderer);
}

int update(Body *sys, uint64_t body_count, SDL_Renderer *renderer,  Vector2d screen_size, Time *t, SDL_Event *e)
{
    while(SDL_PollEvent(e)){
        switch (e->type) {
            case SDL_QUIT:
                exit(0);
                return 1;
            default:
                printf("There's an event!\n");
        }
    }
    double dt = 100;
    system_update(sys, body_count, dt, t);
    render_system(sys, body_count, renderer, screen_size);
    *t+=dt;
    return 0;
}

void rungame()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    
    Vector2d screensize = {600,600};
    
    SDL_Window *win = SDL_CreateWindow("Trajectory!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screensize.x, screensize.y, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
    
    bool quit = false;
    Time t=0;
    SDL_Event e;
    
    Body sys[4];
    
    sys[0] = (Body){.mass =  1e15, .pos =  (Vector3d){0, 0, 0}, .vel =  (Vector3d){0,0,0}};
    sys[1] = (Body){.mass =  1e15, .pos =  (Vector3d){1e5, 1e5, 0}, .vel =  (Vector3d){0,0,0}};
    sys[2] = (Body){.mass =  1e15, .pos =  (Vector3d){-1e5, -1e5, 0}, .vel =  (Vector3d){0,0,0}};
    sys[3] = (Body){.mass =  1e5,  .pos =  (Vector3d){0, 1e3, 0}, .vel = (Vector3d){0, -.1, 0}};
    
    while (!quit) {
        SDL_Delay(10);
        if (update(sys, 4, render, screensize, &t, &e)) {
            quit = true;
        }
    }
}