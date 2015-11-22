#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

/* entrada de diretorio, 32 bytes cada */
typedef struct{
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
} dir_entry_t;

/* tabela de metadados dos diretorios, contento 32 entradas */
typedef struct{
    dir_entry_t folderEntry[32];
}folderMetadata;

#endif // STRUCTURES_H
