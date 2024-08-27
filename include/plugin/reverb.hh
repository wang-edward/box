#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"
// automatable pointer takes precedence over cached value
// automatable pointer works for float
// when you make a change, inserts filler value to avoid click while a block is being processed
namespace box {
class Reverb : public PluginManager {
public:
    Reverb(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::ReverbPlugin *base_plugin_;
};
}