#include "core/track.hh"

#include <algorithm>

#include "core/app.hh"
#include "core/util.hh"

namespace box
{

static void assert_plugins(const std::vector<std::unique_ptr<Plugin>> &plugins, int index, std::string function_name)
{
    if (index >= plugins.size())
    {
        throw std::runtime_error{"Track::" + function_name + " => index [" + std::to_string(index) +
                                 "] out of range: [0, " + std::to_string(plugins.size()) + "]"};
    }
}

Track::Track(te::AudioTrack &base)
    : base_{base}
{
}

Track::~Track()
{
}

void Track::AddPlugin(std::unique_ptr<Plugin> plugin)
{
    if (active_plugin_ == -1)
        active_plugin_ = 0; // TODO better way to handle this
    int index = plugins_.size();
    base_.pluginList.insertPlugin(plugin->GetPlugin(), index, nullptr);
    plugins_.push_back(std::move(plugin));
}

void Track::RemoveActivePlugin()
{
    if (plugins_.size() == 0)
    {
        LOG_MSG("tried to remove plugin from empty track", LogLevel::Warn);
        return;
    }
    if (active_plugin_ == -1)
    {
        LOG_MSG("tried to remove active_plugin == -1", LogLevel::Warn);
        return;
    }
    plugins_.erase(plugins_.begin() + active_plugin_);
    // TODO check for underflow messing up min
    active_plugin_ = std::min(active_plugin_, static_cast<int>(plugins_.size()) - 1);
}

void Track::SetActivePlugin(int index)
{
    active_plugin_ = index;
}

int Track::GetActivePlugin()
{
    return active_plugin_;
}

int clamp_index(int i, int len)
{
    int range = len;
    int val = i % range;
    if (val < 0)
        val += range;
    return val;
}

void Track::HandleEvent(const Event &event)
{
    switch (screen_state_)
    {
    case ScreenState::Overview:
        switch (APP->mode_)
        {
        case App::Mode::Normal:
            switch (event.type)
            {
            case EventType::KeyPress:
                switch (event.value)
                {
                case KEY_ESCAPE:
                    APP->screen_state_ = App::ScreenState::Timeline;
                    break;
                case KEY_A:
                    APP->screen_state_ = App::ScreenState::PluginSelector;
                    break;
                case KEY_X:
                    RemoveActivePlugin();
                    break;
                case KEY_P:
                {
                    auto l = base_.pluginList.getPlugins();
                    for (auto p : l)
                    {
                        std::cout << p->getName() << ", ";
                    }
                    std::cout << std::endl;
                }
                break;
                // TODO KEY_J and KEY_K move up and down
                case KEY_H:
                    LOG_MSG("left");
                    active_plugin_ = clamp_index(active_plugin_ - 1, plugins_.size());
                    LOG_VAR(active_plugin_);
                    break;
                case KEY_L:
                    LOG_MSG("right");
                    active_plugin_ = clamp_index(active_plugin_ + 1, plugins_.size());
                    LOG_VAR(active_plugin_);
                    break;
                case KEY_ENTER:
                    // TODO sketchy
                    if (active_plugin_ != -1)
                        screen_state_ = ScreenState::Plugin;
                    break;
                }
                break;
            }
            break;
        case App::Mode::Insert:
            break;
        }
        break;
    case ScreenState::Plugin:
        if (event.type == EventType::KeyPress && event.value == KEY_ESCAPE)
        {
            screen_state_ = ScreenState::Overview;
            return;
        }
        switch (APP->mode_)
        {
        case App::Mode::Normal:
            plugins_[active_plugin_]->HandleEvent(event);
            break;
        case App::Mode::Insert:
            break;
        }
        break;
    }
}

void Track::Render(Interface &interface)
{
    switch (screen_state_)
    {
    case ScreenState::Overview:
        // draw grid
        for (int i = 1; i < GRID_SIZE; i++)
        {
            float pos = static_cast<float>(i);
            DrawLineV(Vector2{0, pos * 32}, Vector2{128, pos * 32}, WHITE);
            DrawLineV(Vector2{pos * 32, 0}, Vector2{pos * 32, 128}, WHITE);
        }

        for (int i = 0; i < plugins_.size(); i++)
        {
            auto x = static_cast<float>((i % 4) * 32 + 16);
            auto y = static_cast<float>((i / 4) * 32 + 64 + 16);
            DrawTexture(plugins_[i]->GetIcon(), x - 8, y - 8, WHITE);

            constexpr int font_size = 10;
            int width = MeasureText(plugins_[i]->GetName(), font_size);
            DrawText(plugins_[i]->GetName(), (x - width / 2), y + 6, font_size, WHITE);

            if (i == active_plugin_)
            {
                DrawCircleV(Vector2{static_cast<float>(i % 4) * 32 + 16, static_cast<float>(i / 4) * 32 + 64 + 16},
                            5.0f, GREEN);
            }
        }
        for (int i = plugins_.size(); i < MAX_PLUGINS; i++)
        {
            auto x = static_cast<float>((i % 4) * 32 + 16);
            auto y = static_cast<float>((i / 4) * 32 + 64 + 16);
            DrawCircleV(Vector2{static_cast<float>(i % 4) * 32 + 16, static_cast<float>(i / 4) * 32 + 64 + 16}, 1.0f,
                        RED);
            DrawTextPro(GetFontDefault(), "none", Vector2{x, y},
                        Vector2{
                            11,
                            -4,
                        },
                        0.0f, 10.0f, 1.0f, WHITE);
        }
        break;
    case ScreenState::Plugin:
        plugins_[active_plugin_]->Render(interface);
        break;
    }
}

} // namespace box
