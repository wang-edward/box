#include "util.hh"

namespace box {

std::string to_string(ScreenType screenType) {
    switch (screenType) {
        case ScreenType::GraphicsDemo1Stripe:
            return "GraphicsDemo1Stripe";
            break;
        case ScreenType::GraphicsDemo2Bomb:
            return "GraphicsDemoBomb";
            break;
    }
}

} // namespace box
