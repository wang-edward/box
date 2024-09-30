#include "core/plugin.hh"

namespace box {

Plugin:: ~Plugin() {
    UnloadTexture(icon_);
}

}
