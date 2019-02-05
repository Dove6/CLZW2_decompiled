#include <iostream>

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
            unsigned decompressed_size = reinterpret_cast<unsigned *>(input_ptr)[0]; //rozmiar po dekompresji
            char *decompressed_ptr = new char[decompressed_size];                    //miejsce na dekompresję
            unsigned compressed_size = reinterpret_cast<unsigned *>(input_ptr)[1];   //rozmiar skompresowany
            char *compressed_ptr = input_ptr + 8;                                    //dane skompresowane

            reinterpret_cast<unsigned *>(input_ptr)[0] = 0; //zerowanie rozmiaru po dekompresji
            char *last_compressed_ptr = input_ptr + input_size - 1;
            char *first_decompressed_ptr = decompressed_ptr;

            unsigned char current_byte;
            unsigned short current_word;
            unsigned long current_dword;

            //current_byte = *compressed_ptr; //powtórzone później
            if (current_byte <= 17) { //0x11

                current_byte = *compressed_ptr;//↓loc_100EAAE2
                compressed_ptr++;

                if (current_byte < 16) {
                    if (current_byte == 0) {
                        if (*compressed_ptr == 0) {
                            current_dword = current_byte;

                            do {//↓loc_100EAAF9
                                current_byte = compressed_ptr[1];
                                current_dword += 255; //0xFF
                                compressed_ptr++;
                            } while (current_byte == 0);
                        }
                        compressed_ptr++;//↓loc_100EAB07
                        current_dword += current_byte + 15;
                    }
                    unsigned i = current_dword;//↓loc_100EAB10
                    i += 3;

                    for (; i > 0; i--) {//↓loc_100EAB38, jak loc_100EAB43
                        *decompressed_ptr = *compressed_ptr;
                        compressed_ptr+;
                        decompressed_ptr++;
                    }
                    //w tym momencie i = 0
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

                        //↓loc_100EAB8E
                        if (current_byte > 0) {
                            for (unsigned i = current_byte; i > 0; i--) {
                                *decompressed_ptr = *compressed_ptr;
                                decompressed_ptr++;
                                compressed_ptr++;
                            }
                            current_byte = *compressed_ptr;
                            compressed_ptr++;

                            //↓loc_100EAB9C
                            if (current_byte >= 64) {
                                unsigned i = current_byte;
                                prior_decompressed_ptr = decompressed_ptr;
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
                                ;//loc_100EAB82
                            } else {
                                ;//↓loc_100EABD4 (gałąź przed-przed końcem
                                if (current_byte >= 32) {
                                    current_byte &= 31;
                                    if (current_byte == 0) {
                                        if (*compressed_ptr == 0) {
                                            current_dword = current_byte;//↓loc_100EABE3
                                            do {
                                                compressed_ptr++;
                                                current_byte = *compressed_ptr;
                                            } while (current_byte == 0);
                                        }
                                        compressed_ptr++;//↓loc_100EABF1
                                        current_dword += current_byte + 31; //0x1F
                                    }
                                    prior_decompressed_ptr = decompressed_ptr;//↓loc_100EABFA
                                    current_short = *reinterpret_cast<unsigned short *>(compressed_ptr);
                                    compressed_ptr += 2;
                                    current_short >>= 2;
                                    prior_decompressed_ptr -= current_short;
                                    prior_decompressed_ptr--;
                                } else {
                                    ;//↓loc_100EAC0C (gałąź przed końcem)
                                    if (current_byte >= 16) {
                                        prior_decompressed_ptr = decompressed_ptr;
                                        current_dword = current_byte;
                                        current_dword &= 8;
                                        current_dword <<= 1;
                                        prior_decompressed_ptr -= current_dword;
                                        current_byte &= 7;
                                        if (current_byte == 0) {
                                            if (*compressed_ptr == 0) {
                                                current_dword = current_byte;//↓loc_100EAC2B
                                                do {
                                                    compressed_ptr++;
                                                    current_byte = *compressed_ptr;
                                                } while (current_byte == 0);
                                            }
                                            compressed_ptr++;//↓loc_100EAC39
                                            current_dword += current_byte + 7;
                                        }
                                        current_word = *reinterpret_cast<unsigned short *>(compressed_ptr);//↓loc_100EAC42
                                        compressed_ptr += 2;
                                        current_word >>= 2;
                                        prior_decompressed_ptr -= current_word;
                                        if (prior_decompressed_ptr != decompressed_ptr) {
                                            prior_decompressed_ptr -= 16384; //0x4000
                                            ;//↓loc_100EAC59
                                            if (current_dword >= 6) {
                                                int ptr_difference = decompressed_ptr - prior_decompressed_ptr;
                                                if (ptr_difference >= 4) {
                                                    current_dword += 2;
                                                    for (int i = current_dword; i >= 4; i--) {//↓loc_100EAC7C
                                                        *decompressed_ptr = *prior_decompressed_ptr;
                                                        decompressed_ptr++;
                                                        prior_decompressed_ptr++;
                                                    }
                                                    ;//loc_100EAB82
                                                } else {
                                                    ;//loc_100EABBC
                                                }
                                            } else {
                                                ;//loc_100EABBC
                                            }
                                        } else {
                                            ;//↓loc_100EACBB (gałąź końcowa)
                                            unsigned ptr_difference = decompressed_ptr - first_decompressed_ptr;
                                            input_ptr[0] = ptr_difference;
                                            if (compressed_ptr != last_compressed_ptr) { //błędy - return zwraca co innego
                                                ;//↓loc_100EACCE
                                                if (compressed_ptr < last_compressed_ptr) { //wyjście zawiera mniej niż zaplanowano
                                                    return -8;
                                                } else { //wyjście zawiera więcej niż zaplanowano
                                                    return -4;
                                                }
                                            }
                                            //oficjalny koniec pętli
                                        }
                                    } else {
                                        current_byte -= 2;//↓loc_100EACA4
                                        prior_decompressed_ptr = decompressed_ptr;
                                        prior_decompressed_ptr -= current_byte;
                                        current_word = *prior_decompressed_ptr;
                                        current_word <<= 2;
                                        prior_decompressed_ptr -= current_word;
                                        prior_decompressed_ptr--; //charakterystyczne
                                        compressed_ptr++;
                                        ;//loc_100EAB77
                                    }
                                }

                                ;//loc_100EAC59
                            }
                        } else {
                            ;//loc_100EAAE2
                        }
                    } else {
                        ;//loc_100EAB9C
                    }
                } else {//current_byte == 16 || current_byte == 17
                    ;//loc_100EAB9C
                }
            } else {
                current_byte = *compressed_ptr;
                current_byte -= 17;
                compressed_ptr++;
                if (current_byte >= 4) {
                    for (int i = current_byte; i > 0; i--) {
                        *decompressed_ptr = *compressed_ptr;
                        compressed_ptr++;
                        decompressed_ptr++;
                    }
                    ;//loc_100EAB4C
                } else {
                    ;//loc_100EAB8E
                }
            }

            return first_decompressed_ptr;
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

int main()
{
    int text_length = 28;
    char text[text_length];
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
    cout << string(text, text_length) << "\n\n";
    CLZWCompression2 a(text, text_length);
    a.print_beautiful();
    char *decompressed = a.decompress();
    if (decompressed != nullptr) {
        int dec_size = reinterpret_cast<int *>(text)[0];
        cout << "Wyjscie: (dl. " << dec_size << ")\n";
        cout << string(decompressed + 8, dec_size) << '\n';
    }
    return 0;
}
