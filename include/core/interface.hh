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

    bool PollEvent(Event& event);
    bool ShouldClose() const;

private:
    RenderTexture2D target_;
    // std::array<size_t, KEY_LAST + 1> key_states_{}; // TODO get rid of this?
    const std::vector<KeyboardKey> keys_ = {
        KEY_ZERO,
        KEY_ONE,
        KEY_TWO,
        KEY_THREE,
        KEY_FOUR,
        KEY_FIVE,
        KEY_SIX,
        KEY_SEVEN,
        KEY_EIGHT,
        KEY_NINE,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        KEY_SPACE,
        KEY_ESCAPE,
        KEY_ENTER,
        KEY_TAB,
        KEY_BACKSPACE,
        KEY_COMMA,
        KEY_PERIOD,
        KEY_DELETE,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_DOWN,
        KEY_UP,
    };

};

} // namespace box
