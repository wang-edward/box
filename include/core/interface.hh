#pragma once
#include <array>

#include "core/util.hh"

namespace box {

class Interface {
  public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;
    constexpr static DeviceType DISPLAY_TYPE = DeviceType::Emulator;
    constexpr static DeviceType INPUT_TYPE = DeviceType::Emulator;

    Interface();
    ~Interface();

    void PreRender();
    void PostRender();

    bool PollEvent(Event &event);
    bool ShouldClose() const;

  private:
    RenderTexture2D target_;
    std::map<KeyboardKey, bool> keys_ = {
        {KEY_ZERO, false},   {KEY_ONE, false},       {KEY_TWO, false},
        {KEY_THREE, false},  {KEY_FOUR, false},      {KEY_FIVE, false},
        {KEY_SIX, false},    {KEY_SEVEN, false},     {KEY_EIGHT, false},
        {KEY_NINE, false},   {KEY_A, false},         {KEY_B, false},
        {KEY_C, false},      {KEY_D, false},         {KEY_E, false},
        {KEY_F, false},      {KEY_G, false},         {KEY_H, false},
        {KEY_I, false},      {KEY_J, false},         {KEY_K, false},
        {KEY_L, false},      {KEY_M, false},         {KEY_N, false},
        {KEY_O, false},      {KEY_P, false},         {KEY_Q, false},
        {KEY_R, false},      {KEY_S, false},         {KEY_T, false},
        {KEY_U, false},      {KEY_V, false},         {KEY_W, false},
        {KEY_X, false},      {KEY_Y, false},         {KEY_Z, false},
        {KEY_SPACE, false},  {KEY_ESCAPE, false},    {KEY_ENTER, false},
        {KEY_TAB, false},    {KEY_BACKSPACE, false}, {KEY_COMMA, false},
        {KEY_PERIOD, false}, {KEY_DELETE, false},    {KEY_MINUS, false},
        {KEY_EQUAL, false},  {KEY_RIGHT, false},     {KEY_LEFT, false},
        {KEY_DOWN, false},   {KEY_UP, false},
    };
};

} // namespace box
