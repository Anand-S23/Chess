#ifndef UI_H
#define UI_H

typedef struct ui
{
    // renderer
    SDL_Renderer *renderer; 

    // mouse info
    int mouse_x; 
    int mouse_y; 
    b32 mouse_down; 

    // ui color
    v4 primary_color;   // base color of ui 
    v4 secondary_color; // hot color of ui 
} ui;

internal ui CreateUI(SDL_Renderer *renderer, platform *platform);
internal void SetUIColor(ui *ui, v4 primary, v4 secondary);
internal void SetUIPrimary(ui *ui, v4 primary);
internal void SetUISecondary(ui *ui, v4 secondary);
internal b32 Button(ui *ui, v4 rect, SDL_Texture *play);
internal f32 Slider(ui *ui, v4 rect);

#endif