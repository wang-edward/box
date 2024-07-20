#pragma once
#include "util.hh"
#include "interface.hh"

namespace box {

class PluginManager {
public:
    PluginManager(te::Plugin *p);

    virtual void Render(Interface &interface) = 0;
    virtual void HandleEvent(const Event& event) = 0;
    te::Plugin *const GetPlugin() const; 

    virtual ~PluginManager() {}
private:
    te::Plugin *const plugin_;
};

} // namespace box
