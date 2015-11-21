//
//  defines.h
//  trab
//
//  Created by Hariel Giacomuzzi on 11/21/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#ifndef defines_h
#define defines_h

#define CLUSTER_SIZE 1024
#define SECTOR_SIZE 512
#define NUMBER_OF_CLUSTERS 4096
#define DEFAULT_PATCH "/Users/harielgiacomuzzi/Documents/Sistemas Operacionais/trab/trabalhoSisOp-II-/trab/trab/fat.part"

/* entrada de diretorio, 32 bytes cada */
typedef struct{
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
} dir_entry_t;
//
/* 8 clusters da tabela FAT, 4096 entradas de 16 bits = 8192 bytes*/
uint16_t fat[NUMBER_OF_CLUSTERS];
///* diretorios (incluindo ROOT), 32 entradas de diretorio
// com 32 bytes cada = 1024 bytes ou bloco de dados de 1024 bytes*/
union{
    dir_entry_t dir[CLUSTER_SIZE / sizeof(dir_entry_t)];
    uint8_t data[CLUSTER_SIZE];
} data_cluster;

#endif /* defines_h */