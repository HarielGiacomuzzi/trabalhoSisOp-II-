//
//  Utils.cpp
//  trab
//
//  Created by Hariel Giacomuzzi on 11/11/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#include "Shell.hpp"
#include "structures.h"
#include <sstream>
#include <iostream>


/* funcoes auxiliares sobre strings */
// chamada da funcao: std::vector<std::string> x = split("one:two::three", ':');

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}




bool Shell::init(){
    instance.createFAT();
    instance.loadFAT();
    root =  *(folderMetadata*) instance.readClusterData(9);
    return true;
}

bool Shell::ls(std::string dirName){
    folderMetadata actual = root;

    /* se nao foi inserido nenhuma string, imprime as entradas do root */
    if (dirName.size() == 0){
        root =  *(folderMetadata*) instance.readClusterData(9);
        for (int i = 0; i < 32; i++){
           std::string fileName;
           fileName.assign(root.folderEntry[i].filename, root.folderEntry[i].filename + sizeof(root.folderEntry[i].filename));
           std::cout << fileName << '\n';
        }
    }
    /* caso contrario, verifica o caminho  a partir de um vetor splitado a partir dos chars '/' no caminho */
    else{
        std::vector<std::string>  path = split(dirName, '/');
        for (int i = 0; i < path.size(); i++){
            std::string aux = path[i];
            /* procura item na tabela de arquivos do diretorio  */
            for(int j; j < sizeof(actual.folderEntry); j++){
                bool dirNaoEncontrado = true;
                std::string fileName;
                fileName.assign(actual.folderEntry[j].filename, actual.folderEntry[j].filename + sizeof(actual.folderEntry[j].filename));
                if(aux == fileName){
//                    int i = (int) ((instance.readClusterData(actual.folderEntry[j].first_block)));
                    actual = *(folderMetadata *) instance.readClusterData(i);
                    dirNaoEncontrado = false;
                }
                /* se nao possui a entrada corretamente, retorna erro */
                if (dirNaoEncontrado){
                    std::cout << "Diretorio : " << fileName << " nao encontrado " <<'\n';
                    return false;
                }

            }

        }
        /* imprime os itens caso tenha encontrado corretamente */
        for (int i = 0; i < sizeof(actual.folderEntry); i++){
           std::string fileName;
           fileName.assign(actual.folderEntry[i].filename, actual.folderEntry[i].filename + sizeof(actual.folderEntry[i].filename));
           std::cout << fileName << '\n';
            }
        }
        return true;
    }

bool Shell::create(std::string fileName, std::string dirName){
    for (int i = 0; i < 32; i ++) {
        if (root.folderEntry[i].filename[0] == -1) {
            for (int j = 0; j < 18; j++) {
                root.folderEntry[i].filename[j] = fileName[j];
            }
            uint16_t position = instance.findEmptyPlace();
            root.folderEntry[i].first_block = position;
            root.folderEntry[i].attributes = 0;
            void *data = malloc(CLUSTER_SIZE);
            memset(data, 3, CLUSTER_SIZE);
            instance.setUsedCluster(position, 0xff);
            if (instance.writeClusterData(position, data)) {
                free(data);
                return true;
            }
        }
    }
    return false;
}

bool compare(std::string a, uint8_t *b){
    for (int i = 0 ; i < 18; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool Shell::read(std::string fileName, std::string dirName){
    for (int i = 0; i < 32; i++) {
        if (compare(fileName, root.folderEntry[i].filename)) {
            uint16_t pos = root.folderEntry[i].first_block;
            void* data = instance.readClusterData(pos);
            for (int j = 0; j < CLUSTER_SIZE; j++) {
                std::cout << (uint8_t*)data << std::endl;
            }
            free(data);
            return true;
        }
    }
    return false;
}

bool Shell::mkdir(std::string dirName){
    std::vector<std::string>  path = split(dirName, '/');
    folderMetadata actual = root;
    if (path.size() <= 1){
        for (int i = 0; i < 32; i++){
            if (root.folderEntry[i].filename[0] == -1){
                root.folderEntry[i].attributes = 1;
                for (int i = 17; i < 0; i--) {
                    root.folderEntry[i].filename[i] = dirName[i];
                }
                instance.writeClusterData(9, (void*)&root);
                return true;
            }
        }

    }
    return true;
}

