#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Compressor : public PluginManager {
public:
    Compressor(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::CompressorPlugin *base_plugin_;
};


} // namespace box
