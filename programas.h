#ifndef PROGRAMAS_H
#define PROGRAMAS_H

// Definição da estrutura
struct ProgramParameters {
  uint16_t timeObverse;  // tempo obverse (s)
  uint16_t timeReverse;  // tempo reverse (s)
  uint16_t timePaused;   // tempo de pausa (s)
  uint8_t cycles;        // número de ciclos
};

#define NUM_PROGRAMS 4

// Declaração do array de programas
extern ProgramParameters programs[NUM_PROGRAMS];

#endif
