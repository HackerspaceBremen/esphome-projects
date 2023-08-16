#ifndef HSHB_PIXELS_EFFECTS_H
#define HSHB_PIXELS_EFFECTS_H

#include <algorithm>
#include <unordered_map>

constexpr const int TIDE_REFRESH_INTERVAL{300};  // seconds
bool request_tide_data{true}, tide_data_error{false};
unsigned long last_tide_request{0};
float tide_value{0.0};
int16_t tide_pixel_value{0};
std::string tide_unit{""}, tide_shortname{""};

class HSHBPixelEffect {
 public:
  virtual float get_brightness() = 0;

  virtual bool enable_display();
  virtual const char *get_light_effect() { return "None"; }

  virtual void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                          uint64_t frame);
  virtual void update_display(esphome::display::Display &it, uint64_t frame);

  virtual bool is_valid() { return true; }

  virtual Color get_light_color() { return Color(255, 255, 255); }
};

class HSHBEsphomeLightEffect : public HSHBPixelEffect {
 public:
  bool enable_display() override { return false; }

  void update_display(esphome::display::Display &it, uint64_t frame) override {}
};

class HSHBNoneEffect : public HSHBEsphomeLightEffect {
 public:
  float get_brightness() override { return 0.5; }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "No effect");
  }
};

class HSHBTwinkleEffect : public HSHBEsphomeLightEffect {
 public:
  float get_brightness() override { return 1.0; }

  const char *get_light_effect() override { return "Twinkle"; }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Twinkle effect");
  }

  virtual Color get_light_color() override { return Color(110, 255, 160); }
};

class HSHBRainbowEffect : public HSHBEsphomeLightEffect {
 public:
  float get_brightness() override { return 0.85; }

  const char *get_light_effect() override { return "Rainbow"; }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Rainbow effect");
  }
};

class HSHBScanEffect : public HSHBEsphomeLightEffect {
 public:
  float get_brightness() override { return 0.6; }

  const char *get_light_effect() override { return "Scan"; }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Scan effect");
  }

  virtual Color get_light_color() override { return Color(255, 127, 0); }
};

enum class HSHBShapeType { CIRCLE, RECTANGLE };
struct HSHBShape {
  HSHBShape() {
    type = (random_uint32() % 2) == 0 ? HSHBShapeType::CIRCLE
                                      : HSHBShapeType::RECTANGLE;
    size = 11;
    color = Color::random_color();
  }
  HSHBShapeType type;
  int size;
  Color color;
};
std::vector<HSHBShape> shapes_state{};
uint64_t next_shapes_advance{}, next_shapes_emplace{};
constexpr const int SHAPES_ADVANCE{3};
constexpr const int SHAPES_EMPLACE_ADVANCE{8};

// WIP, not yet added to dropdown/demo
class HSHBShapesEffect : public HSHBPixelEffect {
 public:
  float get_brightness() override { return 0.5; }

  bool enable_display() override { return true; }

  void update_display(esphome::display::Display &it, uint64_t frame) override {
    const auto advance_shapes = (frame >= next_shapes_advance ||
                                 next_shapes_advance - frame > SHAPES_ADVANCE);
    const auto emplace_shape =
        (frame >= next_shapes_emplace ||
         next_shapes_emplace - frame > SHAPES_EMPLACE_ADVANCE);

    for (auto &shape : shapes_state) {
      if (shape.type == HSHBShapeType::CIRCLE) {
        it.circle(7, 11, shape.size, shape.color);
      } else if (shape.type == HSHBShapeType::RECTANGLE) {
        it.rectangle(6 - shape.size, 10 - shape.size, 3 + 2 * shape.size,
                     3 + 2 * shape.size, shape.color);
      }
      if (advance_shapes) {
        shape.size--;
      }
    }

    if (emplace_shape) {
      shapes_state.emplace(shapes_state.begin());
      next_shapes_emplace = frame + SHAPES_EMPLACE_ADVANCE;
    }
    if (advance_shapes) {
      next_shapes_advance = frame + SHAPES_ADVANCE;

      for (std::vector<HSHBShape>::iterator it = shapes_state.begin();
           it != shapes_state.end();) {
        HSHBShape &shape = *it;
        if (shape.size < 1) {
          it = shapes_state.erase(it);
        } else {
          ++it;
        }
      }
    }
  }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Shapes effect");
  }
};

constexpr const uint8_t OUTLINE_COLOR_COUNT{3};
std::array<Color, OUTLINE_COLOR_COUNT> outline_colors{
    Color(20, 200, 115),
    Color(200, 120, 20),
    Color(150, 20, 200),
};
const auto OUTLINE_TOTAL_LENGTH = 2 * OUTLINE_COLOR_COUNT * OUTLINE_PIXEL_COUNT;

class HSHBOutlineEffect : public HSHBPixelEffect {
 public:
  float get_brightness() override { return 0.7; }

  bool enable_display() override { return true; }

  void update_display(esphome::display::Display &it, uint64_t frame) override {
    const auto color_index =
        ((frame % OUTLINE_TOTAL_LENGTH) / (2 * OUTLINE_PIXEL_COUNT));
    auto current_color = outline_colors[color_index];
    auto position = frame % OUTLINE_PIXEL_COUNT;
    auto going_dark = frame % (2 * OUTLINE_PIXEL_COUNT) >= OUTLINE_PIXEL_COUNT;

    if (!going_dark) {
      for (int i = 0; i < position; i++) {
        it.draw_pixel_at(OUTLINE_PIXEL_COORDS[2 * i],
                         OUTLINE_PIXEL_COORDS[2 * i + 1], current_color);
      }
    } else {
      for (int i = position; i < OUTLINE_PIXEL_COUNT; i++) {
        it.draw_pixel_at(OUTLINE_PIXEL_COORDS[2 * i],
                         OUTLINE_PIXEL_COORDS[2 * i + 1], current_color);
      }
    }
  }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Outline effect");
  }
};

class HSHBTextEffect : public HSHBPixelEffect {
 public:
  float get_brightness() override { return 0.7; }

  bool enable_display() override { return true; }

  void update_display(esphome::display::Display &it, uint64_t frame) override {
    frame %= (72 * 5);
    it.print(15 - (frame / 4), 11, &id(font_3x5), Color(20, 200, 115),
             "Hackerspace Bremen");
  }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Text effect");
  }
};

std::vector<Color> fade_colors{};
std::array<int, 15> matrix_state{};
uint64_t next_matrix_advance{};
constexpr const int MATRIX_ADVANCE{2};

class HSHBMatrixEffect : public HSHBPixelEffect {
 public:
  float get_brightness() override { return 0.7; }

  bool enable_display() override { return true; }

  void update_display(esphome::display::Display &it, uint64_t frame) override {
    auto green = Color(80, 255, 80);
    if (fade_colors.size() == 0) {
      fade_colors.push_back(Color(20, 220, 20));
      for (int i = 0; i < 9; i++) {
        fade_colors.push_back(fade_colors[i].fade_to_black(40));
      }
    }

    if (frame >= next_matrix_advance ||
        next_matrix_advance - frame > MATRIX_ADVANCE) {
      for (int i = 0; i < 15; i++) {
        if (matrix_state[i] > 0) {
          matrix_state[i]++;
          if (matrix_state[i] > 20 + fade_colors.size()) {
            matrix_state[i] = 0;
          }
        }
      }
      uint32_t rand_pos = random_uint32() % 15;
      if (matrix_state[rand_pos] == 0) {
        matrix_state[rand_pos] = 1;
      }
      next_matrix_advance = frame + MATRIX_ADVANCE;
    }

    for (int i = 0; i < 15; i++) {
      if (matrix_state[i] > 0) {
        it.draw_pixel_at(i, matrix_state[i], green);
        for (int j = 0; j < fade_colors.size(); j++) {
          it.draw_pixel_at(i, matrix_state[i] - j - 1, fade_colors[j]);
        }
      }
    }
  }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    it.print(0, 2, "Matrix effect");
  }
};

class HSHBTideEffect : public HSHBPixelEffect {
 public:
  float get_brightness() override { return 0.7; }

  bool enable_display() override { return true; }

  void update_display(esphome::display::Display &it, uint64_t frame) override {
    if (tide_shortname == "" ||
        (millis() - last_tide_request) > 1000 * TIDE_REFRESH_INTERVAL) {
      request_tide_data = true;
      if (tide_shortname == "") {
        tide_shortname = "?";
      }
    }
    it.image(0, 0, &id(waves_animation));
    auto black = Color(0, 0, 0);
    auto partition_y = 19 - tide_pixel_value;
    auto wave_top_img = &id(wave_top);
    auto wave_top_x = (frame / 5) % 4;

    if (partition_y < 20) {
      it.filled_rectangle(0, 0, 15, partition_y, black);
    }

    it.image(wave_top_x - 4, partition_y, wave_top_img, black);
    it.image(wave_top_x, partition_y, wave_top_img, black);
    it.image(wave_top_x + 4, partition_y, wave_top_img, black);
    it.image(wave_top_x + 8, partition_y, wave_top_img, black);
    it.image(wave_top_x + 12, partition_y, wave_top_img, black);
    it.image(wave_top_x + 16, partition_y, wave_top_img, black);
  }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it,
                  uint64_t frame) override {
    if (tide_shortname != "") {
      auto next_update = std::max(
          0UL, TIDE_REFRESH_INTERVAL - (millis() - last_tide_request) / 1000);
      it.printf(0, 2, "Tide: %.0f %s  %02d:%02d", tide_value, tide_unit.c_str(),
                next_update / 60, next_update % 60);
      it.print(0, 3, tide_shortname.c_str());
    } else if (tide_data_error) {
      it.print(0, 2, "Tide: Error :(");
    } else {
      it.print(0, 2, "Tide: Refreshing...");
    }
  }

  bool is_valid() override { return !tide_data_error; }
};

constexpr const int FRAMES_PER_SECOND{20};
std::vector<std::pair<std::string, uint64_t>> demo_modes{
    std::make_pair("Outline", OUTLINE_TOTAL_LENGTH),
    std::make_pair("Matrix", FRAMES_PER_SECOND * 25),
    std::make_pair("Text", FRAMES_PER_SECOND * 20),
    std::make_pair("Scan", FRAMES_PER_SECOND * 20),
    std::make_pair("Tide", FRAMES_PER_SECOND * 30),
    std::make_pair("Rainbow", FRAMES_PER_SECOND * 20),
    std::make_pair("Twinkle", FRAMES_PER_SECOND * 30),
};

class HSHBPixelEffects {
 public:
  HSHBPixelEffects() : m_effect{"Demo"} { set_effect(m_effect); }

  ~HSHBPixelEffects() {
    for (const std::pair<const std::string, HSHBPixelEffect *> &n :
         m_effect_list) {
      delete n.second;
    }
  }

  [[nodiscard]] std::string get_effect() { return m_effect; }

  void set_effect(const std::string &new_effect) {
    auto found = m_effect_list.find(new_effect);

    if (found != m_effect_list.end()) {
      m_effect = new_effect;
      m_current_effect = m_effect_list[m_effect];
      m_frame = 0;
      m_demo_mode = false;
    } else if (new_effect == "Demo") {
      m_demo_mode = true;
      m_demo_index = 0;
      m_effect = new_effect;
      m_current_effect = m_effect_list[demo_modes[m_demo_index].first];
      apply_effect(&id(led_matrix_light), &id(led_matrix_display));
      m_frame = 0;
    }
  }

  void apply_effect_light(esphome::light::AddressableLightState *light) {
    auto call = light->turn_on();
    call.set_effect(m_current_effect->get_light_effect());
    call.set_brightness(m_current_effect->get_brightness());
    auto color = m_current_effect->get_light_color();
    call.set_red(((float)color.red) / 255.0);
    call.set_green(((float)color.green) / 255.0);
    call.set_blue(((float)color.blue) / 255.0);
    call.perform();
    m_schedule_light_on = false;
  }

  void apply_effect(
      esphome::light::AddressableLightState *light,
      esphome::addressable_light::AddressableLightDisplay *display) {
    if (!light || !display) {
      return;
    }
    auto enable_display = m_current_effect->enable_display();
    display->set_enabled(enable_display);

    if (!enable_display) {
      apply_effect_light(light);
    } else {
      // Turn the light off and turn it back on later in update_display
      // because otherwise the light flashes bright for a frame
      auto call = light->turn_off();
      call.perform();
      m_schedule_light_on = true;
    }
  }

  void update_lcd(esphome::lcd_gpio::GPIOLCDDisplay &it) {
    m_current_effect->update_lcd(it, m_frame);
  }

  void update_display(esphome::display::Display &it) {
    m_current_effect->update_display(it, m_frame);

    if (m_frame > 0 && m_schedule_light_on) {
      apply_effect_light(&id(led_matrix_light));
      m_schedule_light_on = false;
    }
  }

  void advance_frame() {
    m_frame++;
    if (m_demo_mode && (m_frame > demo_modes[m_demo_index].second ||
                        !m_current_effect->is_valid())) {
      m_demo_index++;
      m_demo_index %= (demo_modes.size());
      m_current_effect = m_effect_list[demo_modes[m_demo_index].first];
      m_frame = 0;
      apply_effect(&id(led_matrix_light), &id(led_matrix_display));
    }
  }

 private:
  std::string m_effect;
  std::unordered_map<std::string, HSHBPixelEffect *> m_effect_list = {
      {"None", new HSHBNoneEffect()},
      {"Twinkle", new HSHBTwinkleEffect()},
      {"Rainbow", new HSHBRainbowEffect()},
      {"Scan", new HSHBScanEffect()},
      {"Outline", new HSHBOutlineEffect()},
      {"Text", new HSHBTextEffect()},
      {"Matrix", new HSHBMatrixEffect()},
      {"Tide", new HSHBTideEffect()},
      {"Shapes", new HSHBShapesEffect()},
  };
  HSHBPixelEffect *m_current_effect{nullptr};

  uint64_t m_frame{0}, m_demo_index{0};
  bool m_demo_mode{}, m_schedule_light_on{};
};

void replace(std::string &str, const std::string &from, const std::string &to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos) {
    return;
  }
  str.replace(start_pos, from.length(), to);
}

void sanitize_string(std::string &str) {
  // Replace various chars the LCD cannot display
  replace(str, "Ä", "AE");
  replace(str, "ä", "ae");
  replace(str, "Ö", "OE");
  replace(str, "ö", "oe");
  replace(str, "Ü", "UE");
  replace(str, "ü", "ue");
  replace(str, "ß", "SS");
}

void parse_tide_response(int status_code) {
  last_tide_request = millis();
  if (status_code == 200) {
    auto str = id(http_request_data).get_string();
    ESP_LOGD("effects", "HTTP Response: %s", str);
    tide_shortname = "?";
    tide_value = 0.0;
    tide_unit = "?";
    DynamicJsonDocument doc(2048);
    if (deserializeJson(doc, str) == DeserializationError::Ok) {
      tide_shortname = (const char *)doc["shortname"];
      tide_value = doc["timeseries"][0]["currentMeasurement"]["value"];
      tide_unit = (const char *)doc["timeseries"][0]["unit"];

      sanitize_string(tide_shortname);
      sanitize_string(tide_unit);

      ESP_LOGD("effects", "Tide Shortname: %s", tide_shortname.c_str());
      ESP_LOGD("effects", "Tide Value: %.2f %s", tide_value, tide_unit.c_str());

      // Tide usually between 300 and 850
      tide_pixel_value = map(tide_value, 300, 850, 2, 19);
      ESP_LOGD("effects", "Mapped tide value to %d", tide_pixel_value);
      tide_data_error = false;
    } else {
      ESP_LOGD("effects", "JSON parse failed");
    }
  } else {
    ESP_LOGD("effects", "HTTP error: %d", status_code);
    tide_data_error = true;
  }
}

HSHBPixelEffects pixel_effects;

#endif
