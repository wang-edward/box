#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"

namespace box {


class Equaliser : public Plugin {
public:
    Equaliser(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::EqualiserPlugin *base_plugin_;
};


} // namespace box
