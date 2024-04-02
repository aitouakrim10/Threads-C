#ifndef STREAM_COMMON_H
#define STREAM_COMMON_H

#include "ensivideo.hpp"
#include <chrono>

using namespace std;

extern bool fini;
extern chrono::time_point<chrono::high_resolution_clock> datedebut;

int msFromStart();
void pageReader(ifstream &vf, ogg_sync_state *pstate, ogg_page *ppage);
struct streamstate *getStreamState(ogg_sync_state *pstate, ogg_page *ppage,
                                   enum streamtype type);
int addPageGetPacket(ogg_page *ppage, struct streamstate *s);
int getPacket(struct streamstate *s);
int decodeAllHeaders(int respac, struct streamstate *s, enum streamtype type);

#endif
