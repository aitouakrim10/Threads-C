#ifndef THEORA_H
#define THEORA_H

#include <map>
using namespace std;

#include "ensivideo.hpp"
#include <SDL2/SDL.h>
#include <ogg/ogg.h>

constexpr int NBTEX= 30;

struct TextureDate {
  unsigned char *plane[3]; // copy of plane are contiguous, thus
                           // stride is width
  double timems;           // expiration date of the frame, in ms
};

extern map<int, struct streamstate *> maptheorastrstate;

void draw2SDL(int serial);
void theora2SDL(struct streamstate *s);

extern int windowsx;
extern int windowsy;

#endif
