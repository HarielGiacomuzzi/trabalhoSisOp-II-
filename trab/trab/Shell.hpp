//
//  Utils.hpp
//  trab
//
//  Created by Hariel Giacomuzzi on 11/11/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <sstream>
#include <vector>
#include <string>
#include "structures.h"
#include "FatManager.hpp"



class Shell {


public:

    folderMetadata root;
    FAT instance;

    // inicializa o sistema, cria os aquivos e carrega a fat para a memória
    bool init();
    // carrega o bash
    bool load();

    //*************************************
    //* comandos do bash                  *
    //*************************************
    bool ls(std::string dirName);
    bool mkdir(std::string dirName);
    bool create(std::string fileName, std::string dirName);
    bool unlink(std::string fileName, std::string dirName);
    bool write(std::string fileName, std::string dirName, std::string data);
    bool append(std::string fileName, std::string dirName, std::string data);
    bool read(std::string fileName, std::string dirName);



};

#endif /* Utils_hpp */
