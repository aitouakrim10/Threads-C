//#include "ensitheora.h"
//#include "synchro.h"
//#include "pthread.h"
///* les variables pour la synchro, ici */
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t fenetrePrete = PTHREAD_COND_INITIALIZER;
//pthread_cond_t tailleDispo = PTHREAD_COND_INITIALIZER;
//pthread_cond_t peutConsommer = PTHREAD_COND_INITIALIZER;
//pthread_cond_t peutDeposer = PTHREAD_COND_INITIALIZER;
//bool tailleDisponible = false;
//bool fenetrePret = false;
//int nbTextDepose = 0;
///* l'implantation des fonctions de synchro ici */
//
//void envoiTailleFenetre(th_ycbcr_buffer buffer) {
//    pthread_mutex_lock(&mutex);
//    windowsx = buffer[0].width;
//    windowsy = buffer[0].height;
//    // Signaler que la taille est prête
//    tailleDisponible = true;
//    pthread_cond_signal(&tailleDispo);
//    pthread_mutex_unlock(&mutex);
//
//}
//
//void attendreTailleFenetre() {
//    pthread_mutex_lock(&mutex);
//    // Attendre que la taille de la fenêtre soit disponible
//    while (!tailleDisponible) {
//        pthread_cond_wait(&tailleDispo, &mutex);
//    }
//    pthread_mutex_unlock(&mutex);
//}
//
//void signalerFenetreEtTexturePrete() {
//    pthread_mutex_lock(&mutex);
//    fenetrePret = true;
//    pthread_cond_signal(&fenetrePrete);
//    pthread_mutex_unlock(&mutex);
//
//}
//
//void attendreFenetreTexture() {
//    pthread_mutex_lock(&mutex);
//    while (!fenetrePret) {
//        pthread_cond_wait(&fenetrePrete, &mutex);
//    }
//    pthread_mutex_unlock(&mutex);
//}
//
//void debutConsommerTexture() {
//    pthread_mutex_lock(&mutex);
//    while (nbTextDepose==0)
//    {
//        pthread_cond_wait(&peutConsommer, &mutex);
//    }
//    nbTextDepose--;
//    //pthread_mutex_unlock(&mutex);
//}
//
//void finConsommerTexture() {
//    //pthread_mutex_lock(&mutex);
//    pthread_cond_signal(&peutDeposer);
//    pthread_mutex_unlock(&mutex);
//}
//
//void debutDeposerTexture() {
//    pthread_mutex_lock(&mutex);
//    while (nbTextDepose==NBTEX)
//    {
//        pthread_cond_wait(&peutDeposer, &mutex);
//    }
//    nbTextDepose++;
//    //pthread_mutex_unlock(&mutex);
//}
//
//void finDeposerTexture() {
//    //pthread_mutex_lock(&mutex);
//    pthread_cond_signal(&peutConsommer);
//    pthread_mutex_unlock(&mutex);
//}


#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>


/* les variables pour la synchro, ici */
pthread_cond_t tailleFenetreEnvoyee; // condition taille fenetre envoyee
pthread_cond_t fentextwait; //condition fenetre et texture prete
pthread_cond_t lectureAutorisee;
pthread_cond_t ecritureAutorisee;

pthread_mutex_t mutex;

int texture_recue = 0;
/* texture_recue pour confirmer qu'on a bien reçu la texture et pour éviter un bloquage eventuel lors des itérations suivantes*/

int p = 0,
nbTexAAfficher =0;
/* p pour confirmer qu'on a bien reçu la taille de la fenetre et pour éviter un bloquage eventuel lors des itérations suivantes*/

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&mutex);
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    p = 1;
    pthread_cond_signal(&tailleFenetreEnvoyee);
    pthread_mutex_unlock(&mutex);

}

void attendreTailleFenetre() {
    pthread_mutex_lock(&mutex);
    while(p==0){

        pthread_cond_wait(&tailleFenetreEnvoyee,&mutex);

    }
    pthread_mutex_unlock(&mutex);

}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&mutex);
    texture_recue = 1;
    pthread_cond_signal(&fentextwait);
    pthread_mutex_unlock(&mutex);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&mutex);
    if (texture_recue == 0){
        pthread_cond_wait(&fentextwait,&mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&mutex);
    while (nbTexAAfficher == 0){
        pthread_cond_wait(&lectureAutorisee,&mutex);    }
}

void finConsommerTexture() {
    nbTexAAfficher--;
    pthread_cond_signal(&ecritureAutorisee);
    pthread_mutex_unlock(&mutex);

}


void debutDeposerTexture() {
    pthread_mutex_lock(&mutex);
    while (nbTexAAfficher == NBTEX){
        pthread_cond_wait(&ecritureAutorisee,&mutex);
    }
    pthread_cond_signal(&ecritureAutorisee);
    }

void finDeposerTexture() {
    nbTexAAfficher ++;
    pthread_mutex_unlock(&mutex);
}