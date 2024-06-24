enum class EventType {A, B}


struct Event {
    EventType type;
};

class Screen {
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void handleEvent(const Event &event) = 0;
    virtual ~Screen() {}
};
