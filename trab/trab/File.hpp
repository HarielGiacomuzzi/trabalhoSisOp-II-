//
//  File.hpp
//  trab
//
//  Created by Hariel Giacomuzzi on 11/16/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#ifndef File_hpp
#define File_hpp

#include <stdio.h>
#include <string>
#include <vector>

class File {
    
public:
    // cria um arquivo e o aloca em clusters
    bool createFile(std::string fileName);
    // lê o conteudo do arquivo e printa na tela
    void printFileContent(std::string fileName);
    // lê todos os clusters do arquivo (trás tudo pra memória)
    void readFile(std::string fileName);
    // limpa os clusters do arquivo e salva tudo denovo
    void saveFile(std::string fileName, std::vector<void*> fileParts);
private:
    
};

#endif /* File_hpp */
