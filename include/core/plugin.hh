#pragma once
#include "core/util.hh"
#include "core/interface.hh"

namespace box {


class Plugin {
public:
    Plugin(te::Plugin *p);
    virtual ~Plugin() = default;

    virtual void Render(Interface &interface) = 0;
    virtual void HandleEvent(const Event &event) = 0;
    te::Plugin *const GetPlugin() const; 

    virtual Texture2D GetIcon() const = 0;
    te::Plugin *const plugin_;
};


} // namespace box
