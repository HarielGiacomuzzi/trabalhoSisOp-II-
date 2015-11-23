//
//  FatManager.cpp
//  trab
//
//  Created by Hariel Giacomuzzi on 11/11/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#include "FatManager.hpp"

bool FAT::createFAT(){
    // clearing the fat array in order to make sure no trash is in there
    for (int i = 0; i < NUMBER_OF_CLUSTERS; i++) {
        fat[i] = 0x00;
    }
    fat[0] = 0xfffd;
    fat[1] = 0xfffe;
    fat[2] = 0xfffe;
    fat[3] = 0xfffe;
    fat[4] = 0xfffe;
    fat[5] = 0xfffe;
    fat[6] = 0xfffe;
    fat[7] = 0xfffe;
    fat[8] = 0xfffe;
    fat[9] = 0xffff;
    
    // cria o bloco de boot com 0xbb conforme a especificação
    uint8_t boot[CLUSTER_SIZE];
    for (int i = 0; i < CLUSTER_SIZE; i++) {
        boot[i] = 0xbb;
    }
    
    //creating the root dir
    folderMetadata root;
    for (int i = 0; i < 32; i++) {
        dir_entry_t aux;
        aux.filename[0] = -1;
        root.folderEntry[i] = aux;
    }
    root.folderEntry[0].filename[0] = '/';
    root.folderEntry[0].attributes = 1;
    
    
    // creating the file, opening it and writing the first set of data
    FILE *fatFile;
    fatFile = fopen(DEFAULT_PATCH, "w+");
    // writing the boot block
    fseek(fatFile, 0, SEEK_SET);
    unsigned long a = fwrite(&boot, sizeof(boot), 1, fatFile);
    // writing the fat table
    fseek(fatFile, CLUSTER_SIZE, SEEK_SET);
    unsigned long b = fwrite(&fat, sizeof(fat), 1, fatFile);
    // writing the root dir
    fseek(fatFile, 9*CLUSTER_SIZE, SEEK_SET);
    unsigned long c = fwrite(&root, sizeof(fat), 1, fatFile);
    fclose(fatFile);
    
    if(a > 0 && b > 0 && c > 0){
        return true;
    }
    return false;
}

bool FAT::loadFAT(){
    FILE *fatFile;
    // opens the file
    fatFile = fopen(DEFAULT_PATCH, "r+");
    // seek for the fat table part
    fseek(fatFile, CLUSTER_SIZE, SEEK_SET);
    // reads the fat table to the array in memory
    unsigned long a = fread(&fat, sizeof(uint16_t), NUMBER_OF_CLUSTERS, fatFile);
    // saves the reference of the file descriptor
    isFatLoaded = true;
    fileOpen = fatFile;
    fclose(fatFile);
    if(a > 0){
        return true;
    }
    
    return false;
}

int FAT::findEmptyPlace(){
    if (isFatLoaded) {
        for (int i = 0; i < NUMBER_OF_CLUSTERS ; i++) {
            if (fat[i] == 0) {
                std::cout<< fat[i] << std::endl;
                return i;
            }
        }
        //return -2 in case there's no empty place in memory
        return -2;
    }
    // returns -1 if the fat is not loaded, please use loadFAT() :) and no I'll not do it for you :( 
    return -1;
}

std::vector<int> FAT::seekClusterParts(int init){
    std::vector<int> aux;
    if(isFatLoaded){
        int actualPos = init;
        do {
            aux.push_back(actualPos);
            actualPos = fat[actualPos];
        } while (actualPos != -1);
    }
    return aux;
}

bool FAT::setUsedCluster(int clusterPosition, uint16_t next){
    if (isFatLoaded) {
        if (fat[clusterPosition] == 0) {
            fat[clusterPosition] = next;
            return true;
        }
    }
    return false;
}

bool FAT::clearCluster(int position){
    if (isFatLoaded) {
        fat[position] = 0;
        return true;
    }
    return false;
}

bool FAT::writeClusterData(int index, void* clusterData){
    if (isFatLoaded && fileOpen != 0 && index > 9) {
        FILE *fatFile;
        // opens the file
        fatFile = fopen(DEFAULT_PATCH, "r+");
        fseek(fatFile, index*CLUSTER_SIZE, SEEK_SET);
        unsigned long a = fwrite(&clusterData, CLUSTER_SIZE, 1, fatFile);        
        fclose(fatFile);
        return a > 0;
    }
    return false;
}

void* FAT::readClusterData(int index){
    void *data = malloc(CLUSTER_SIZE);
    if (isFatLoaded && fileOpen != 0 && index > 8) {
        FILE *fatFile;
        // opens the file
        fatFile = fopen(DEFAULT_PATCH, "r+");
        fseek(fatFile, index*CLUSTER_SIZE, SEEK_SET);
        unsigned long a = fread(data, sizeof(data), 1, fatFile);
        fclose(fatFile);
        if (a <= 0) {
            return NULL;
        }
    }
    return data;
}

bool FAT::unloadFAT(){
    if (isFatLoaded && fileOpen != 0){
        FILE *fatFile;
        // opens the file
        fatFile = fopen(DEFAULT_PATCH, "r+");
        fseek(fatFile, CLUSTER_SIZE, SEEK_SET);
        unsigned long b = fwrite(&fat, sizeof(fat), 1, fatFile);
        if (b > 0) {
            fclose(fatFile);
            return true;
        }
    }
    return false;
}
