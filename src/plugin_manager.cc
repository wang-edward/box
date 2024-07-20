#include "plugin_manager.hh"

namespace box {

PluginManager:: PluginManager(te::Plugin *p): plugin_{p} {}

te::Plugin *const PluginManager:: GetPlugin() const {
    return plugin_;
}

} // namespace box
