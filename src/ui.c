#include "ui.h"

internal ui CreateUI(SDL_Renderer *renderer, platform *platform)
{
    ui result = {0};
    {
        result.renderer = renderer; 

        result.mouse_x    = platform->mouse_x; 
        result.mouse_y    = platform->mouse_y; 
        result.mouse_down = platform->mouse_down; 

        result.primary_color   = v4(75, 75, 75, 255);
        result.secondary_color = v4(150, 150, 150, 255);
    }

    return result;
}

internal void SetUIColor(ui *ui, v4 primary, v4 secondary)
{
    ui->primary_color   = primary;
    ui->secondary_color = secondary;
}

internal void SetUIPrimary(ui *ui, v4 primary)
{
    ui->primary_color = primary;
}

internal void SetUISecondary(ui *ui, v4 secondary)
{
    ui->secondary_color = secondary;
}


internal b32 Button(ui *ui, v4 rect, SDL_Texture *play)
{
    b32 is_pressed = 0;

    b32 hot = (ui->mouse_x >= rect.x && 
               ui->mouse_x <= rect.x + rect.width &&
               ui->mouse_y >= rect.y && 
               ui->mouse_y <= rect.y + rect.height);

    v4 color;

    if (hot && ui->mouse_down)
    {
        color = ui->secondary_color;
        is_pressed = 1;
    }
    else if (hot)
    {
        color = ui->secondary_color;
    }
    else
    {
        color = ui->primary_color;
    }
    
    RenderFilledRect(ui->renderer, color, rect);
    RenderImage(ui->renderer, play, rect);

    return is_pressed;
}

internal f32 Slider(ui *ui, v4 rect)
{
    f32 slider_value;
    local_persist v4 slider = {0};
    {
        slider.x = rect.x;
        slider.y = rect.y;
        slider.height = rect.height;
    }

    b32 hot = (ui->mouse_x >= rect.x && 
               ui->mouse_x <= rect.x + rect.width &&
               ui->mouse_y >= rect.y && 
               ui->mouse_y <= rect.y + rect.height);

    if (hot && ui->mouse_down)
    {
        slider.width = ui->mouse_x - rect.x;
    }
    
    RenderFilledRect(ui->renderer, ui->primary_color, rect);
    RenderFilledRect(ui->renderer, ui->secondary_color, slider);

    slider_value = (f32)slider.width / (f32)rect.width;
    return slider_value;
}



