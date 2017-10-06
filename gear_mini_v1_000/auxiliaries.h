#ifndef AUX_H
#define AUX_H

void selectorInput();
void gearStatus();
void gearOutput();
void stepgear();
void driveOutputs();
void outputaction(uint8_t chanX);
void initialOutputs();

  bool state;
  bool invstate;
  uint8_t portX;

#endif
