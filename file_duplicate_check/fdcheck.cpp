#include <iostream>
#include <cstring>
#include <fstream>

//Chamsko skopiowane z http://www.cplusplus.com/forum/general/94032/#msg504785
//Autor: seftoner (http://www.cplusplus.com/user/seftoner/)

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc == 3) {
        const std::string lFilePath(argv[1]), rFilePath(argv[2]);
        std::ifstream lFile(lFilePath.c_str(), std::ifstream::in | std::ifstream::binary);
        std::ifstream rFile(rFilePath.c_str(), std::ifstream::in | std::ifstream::binary);

        if(!lFile.is_open() || !rFile.is_open())
        {
            std::cout << "Blad przy otwieraniu plikow!\n";
            return -1;
        }

        char *lBuffer = new char[BUFFER_SIZE]();
        char *rBuffer = new char[BUFFER_SIZE]();

        do {
            lFile.read(lBuffer, BUFFER_SIZE);
            rFile.read(rBuffer, BUFFER_SIZE);

            if (std::memcmp(lBuffer, rBuffer, BUFFER_SIZE) != 0)
            {
                delete[] lBuffer;
                delete[] rBuffer;
                std::cout << "Pliki roznia sie!\n";
                return 0;
            }
        } while (lFile.good() || rFile.good());

        delete[] lBuffer;
        delete[] rBuffer;
        std::cout << "Pliki sa identyczne!\n";
    } else {
        std::cout << "Uzycie:\n\tfdcheck plik1 plik2\n";
    }
    return 0;
}
