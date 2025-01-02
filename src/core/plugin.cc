#include "core/plugin.hh"

namespace box
{

Plugin::Plugin(te::Plugin *p)
    : plugin_{p}
{
}

Plugin::~Plugin()
{
    plugin_->deleteFromParent();
}

void Plugin::EnsureTextureLoaded() const
{
    if (GetIcon().id == 0)
    {
        Image icon = LoadImage(GetIconPath());
        GetIcon() = LoadTextureFromImage(icon);
        UnloadImage(icon);
    }
}

te::Plugin *const Plugin::GetPlugin() const
{
    return plugin_;
}

} // namespace box
