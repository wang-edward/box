#include "core/interface.hh"

#include <algorithm>
#include <iostream>

namespace box
{

Interface::Interface()
{
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "raylib - Rescalable 128x128 render");
    // target_ = LoadRenderTexture(WIDTH, HEIGHT);
    // SetTargetFPS(60);
    // SetExitKey(0); // esc doesn't close program

    // if constexpr (INPUT_TYPE == DeviceType::Hardware)
    // {
    //     // Initialize microcontroller input handling (e.g., serial interface)
    // }
}

Interface::~Interface()
{
    if (DISPLAY_TYPE == DeviceType::Emulator)
    {
        UnloadRenderTexture(target_);
        CloseWindow();
    }
}

void Interface::PreRender()
{
    BeginTextureMode(target_); // matches PostRender
    ClearBackground(BLACK);
}

void Interface::PostRender()
{
    if (DISPLAY_TYPE == DeviceType::Emulator)
    {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        int square_len = std::min(screen_width, screen_height);
        int pos_x = (screen_width - square_len) / 2;
        int pos_y = (screen_height - square_len) / 2;

        EndTextureMode(); // matches PreRender
        BeginDrawing();
        DrawTexturePro(target_.texture, Rectangle{0, 0, float(target_.texture.width), float(-target_.texture.height)},
                       Rectangle{float(pos_x), float(pos_y), float(square_len), float(square_len)}, Vector2{0, 0}, 0.0f,
                       WHITE);
        EndDrawing();
        // hardware rendering
    }
    else if (DISPLAY_TYPE == DeviceType::Hardware)
    {
        Image image = LoadImageFromTexture(target_.texture);
        Color c = GetImageColor(image, 0, 0); // get pixel (0, 0)
    }
}

// this code has really bad performance. keys often get stuck,
// but for the purpose of being an emulator this is fine.
bool Interface::PollEvent(Event &event)
{
    for (auto &[key, is_pressed] : keys_)
    {
        if (IsKeyDown(key))
        {
            if (is_pressed == false)
            {
                is_pressed = !is_pressed;
                event.type = EventType::KeyPress;
                event.value = key;
                return true;
            }
        }
        if (IsKeyUp(key))
        {
            if (is_pressed == true)
            {
                is_pressed = !is_pressed;
                event.type = EventType::KeyRelease;
                event.value = key;
                return true;
            }
        }
    }

    return false;
}

bool Interface::ShouldClose() const
{
    return WindowShouldClose();
}

} // namespace box
