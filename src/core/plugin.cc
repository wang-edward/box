#include "core/plugin.hh"

namespace box {

Plugin:: Plugin(te::Plugin *p)
: plugin_{p}
{}

Plugin:: ~Plugin()
{
    plugin_->deleteFromParent();
}

te::Plugin *const Plugin:: GetPlugin() const
{
    return plugin_;
}

}
