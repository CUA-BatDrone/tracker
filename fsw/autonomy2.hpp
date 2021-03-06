#ifndef AUTONOMY2_HPP
#define AUTONOMY2_HPP
#define COLS 80
#define ROWS 60
#include <stdint.h>
#include <iostream>
#include <string>
#include "control_arbiter.hpp"
#include "control.hpp"
#include "sender.hpp"
#include "triple_buffer.hpp"
#include <list>
#include "pid_controller.hpp"


class Autonomy2 {
public:
  class Blob {
  public:
    float x, y;
    int size;
    Blob();
    void addPixel(int x, int y);
    void calculateCentroid();
  };
protected:
  void threshold(bool out[ROWS][COLS], const uint16_t in[ROWS][COLS]);
  list<Blob> findBlobs(bool in[ROWS][COLS]);
  void blobListToCmdBlobVect(std::vector<Commands::Blob> &commandBlobs, std::list<Autonomy2::Blob> &blobs);
  Blob findLargestBlob(std::list<Autonomy2::Blob> &blobs);
  Control calculateFlightControls(Blob blob);
  Control calculateFlightControlsPID(Blob blob);
  TripleBuffer<Control> receivedControlBuffer;
  // The target blob the user selects to track
  TripleBuffer <Blob> receivedTargetBuffer;
  float targetDist;
  uint16_t thresholdLow;
  uint16_t thresholdHigh;
  PIDController<float> aPID, ePID, tPID, rPID;
  mutex pidMutex;
public:
  Autonomy2(ControlArbiter &a, Sender &send) : arbiter(a), send(send), thresholdLow(2800), thresholdHigh(3600) {}
	ControlArbiter &arbiter;
	Sender &send;
	void giveFrame(uint16_t frame[ROWS][COLS]);
  void giveControl(Control control);
  void giveTarget(int x, int y);
  void givePID(float p1, float i1, float d1, float p2, float i2, float d2, float p3, float i3, float d3, float p4, float i4, float d4);
  void giveThreshold(uint16_t low, uint16_t high);
	TripleBuffer < uint16_t [ROWS][COLS] > buffer;
  void mainLoop(bool & run);
};

typedef Autonomy2 Autonomy;
#endif