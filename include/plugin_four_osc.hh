#pragma once
#include "interface.hh"
#include "plugin_manager.hh"

namespace box {

class FourOscManager : public PluginManager {
public:
    FourOscManager(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
};

} // namespace box
