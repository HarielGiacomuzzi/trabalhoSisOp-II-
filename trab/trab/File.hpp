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

class File {
    
public:
    bool createFile(std::string fileName);
    void printFileContent(std::string fileName);
private:
    
};

#endif /* File_hpp */
