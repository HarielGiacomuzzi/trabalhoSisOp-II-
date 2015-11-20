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

#define CLUSTER_SIZE 1024
#define SECTOR_SIZE 512
#define NUMBER_OF_CLUSTERS 4096

/* entrada de diretorio, 32 bytes cada */
typedef struct{
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
} dir_entry_t;

/* 8 clusters da tabela FAT, 4096 entradas de 16 bits = 8192 bytes*/
uint16_t fat[NUMBER_OF_CLUSTERS];

/* diretorios (incluindo ROOT), 32 entradas de diretorio
 com 32 bytes cada = 1024 bytes ou bloco de dados de 1024 bytes*/
union{
    dir_entry_t dir[CLUSTER_SIZE / sizeof(dir_entry_t)];
    uint8_t data[CLUSTER_SIZE];
} data_cluster;

class FAT {
    
public:
    // cria a tabela fat e o arquivo fat.part
    bool createFAT();
    // carrega a fat para a memória
    bool loadFAT();
    // retorna o indice de um cluster vazio
    int findEmptyPlace();
    // retorna uma lista de todas as partes(clusters) de um arquivo
    std::vector<int> seekClusterParts(int initialPosition);
    // marca um cluster como usado e que tipo de arquivo tem lá
    bool setUsedCluster(int clusterPosition, int tipo);
    // remove a marcação de usado da fat para que possa ser escrito novamente
    bool clearCluster(int clusterPosition);
    
private:
    
};


#endif /* FatManager_hpp */
