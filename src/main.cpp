#include <iostream>
#include <fstream>

using namespace std;

extern void run( FILE* data, ofstream& out, ofstream& debugst);

int main(int argc, char const* argv[]) {
    FILE* data;
    ofstream out, debug;

    if(argc!=3 && argc!=4){
        cerr << "Poprawne użycie: kompilator plik_wejściowy plik_wyjściowy (opcjonalnie plik debug)"<<endl;
        cerr << "Correct usage: "<<endl;
        return -1;
    }

    data = fopen(argv[1], "r");
    out.open(argv[2]);
    debug.open(argv[3]);

    if( !data){
        cerr<< "Błąd: nie można otworzyć pliku " << argv[1] << endl;
        return -1;
    }
    if( !out.good()){
        cerr<< "Błąd: nie można otworzyć/utworzyć pliku " << argv[2] << endl;
        return -1;
    }
    if(argc == 4 && !debug.good()){
        cerr<<"Błąd: nie można otworzyć pliku debug " << argv[3] << endl;
    }
    run(data, out, debug);

    return 0;
}

