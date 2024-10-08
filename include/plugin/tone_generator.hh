#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"

namespace box {


class ToneGenerator : public Plugin {
public:
    ToneGenerator(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::ToneGeneratorPlugin *base_plugin_;
};


} // namespace box
