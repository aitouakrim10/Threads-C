#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.hpp"

#include <condition_variable>
#include <mutex>
using namespace std;

/* Les extern des variables pour la synchro ici */

extern mutex allstrm;
extern mutex mutextex;
extern condition_variable texprod;
extern condition_variable texconso;

extern bool fini;
extern int tex_nbready;
extern bool windowsdone;

/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
