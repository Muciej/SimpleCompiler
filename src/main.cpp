#include <iostream>

using namespace std;

extern void run( FILE* data, FILE* out);

int main(int argc, char const* argv[]) {
    FILE* data;
    FILE* out;
    if(argc!=3){
        cerr << "Poprawne użycie: kompilator plik_wejściowy plik_wyjściowy"<<endl;
        return -1;
    }

    data = fopen(argv[1], "r");
    out = fopen(argv[2], "w+");

    if( !data){
        cerr<< "Błąd: nie można otworzyć pliku " << argv[1] << endl;
        return -1;
    }
    if( !out){
        cerr<< "Błąd: nie można otworzyć/utworzyć pliku " << argv[2] << endl;
        return -1;
    }

    run(data, out);

    return 0;
}

