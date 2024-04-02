#ifndef ENSIVORBIS_H
#define ENSIVORBIS_H

#include <map>

extern std::map<int, struct streamstate *> mapvorbisstrstate;
extern void vorbis2SDL(struct streamstate *s);

#endif
