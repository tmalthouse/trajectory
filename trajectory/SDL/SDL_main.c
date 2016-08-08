//
//  SDL_main.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/13/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "SDL_main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "SDL_coords.h"
#include "../body.h"
#include "../types.h"
#include "../debug.h"
#include "../color.h"

const uint32_t max_fps = 30;

const uint32_t ms_per_frame = 1000/(max_fps);

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

void render_system(Body *sys, uint64_t body_count, SDL_Renderer *renderer, Vector2d screen_size, Vector2d *spos_buffer)
{
    // First, we need to clear the screen and paint it black.
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    //focus_body(sys[3]);
    
    // Then, we set the color to planet color (in this case, white for now)
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (uint64_t i=0; i<body_count; i++) {
        //SDL_RenderDrawPoint(renderer, spos.x, spos.y);
        Color c = sys[i].color;
        filledCircleRGBA(renderer, spos_buffer[i].x, spos_buffer[i].y, 2, c.r, c.g, c.b, c.a);
    }
    
    //Finally, we push our changes to the screen
    SDL_RenderPresent(renderer);
}

void calculate_spositions(Body *sys, Vector2d *spos_buffer, uint64_t count)
{
    for (uint64_t i=0; i<count; i++) {
        Vector2d spos = calculate_screencoord(sys[i].pos);
        spos_buffer[i] = spos;
    }
}

int event_handler(SDL_Event *e, uint64_t *steps, Vector2d *spos_buffer, Body *sys, uint64_t count)
{
    while(SDL_PollEvent(e)){
        switch (e->type) {
            case SDL_QUIT:
                return 1;
                break;
                
            case SDL_MOUSEWHEEL:
            {
                SDL_MouseWheelEvent mousewheel = e->wheel;
                if (mousewheel.y>0) {
                    scale_display(1.5);
                } else {
                    scale_display(0.66);
                }
            } break;
            
            case SDL_KEYDOWN:
            {
                switch (e->key.keysym.sym) {
                    case SDLK_UP:
                        change_shift(UP);
                        break;
                    case SDLK_DOWN:
                        change_shift(DOWN);
                        break;
                    case SDLK_LEFT:
                        change_shift(LEFT);
                        break;
                    case SDLK_RIGHT:
                        change_shift(RIGHT);
                        break;
                    case SDLK_PERIOD:
                        *steps+=5;
                        break;
                    case SDLK_COMMA:
                        if (*steps<5) {
                            *steps=0;
                        } else {
                            *steps-=5;
                        }
                    default:
                        break;
                }
            }
                
            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_MouseButtonEvent event = e->button;
                Vector2d mousepos = {event.x, event.y};
                int pointed_item = item_pointed_at(spos_buffer, count, mousepos, 10);
                if (pointed_item != -1) {
                    focus_body(&sys[pointed_item]);
                }
            }
            default:;
                //dblogger("Event detected");
        }
    }
    return 0;
}

int update(Body *sys, uint64_t body_count, SDL_Renderer *renderer,  Vector2d screen_size, Time *t, SDL_Event *e, Vector2d *spos_buffer)
{
    static uint64_t steps;
    
    Time dt = 1000;
    for (int i=0; i<steps; i++) {
        system_update(sys, body_count, dt, t);
        *t+=dt;
    }
    
    calculate_spositions(sys, spos_buffer, body_count);
    
    if (event_handler(e, &steps, spos_buffer, sys, body_count)) {
        return 1;
    }
    
    render_system(sys, body_count, renderer, screen_size, spos_buffer);
    shift_display();
    return 0;
}


void rungame(SolarSystem syss)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    
    int scrx, scry;
    
    
    SDL_Window *win = SDL_CreateWindow("Trajectory!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_GetWindowSize(win, &scrx, &scry);
    Vector2d screensize = {scrx, scry};
    
    SDL_Renderer *render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
    
    bool quit = false;
    Time t=0;
    uint32_t last_update_time = SDL_GetTicks();
    SDL_Event e;
    
    Body sys[11];
    /*
    sys[0] = (Body){.mass =  1e8, .pos =  (Vector3d){0, 0, 0}, .vel =  (Vector3d){0,0,0}};
    sys[1] = (Body){.mass =  1, .pos =  (Vector3d){1e3, 1e3, 0}, .vel =  (Vector3d){0,.01,0}};
    sys[2] = (Body){.mass =  1, .pos =  (Vector3d){-1e3, -1e3, 0}, .vel =  (Vector3d){.002,0,0}};
    sys[3] = (Body){.mass =  1,  .pos =  (Vector3d){5e2, 5e2, 0}, .vel = (Vector3d){-.001, 0, 0}};
    */
    sys[0] = (Body){.name="Sun", .mass=1.988e30, .vel = {20.0,0.0,0.0}, .color = hex_to_color(COLOR_YELLOW)};
    sys[1] = (Body){.name="Mercury", .mass=3.3e23, .vel={47362,0,0}, .pos={0,-5.7e10,0}, .color = hex_to_color(COLOR_GRAY)};
    sys[2] = (Body){.name="Venus", .mass=4.87e24, .vel={0,35002,0}, .pos={1.08e11, 0,0}, .color = hex_to_color(COLOR_ORANGE)};
    sys[3] = (Body){.name="Earth", .mass=5.97e24, .vel={-29780, 0, 0}, .pos={0, 1.49e11,0}, .color = hex_to_color(COLOR_BLUE)};
    sys[4] = (Body){.name="Mars", .mass=6.42e23, .vel={0, -24000,0}, .pos={-2.27e11, 0,0}, .color = hex_to_color(COLOR_RED)};
    sys[5] = (Body){.name="Jupiter", .mass=1.89e27, .vel={-13070,0,0}, .pos={0, 7.41e11,0}, .color = hex_to_color(COLOR_ORANGE)};
    sys[6] = (Body){.name="Saturn", .mass=5.68e26, .vel={9690,0,0}, .pos={0,-1.509e12,0}, .color = hex_to_color(COLOR_YELLOW)};
    sys[7] = (Body){.name="Uranus", .mass=8.68e25, .vel={6800,0,0}, .pos={0,-2.875e12,0}, .color = hex_to_color(COLOR_TEAL)};
    sys[8] = (Body){.name="Neptune", .mass=1.02e26, .vel={5430,0,0}, .pos={0,-4.504e12,0}, .color = hex_to_color(COLOR_BLUE)};
    sys[9] = (Body){.name="Pluto", .mass=1.3e23, .vel={6100, 0,0}, .pos={0,-4.436e12,0}, .color = hex_to_color(COLOR_GRAY)};
    
    sys[10] = (Body){.name="Moon", .mass=7.3e22, .vel=v3d_vsum(sys[3].vel, (Vector3d){0,1022,0}), .pos=v3d_vsum(sys[3].pos, (Vector3d){384399000,0,0}), .color=hex_to_color(COLOR_BLUE)};
    int bodycount = 11;
    
    set_screensize(screensize);
    Vector2dPair minmax = min_max_xy_coords(sys, bodycount);
    set_minmax_coords(minmax.a, minmax.b);
    Vector2d screen_positions[bodycount];
    
    while (!quit) {
        if (update(sys, bodycount, render, screensize, &t, &e, screen_positions)) {
            quit = true;
        }
        
        //If we haven't elapsed the minimum per frame yet
        while (last_update_time + ms_per_frame > SDL_GetTicks()) {
            //Sleep 1 ms before checking again
            SDL_Delay(5);
        }
        last_update_time = SDL_GetTicks();
    }
    
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    end_logger();
}

