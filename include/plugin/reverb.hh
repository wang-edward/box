#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Reverb : public Plugin {
public:
    Reverb(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::ReverbPlugin *base_plugin_;
};


} // namespace box
