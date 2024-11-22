#pragma once
#include "core/util.hh"
#include "core/interface.hh"

namespace box {


struct Plugin {
    te::Plugin *const plugin_;

public:
    Plugin(te::Plugin *p);
    virtual ~Plugin();

    void EnsureTextureLoaded() const;
    te::Plugin *const GetPlugin() const; 

    virtual Texture2D &GetIcon() const = 0;
    virtual const char *GetIconPath() const = 0;
    virtual void Render(Interface &interface) = 0;
    virtual void HandleEvent(const Event &event) = 0;
};


} // namespace box
