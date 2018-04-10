#include "telemetry_handler.hpp"
#include "ui.hpp"
#include <cmd_tlm.hpp>
#include <iostream>

using namespace std;

TelemetryHandler::TelemetryHandler(CmdTlm &cmdtlm, UI &ui) : cmdtlm(cmdtlm), ui(ui) {
  run = true;
}

void TelemetryHandler::mainLoop() {
  //try {
    class CommandListener : public Commands {
    public:
      UI & ui;
      CommandListener(UI &ui) : ui(ui) {
      }
      void lwirFrame(const uint16_t frame[60][80]) {
        ui.updateTexture(frame);
      }
      void blob(uint16_t x, uint16_t y) {
        cout << "XY: " << x << ", " << y << endl;
      }
    } cl(ui);
    while (run) {
      cmdtlm.telemetry(cl);
    }
  //} catch (string e) {
    //cout << e << endl;
  //}
}

void TelemetryHandler::start() {
  tlmthread = thread(&TelemetryHandler::mainLoop, this);
}

void TelemetryHandler::stop() {
  run = false;
}

void TelemetryHandler::join() {
  tlmthread.join();
}