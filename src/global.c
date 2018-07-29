#ifndef GLOBAL2_H
#define GLOBAL2_H

#include "stdio.h"
#include<pthread.h>
#include "time.h"

char arr[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

int fileExists (char * filename) {
    FILE *file  = fopen( filename, "r");

    if ( file ) {
        fclose(file);
        return 1;
    }
    return 0;
}

pthread_mutex_t lock;
pthread_mutex_t lockOutput;


char g_working = 0;

long unsigned g_startPrev = 0, g_start = 0, g_length = 0, g_end = 0, g_batch;
unsigned char g_pwd[1024*1026], g_difficulty[34];
unsigned char g_bestHash[34];
long unsigned g_bestHashNonce = 0;
long unsigned g_hashesTotal = 0;

char * g_filename;
char g_filenameOutput[50] ;

clock_t g_tstart;


int readData(char * filename){

    int i, a;
    long unsigned _start, _length, security;


    if (!fileExists(filename))  return 0;

    FILE * fin = fopen(filename, "r");

    fscanf (fin, "%lu", &_start);
    fscanf (fin, "%lu", &_length);


    if (_start == g_startPrev && _length == g_length){
        fclose(fin);
        return 0;
    }

    pthread_mutex_lock(&lock);


    //printf("hash:   \n");
    for ( i=0; i<_length ; i++){
        fscanf (fin,"%d", &a);
        g_pwd[i] = a;

        if (feof(fin)){
            fclose(fin);
            pthread_mutex_unlock(&lock);
            return 0;
        }
        //std::cout << a << " ";
    }

    //printf("DIFFICULTY:   \n");
    for ( i=0; i<32; i++){

        fscanf (fin, "%d", &a);
        g_difficulty[i] = a;

        if (feof(fin)){
            fclose(fin);
            pthread_mutex_unlock(&lock);
            return 0;
        }
        //std::cout << a << " ";
    }


    //fin >> pwdHex;
    //fin >> difficultyHex;
    fscanf(fin,"%lu", &g_end);
    fscanf(fin,"%lu", &g_batch);

    fscanf(fin,"%lu", &security);

    //std::cout  << " cool " << end << " " << batch << " " << security << "\n";

    if (security != 218391){
        fclose(fin);
        pthread_mutex_unlock(&lock);
        return 0;
    }

    g_start = _start;
    g_startPrev = g_start;
    g_length = _length;

    for (i=0; i<32; i++)
        g_bestHash[i] = 255;

    g_bestHashNonce = 0;
    g_hashesTotal = 0;
    g_tstart = clock();

    fclose(fin);
    pthread_mutex_unlock(&lock);

    printf("DATA READ!!! %lu %lu %lu \n", g_length, g_start, g_end);

/*
    for (i=0;i < length;i++) {
         char a = pwdHex[2 * i],  b = pwdHex[2 * i + 1];
        pwd[i] = (((encode(a) * 16) & 0xF0) + (encode(b) & 0x0F));
    }

    for (i=0;i < 32; i++) {
         char a = difficultyHex[2 * i],  b = difficultyHex[2 * i + 1];
     difficulty[i] = (((encode(a) * 16) & 0xF0) + (encode(b) & 0x0F));

    std::cout << length << " " << start << " "<< end << " " << batch << '\n';
    std::cout << pwd << '\n';
    std::cout << difficulty << '\n';

*/

    /*
        for (auto q=0; q < length; q++){
             d2base( pwd[q], 16);
             std::cout << " ";
        }

        std::cout << "\n\n";
       for (auto q=0; q < 32; q++){
             //d2base( difficulty[q], 16);
             //std::cout << difficulty[q];
             std::cout << " ";
        }
        std::cout  << "\n\n";
    */

    return 1;

}



#endif