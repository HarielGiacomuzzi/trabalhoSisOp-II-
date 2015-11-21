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
        fat[i] = 0;
    }
    
    // cria o bloco de boot com 0xbb conforme a especificação
    uint8_t boot[CLUSTER_SIZE];
    for (int i = 0; i < CLUSTER_SIZE; i++) {
        boot[i] = 0xbb;
    }
    
    // creating the file, opening it and writing the first set of data
    FILE *fatFile;
    fatFile = fopen("fat.part", "w+");
    // writing the boot block
    fseek(fatFile, 0, CLUSTER_SIZE);
    unsigned long a = fwrite(&boot, sizeof(boot), 1, fatFile);
    // writing the fat table
    fseek(fatFile, 1, CLUSTER_SIZE);
    unsigned long b = fwrite(&fat, sizeof(fat), 1, fatFile);
    fclose(fatFile);
    
    if(a > 0 && b > 0){
        return true;
    }
    return false;
}

bool FAT::loadFAT(){
    FILE *fatFile;
    // opens the file
    fatFile = fopen(DEFAULT_PATCH, "r+");
    // seek for the fat table part
    fseek(fatFile, 1, CLUSTER_SIZE);
    // reads the fat table to the array in memory
    unsigned long a = fread(fat, sizeof(uint16_t), NUMBER_OF_CLUSTERS, fatFile);
    // saves the reference of the file descriptor
    isFatLoaded = true;
    fileOpen = fatFile;
    
    if(a > 0){
        return true;
    }
    
    return false;
}

int FAT::findEmptyPlace(){
    if (isFatLoaded) {
        for (int i = 0; i < NUMBER_OF_CLUSTERS ; i++) {
            if (fat[i] == 0) {
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
        fseek(fileOpen, index, CLUSTER_SIZE);
        unsigned long a = fwrite(clusterData, CLUSTER_SIZE, 1, fileOpen);
        return a > 0;
    }
    return false;
}

void* FAT::readClusterData(int index){
    void *data;
    if (isFatLoaded && fileOpen != 0 && index > 8) {
        fseek(fileOpen, index, CLUSTER_SIZE);
        unsigned long a = fread(data, CLUSTER_SIZE, 1, fileOpen);
        if (a <= 0) {
            return NULL;
        }
    }
    return data;
}

bool FAT::unloadFAT(){
    if (isFatLoaded && fileOpen != 0){
        fseek(fileOpen, 1, CLUSTER_SIZE);
        unsigned long b = fwrite(&fat, sizeof(fat), 1, fileOpen);
        if (b > 0) {
            fclose(fileOpen);
            return true;
        }
    }
    return false;
}
