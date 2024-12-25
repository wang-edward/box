#pragma once
#include "core/plugin_selector.hh"
#include "core/timeline.hh"
#include "core/track.hh"
#include "core/util.hh"

namespace box {

struct App {
  enum class ScreenState {
    Timeline,
    Track,
    PluginSelector,
  };

  enum class Mode {
    Normal,
    Insert,
  };

  ScreenState screen_state_ = ScreenState::Timeline;
  Mode mode_ = Mode::Normal;
  int key_offset_ = 0;
  std::map<int, int> active_notes_;

  te::Engine &engine_;
  te::Edit &edit_;

  std::vector<std::unique_ptr<Track>> tracks_;
  juce::Array<te::AudioTrack *> base_tracks_;

  PluginSelector plugin_sel_;
  Timeline timeline_;

  const std::vector<Color> colors_ = {
      Color{0x29, 0x73, 0x55, 0xff},
      Color{0x1e, 0x4d, 0x3f, 0xff},
      Color{0xdd, 0xe6, 0x63, 0xff},
      Color{0x7d, 0x54, 0x39, 0xff},
  };

  // functions
  App(te::Engine &engine, te::Edit &edit);
  void Render(Interface &interface);
  void HandleEvent(const Event &event);

  Track &CurrTrack();
  size_t GetCurrTrack();
  void SetCurrTrack(size_t track_index);
  void AddTrack();
  void ChangeArmMidi(size_t new_index);

 private:
  size_t current_track_;
  void ArmMidi(size_t index);
  void UnarmMidi(size_t index);
};

}  // namespace box
