#include "ensivorbis.hpp"
#include "ensivideo.hpp"
#include "stream_common.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cassert>
#include <chrono>

SDL_AudioDeviceID audioid = 0;
SDL_AudioSpec want = {};
SDL_AudioSpec have = {};

map<int, struct streamstate *> mapvorbisstrstate;

void vorbis2SDL(struct streamstate *s) {
  static long long int nbsamplesbytes = 0;
  assert(s->strtype == TYPE_VORBIS);
  if (!audioid) {
    want.freq = s->vo_dec.info.rate;
    want.format = AUDIO_F32;
    want.channels = s->vo_dec.info.channels;
    want.samples = 4096;
    want.callback = NULL;

    audioid = SDL_OpenAudioDevice(NULL, false, &want, &have, 0);
    SDL_PauseAudioDevice(audioid, 0);
    // start point
    datedebut = chrono::high_resolution_clock::now();
  }
  assert(audioid);

  if (vorbis_synthesis(&s->vo_dec.block, &s->packet) == 0) {
    int res = vorbis_synthesis_blockin(&s->vo_dec.dsp, &s->vo_dec.block);
    assert(res == 0);
  }
  float **pcm = 0;
  int samples = 0;
  while ((samples = vorbis_synthesis_pcmout(&s->vo_dec.dsp, &pcm)) > 0) {
    float *tmpbuff = new float[samples * s->vo_dec.info.channels];
    for (int sa = 0, idx = 0; sa < samples; sa++)
      for (int c = 0; c < s->vo_dec.info.channels; c++, idx++)
        tmpbuff[idx] = pcm[c][sa];

    SDL_QueueAudio(audioid, tmpbuff,
                   samples * s->vo_dec.info.channels * sizeof(float));
    nbsamplesbytes += samples * s->vo_dec.info.channels * sizeof(float);
    delete[] tmpbuff;
    int res = vorbis_synthesis_read(&s->vo_dec.dsp, samples);
    assert(res == 0);
  }
  /* évaluer le temps pour jouer les données en attente */
  /* taille avec SDL_GetQueuedAudioSize */
  /* résultat dans le have */
  /* attendre 1000 ms (1s) de moins */

  double fullaudiotimems = 1000 * nbsamplesbytes / sizeof(float) /
                           static_cast<float>(s->vo_dec.info.rate) /
                           static_cast<float>(s->vo_dec.info.channels);

  double timemsfromstart = msFromStart();

  double ecartms = fullaudiotimems - timemsfromstart;
  if (ecartms > 1000.0) {
    SDL_Delay((int)(ecartms - 1000.0));
  }
}
