#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"

namespace box {


class Phaser : public Plugin {
public:
    Phaser(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::PhaserPlugin *base_plugin_;
};


} // namespace box
