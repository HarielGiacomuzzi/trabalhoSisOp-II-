//
//  FatManager.hpp
//  trab
//
//  Created by Hariel Giacomuzzi on 11/11/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#ifndef FatManager_hpp
#define FatManager_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "defines.h"

class FAT {

uint16_t fat[NUMBER_OF_CLUSTERS];
///* diretorios (incluindo ROOT), 32 entradas de diretorio
// com 32 bytes cada = 1024 bytes ou bloco de dados de 1024 bytes*/
union{
    dir_entry_t dir[CLUSTER_SIZE / sizeof(dir_entry_t)];
    uint8_t data[CLUSTER_SIZE];
} data_cluster;


public:
    // cria a tabela fat e o arquivo fat.part
    bool createFAT();
    // carrega a fat para a memória
    bool loadFAT();
    // retorna o indice de um cluster vazio
    int findEmptyPlace();
    // retorna uma lista de todas as indices das partes(clusters) de um arquivo
    std::vector<int> seekClusterParts(int initialPosition);
    // marca um cluster como usado e que tipo de arquivo tem lá (na real qual a próxima parte do arquivo e se é end of file :P)
    bool setUsedCluster(int clusterPosition, uint16_t next);
    // remove a marcação de usado da fat para que possa ser escrito novamente
    bool clearCluster(int clusterPosition);
    // escreve os dados de um cluster no disco NÃO É NA FAT !!! E O CLUSTER_DATA TEM QUE TER O TAMANHO DE UM CLUSTER SENÃO VAI DAR MERDA ! eu avisei...
    bool writeClusterData(int clusterIndex, void* clusterData);
    // lê os dados de um cluster do disco retorna null se não é possivel ler os dados
    void* readClusterData(int clusterIndex);
    // salva as alterações no disco e fecha o descritor do arquivo
    bool unloadFAT();
private:
    // check if the system is loaded
    bool isFatLoaded = false;
    // stores the fat descriptor
    FILE *fileOpen = 0;

};


#endif /* FatManager_hpp */
