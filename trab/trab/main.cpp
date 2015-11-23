//
//  main.cpp
//  trab
//
//  Created by Hariel Giacomuzzi on 11/11/15.
//  Copyright © 2015 Hariel Giacomuzzi. All rights reserved.
//

#include <iostream>
#include "FatManager.hpp"
#include "Shell.hpp"
#include "File.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    Shell s;
    s.init();
    s.ls("");
    s.create("olaMundoB.c", "/");
    s.read("olaMundoB.c", "/");
    s.ls("");
    return 0;
}
