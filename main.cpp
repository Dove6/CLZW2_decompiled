#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

class CLZWCompression {
protected:
	int p1, //+0
        p2, //+4
        p3, //+8
        p4; //+12
	int input_size; //+16
	char *input_ptr; //+20
	int p7, //+24
        p8; //+28
	unsigned long __compress(unsigned char *, unsigned char *, unsigned long);
	void __decompress(char *, char *, long);

public:
	virtual char *compress(int &output_size) //funkcja z CLZW2 ma tę samą sygnaturę
	{
	    return nullptr;
	}
	char *decompress(int wakaranai);

	virtual void print_beautiful()
	{
        cout << "Obiekt klasy CLZWCompression: " << this << '\n';
        cout << " (int)    pole 1:  " << p1 << '\n';
        cout << " (int)    pole 2:  " << p2 << '\n';
        cout << " (int)    pole 3:  " << p3 << '\n';
        cout << " (int)    pole 4:  " << p4 << '\n';
        cout << " (int)    dlugosc: " << input_size << '\n';
        cout << " (char *) dane:    " << (int *)input_ptr << '\n';
        cout << "     pierwszy znak: " << *input_ptr << '\n';
        cout << " (int)    pole 7:  " << p7 << '\n';
        cout << " (int)    pole 8:  " << p8 << "\n\n";
	}

	CLZWCompression(char *data, int data_size) //konstruktor
	: input_size(data_size), input_ptr(data)
	{
	}
};

class CLZWCompression2 : public CLZWCompression {
    using CLZWCompression::decompress;
public:
	virtual char *compress(int &output_size)
	{
	    return nullptr;
	}
	char *decompress()
	{
	    if (input_size > 8 && input_ptr != nullptr) {
            //zdekompilowana funkcja
            //↓sub_100EAAA0
            unsigned decompressed_size = reinterpret_cast<unsigned *>(input_ptr)[0]; //rozmiar po dekompresji
            char *decompressed_ptr = new char[decompressed_size];                    //miejsce na dekompresję
            unsigned compressed_size = reinterpret_cast<unsigned *>(input_ptr)[1];   //rozmiar skompresowany
            char *compressed_ptr = input_ptr + 8;                                    //dane skompresowane

            reinterpret_cast<unsigned *>(input_ptr)[0] = 0; //zerowanie rozmiaru po dekompresji
            char *last_compressed_ptr = input_ptr + input_size/* - 1*/; //ebp
            char *first_compressed_ptr = compressed_ptr;
            char *first_decompressed_ptr = decompressed_ptr; //ebx

            unsigned char current_byte;
            unsigned short current_word;
            unsigned long current_dword;

            unsigned i = 0;

            current_byte = *compressed_ptr;
            compressed_ptr++;
            do {
                if (current_byte <= 17) { //0x11
                    //↓loc_100EAAE2
                    if (current_byte < 16) {
                        current_dword = 0;
                        if (current_byte == 0) {
                            if (*compressed_ptr == 0) {
                                do {//↓loc_100EAAF9
                                    current_byte = compressed_ptr[1];
                                    current_dword += 255; //0xFF
                                    compressed_ptr++;
                                } while (current_byte == 0);
                            }
                            current_byte = *compressed_ptr;
                            compressed_ptr++;//↓loc_100EAB07
                            current_dword += current_byte + 15;
                        }
                        i = current_dword;//↓loc_100EAB10
                        i += 3;

                        for (; i > 0; i--) {//↓loc_100EAB38 a.k.a. loc_100EAB43
                            *decompressed_ptr = *compressed_ptr;
                            compressed_ptr++;
                            decompressed_ptr++;
                        }
                        //w tym momencie i = 0
                    }

                    current_byte = *compressed_ptr;//↓loc_100EAB4C
                    compressed_ptr++;

                    if (current_byte < 16) {
                        current_byte >>= 2;
                        char *prior_decompressed_ptr = decompressed_ptr;
                        prior_decompressed_ptr -= current_byte;
                        current_word = *compressed_ptr;
                        current_word <<= 2;
                        prior_decompressed_ptr -= current_word;
                        prior_decompressed_ptr -= 2049;
                        compressed_ptr++;
                        *decompressed_ptr = *prior_decompressed_ptr;
                        decompressed_ptr++;
                        prior_decompressed_ptr++;

                        *decompressed_ptr = prior_decompressed_ptr[0];//↓loc_100EAB77
                        decompressed_ptr++;
                        *decompressed_ptr = prior_decompressed_ptr[1];
                        decompressed_ptr++;

                        current_byte = compressed_ptr[-2];//↓loc_100EAB82
                        current_byte &= 3;
                    }
                } else {
                    current_byte -= 17;
                    if (current_byte >= 4) {//↓loc_100EAAD7
                        for (i = current_byte; i > 0; i--) {
                            *decompressed_ptr = *compressed_ptr;
                            compressed_ptr++;
                            decompressed_ptr++;
                        }

                        //current_byte = *compressed_ptr;//↓loc_100EAB4C
                        //compressed_ptr++;

                        if (current_byte < 16) {
                            current_byte >>= 2;
                            char *prior_decompressed_ptr = decompressed_ptr;
                            prior_decompressed_ptr -= current_byte;
                            current_word = *compressed_ptr;
                            current_word <<= 2;
                            prior_decompressed_ptr -= current_word;
                            prior_decompressed_ptr -= 2049;
                            compressed_ptr++;
                            *decompressed_ptr = *prior_decompressed_ptr;
                            decompressed_ptr++;
                            prior_decompressed_ptr++;

                            *decompressed_ptr = prior_decompressed_ptr[0];//↓loc_100EAB77
                            decompressed_ptr++;
                            *decompressed_ptr = prior_decompressed_ptr[1];
                            decompressed_ptr++;

                            current_byte = compressed_ptr[-2];//↓loc_100EAB82
                            current_byte &= 3;
                        }
                    }
                }
            //} while (current_byte == 0);

            //↓loc_100EAB8E
            for (; i > 0; i--) {
                *decompressed_ptr = *compressed_ptr;
                decompressed_ptr++;
                compressed_ptr++;
            }
            //current_byte = *compressed_ptr;
            //compressed_ptr++;

            ;//↓loc_100EAB9C
            //while (compressed_ptr <= last_compressed_ptr) {
                if (current_byte >= 64) {
                    i = current_byte;
                    char *prior_decompressed_ptr = decompressed_ptr;
                    current_byte >>= 2;
                    current_byte &= 7;
                    prior_decompressed_ptr -= current_byte;
                    current_word = *compressed_ptr;
                    compressed_ptr++;
                    current_word <<= 3;
                    prior_decompressed_ptr -= current_word;
                    prior_decompressed_ptr--;
                    i >>= 5;

                    //↓loc_100EABBC
                    i++;

                    //↓loc_100EABC9
                    for (; i > 0; i--) {
                        *decompressed_ptr = *prior_decompressed_ptr;
                        prior_decompressed_ptr++;
                        decompressed_ptr++;
                    }
                    current_byte = compressed_ptr[-2];//↓loc_100EAB82
                    current_byte &= 3;
                    //↓loc_100EAB8E
                    for (i = current_byte; i > 0; i--) {
                        *decompressed_ptr = *compressed_ptr;
                        decompressed_ptr++;
                        compressed_ptr++;
                    }
                    current_byte = *compressed_ptr;
                    compressed_ptr++;
                } else if (current_byte >= 32) {//↓loc_100EABD4 (gałąź przed-przed końcem
                    current_byte &= 31;
                    if (current_byte == 0) {
                        if (*compressed_ptr == 0) {
                            current_dword = 0;//↓loc_100EABE3
                            do {
                                compressed_ptr++;
                                current_byte = *compressed_ptr;
                                current_dword += 255; //0xFF
                            } while (current_byte == 0);
                        }
                        compressed_ptr++;//↓loc_100EABF1
                        current_dword += current_byte + 31; //0x1F
                    }
                    char *prior_decompressed_ptr = decompressed_ptr;//↓loc_100EABFA
                    current_word = *reinterpret_cast<unsigned short *>(compressed_ptr);
                    compressed_ptr += 2;
                    current_word >>= 2;
                    prior_decompressed_ptr -= current_word;
                    prior_decompressed_ptr--;

                    ;//↓loc_100EAC59
                    int i = current_dword;
                    if (current_dword >= 6) {
                        int ptr_difference = decompressed_ptr - prior_decompressed_ptr;
                        if (ptr_difference >= 4) {
                            current_dword += 2;
                            for (; i >= 4; i--) {//↓loc_100EAC7C
                                *decompressed_ptr = *prior_decompressed_ptr;
                                decompressed_ptr++;
                                prior_decompressed_ptr++;
                            }
                            current_byte = compressed_ptr[-2];//↓loc_100EAB82
                            current_byte &= 3;
                            //↓loc_100EAB8E
                            for (unsigned i = current_byte; i > 0; i--) {
                                *decompressed_ptr = *compressed_ptr;
                                decompressed_ptr++;
                                compressed_ptr++;
                            }
                            current_byte = *compressed_ptr;
                            compressed_ptr++;
                            ;//loc_100EAB9C
                        }
                    }
                    //↓loc_100EABBC
                    i++;

                    //↓loc_100EABC9
                    for (; i > 0; i--) {
                        *decompressed_ptr = *prior_decompressed_ptr;
                        prior_decompressed_ptr++;
                        decompressed_ptr++;
                    }
                    current_byte = compressed_ptr[-2];//↓loc_100EAB82
                    current_byte &= 3;
                    //↓loc_100EAB8E
                    for (unsigned i = current_byte; i > 0; i--) {
                        *decompressed_ptr = *compressed_ptr;
                        decompressed_ptr++;
                        compressed_ptr++;
                    }
                    current_byte = *compressed_ptr;
                    compressed_ptr++;
                } else if (current_byte >= 16) {//↓loc_100EAC0C (gałąź przed końcem)
                    char *prior_decompressed_ptr = decompressed_ptr;
                    current_dword = current_byte;
                    current_dword &= 8;
                    current_dword <<= 1;
                    prior_decompressed_ptr -= current_dword;
                    current_byte &= 7;
                    if (current_byte == 0) {
                        if (*compressed_ptr == 0) {
                            current_dword = 0;//↓loc_100EAC2B
                            do {
                                compressed_ptr++;
                                current_byte = *compressed_ptr;
                                current_dword += 255;
                            } while (current_byte == 0);
                        }
                        compressed_ptr++;//↓loc_100EAC39
                        current_dword += current_byte + 7;
                    }
                    current_word = *reinterpret_cast<unsigned short *>(compressed_ptr);//↓loc_100EAC42
                    compressed_ptr += 2;
                    current_word >>= 2;
                    prior_decompressed_ptr -= current_word;
                    if (prior_decompressed_ptr == decompressed_ptr) {
                        break;
                    } else {
                        prior_decompressed_ptr -= 16384; //0x4000
                        ;//↓loc_100EAC59
                        int i = current_dword;
                        if (current_dword >= 6) {
                            int ptr_difference = decompressed_ptr - prior_decompressed_ptr;
                            if (ptr_difference >= 4) {
                                current_dword += 2;
                                for (; i >= 4; i--) {//↓loc_100EAC7C
                                    *decompressed_ptr = *prior_decompressed_ptr;
                                    decompressed_ptr++;
                                    prior_decompressed_ptr++;
                                }
                            }
                        } else {
                            //↓loc_100EABBC
                            i++;

                            //↓loc_100EABC9
                            for (; i > 0; i--) {
                                *decompressed_ptr = *prior_decompressed_ptr;
                                prior_decompressed_ptr++;
                                decompressed_ptr++;
                            }
                        }

                        current_byte = compressed_ptr[-2];//↓loc_100EAB82
                        current_byte &= 3;
                        //↓loc_100EAB8E
                        for (unsigned i = current_byte; i > 0; i--) {
                            *decompressed_ptr = *compressed_ptr;
                            decompressed_ptr++;
                            compressed_ptr++;
                        }
                        current_byte = *compressed_ptr;
                        compressed_ptr++;
                    }
                } else {
                    current_byte -= 2;//↓loc_100EACA4
                    char *prior_decompressed_ptr = decompressed_ptr;
                    prior_decompressed_ptr -= current_byte;
                    current_word = *prior_decompressed_ptr;
                    current_word <<= 2;
                    prior_decompressed_ptr -= current_word;
                    prior_decompressed_ptr--; //charakterystyczne
                    compressed_ptr++;
                    *decompressed_ptr = prior_decompressed_ptr[0];//↓loc_100EAB77
                    decompressed_ptr++;
                    *decompressed_ptr = prior_decompressed_ptr[1];
                    decompressed_ptr++;

                    current_byte = compressed_ptr[-2];//↓loc_100EAB82
                    current_byte &= 3;
                    for (unsigned i = current_byte; i > 0; i--) {
                        *decompressed_ptr = *compressed_ptr;
                        decompressed_ptr++;
                        compressed_ptr++;
                    }
                    current_byte = *compressed_ptr;
                    compressed_ptr++;
                }
            } while (current_byte == 0);

            ;//↓loc_100EACBB (gałąź końcowa)
            unsigned ptr_difference = decompressed_ptr - first_decompressed_ptr;
            input_ptr[0] = ptr_difference;
            if (compressed_ptr != last_compressed_ptr) { //błędy - return zwraca co innego
                ;//↓loc_100EACCE
                if (compressed_ptr < last_compressed_ptr) { //wyjście zawiera mniej niż zaplanowano
                    return (char *)-8;
                } else { //wyjście zawiera więcej niż zaplanowano
                    return (char *)-4;
                }
                //oficjalny koniec pętli
            } else {
                return first_decompressed_ptr;
            }

	    } else {
            return nullptr;
	    }
	}

	virtual void print_beautiful()
	{
        cout << "Obiekt klasy CLZWCompression2: " << this << '\n';
        cout << " (int)    pole 1:  " << p1 << '\n';
        cout << " (int)    pole 2:  " << p2 << '\n';
        cout << " (int)    pole 3:  " << p3 << '\n';
        cout << " (int)    pole 4:  " << p4 << '\n';
        cout << " (int)    dlugosc: " << input_size << '\n';
        cout << " (char *) dane:    " << (int *)input_ptr << '\n';
        cout << "     pierwszy znak: " << *input_ptr << '\n';
        cout << " (int)    pole 7:  " << p7 << '\n';
        cout << " (int)    pole 8:  " << p8 << "\n\n";
	}

	CLZWCompression2(char *data, int data_size)
    : CLZWCompression(data, data_size)
	{
	}
};

int main(int argc, char **argv)
{
    int text_length;
    char *text;
    if (argc > 1) {
        if (argc > 2) {
            cout << "Wersja demo przyjmuje tylko jeden parametr xD";
        }
        ifstream compressed_file(argv[1], ios::in | ios::binary);
        compressed_file.ignore(numeric_limits<streamsize>::max());
        text_length = compressed_file.gcount();
        compressed_file.clear();
        compressed_file.seekg(0, ios_base::beg);
        text = new char[text_length];
        compressed_file.read(text, text_length);
        cout << "Dane (" << (int *)text << ", dl. " << text_length << ") z pliku " << argv[1] << '\n';
    } else {
        text_length = 28;
        text = new char[text_length];
        cout << "Dane (" << (int *)text << ", dl. " << text_length << ")\n";
        {//text initialization
            text[0] = '\x10';
            text[1] = '\0';
            text[2] = '\0';
            text[3] = '\0';
            text[4] = '\x14';
            text[5] = '\0';
            text[6] = '\0';
            text[7] = '\0';
            text[8] = '!';
            text[9] = 'a';
            for (int i = 10; i < 25; i++) {
                text[i] = text[i - 1] + 1;
            }
            text[25] = '\x11';
            text[26] = '\0';
            text[27] = '\0';
        }
    }
    cout << string(text, text_length) << "\n\n";
    CLZWCompression2 a(text, text_length);
    a.print_beautiful();
    char *decompressed = a.decompress();
    if (decompressed != nullptr) {
        int dec_size = reinterpret_cast<int *>(text)[0];
        cout << "Wyjscie: (dl. " << dec_size << ")\n";
        cout << string(decompressed, dec_size) << '\n';
    }
    return 0;
}
