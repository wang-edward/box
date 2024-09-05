#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Latency : public Plugin {
public:
    Latency(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::LatencyPlugin *base_plugin_;
};


} // namespace box
