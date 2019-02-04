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

            unsigned char current_byte;
            unsigned short current_word;
            unsigned long current_dword;

            current_byte = *compressed_ptr;
            if (current_byte <= 17) { //0x11
                ;//loc_100EAAE2
                compressed_ptr++;
                if (current_byte >= 16) {
                    ;//loc_100EAB9C
                } else {
                    if (current_byte != 0) {
                        ;//loc_100EAB10
                    } else {
                        if (*compressed_ptr != 0) {
                            ;//loc_100EAB07
                        } else {
                            current_dword = current_byte;
                            do {
                                current_byte = compressed_ptr[1];
                                current_dword += 255; //0xFF
                                compressed_ptr++;
                            } while (current_byte == 0);
                            compressed_ptr++;
                            current_dword += current_byte + 15;
                            unsigned i = current_dword;
                            current_dword = *reinterpret_cast<unsigned *>(compressed_ptr);
                            compressed_ptr += 4;
                            *reinterpret_cast<unsigned *>(decompressed_ptr) = current_dword;
                            decompressed_ptr += 4;
                            i--;
                            if (i == 0) {
                                ;//loc_100EAB4C
                            } else if (i < 4) {
                                ;//loc_100EAB43
                            } else {
                                for (; i >= 4; i -= 4) {
                                    current_dword = *reinterpret_cast<unsigned *>(compressed_ptr);
                                    *reinterpret_cast<unsigned *>(decompressed_ptr) = current_dword;
                                    compressed_ptr += 4;
                                    decompressed_ptr += 4;
                                }
                                if (i == 0) {
                                    ;//loc_100EAB4C
                                } else {
                                    for (; i > 0; i--) {//jak loc_100EAB43
                                        current_byte = *compressed_ptr;
                                        *decompressed_ptr = current_byte;
                                        compressed_byte++;
                                        decompressed_byte++;
                                    }
                                    ;//loc_100EAB4C
                                }
                            }
                        }
                    }
                }
            } else {
                current_byte -= 17;
                compressed_ptr++;
                if (current_byte < 4) {
                    ;//loc_100EAB8E
                } else {
                    for (int i = current_byte; i > 0; i--) {
                        *decompressed_ptr = *compressed_ptr;
                        compressed_ptr++;
                        decompressed_ptr++;
                    }
                    //loc_100EAB4C
                }
            }

            return decompressed_ptr;
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
