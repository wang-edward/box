#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Phaser : public PluginManager {
public:
    Phaser(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::PhaserPlugin *base_plugin_;
};


} // namespace box
