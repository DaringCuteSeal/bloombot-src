
class PumpController
{
public:
    bool poll();

    void water();
    void start_pump();

    void stop_pump();
};

void start_pump();
void stop_pump();
bool poll_pump();