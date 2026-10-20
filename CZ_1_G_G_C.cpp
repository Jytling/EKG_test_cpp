#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cmath>
#include <algorithm>
using namespace std;
//void getlineinfo(int range, double min_value, double max_value)

double gettable(double tab[],int range, double min_value, double max_value) {

string temp;

    ifstream reader("samples.csv");

    getline(reader, temp);
    getline(reader, temp);

const int chart_width = 80;
tab[range+1];
float sample;

for(int i=0;reader.good() && i<range; i++){
    getline(reader,temp,',');
    getline(reader,temp);
    sample=stof(temp);
    // nie ma sensu wymnażać teraz na 200. Przy funkcji square wychodzą mega wielkie liczby.
    //Najlepiej to przemnożyć w każdej funkcji na końcu lub w funkcji która będzie rysować wykres.
    //Teraz jest przy każdej funkcji, na zajęciach lub może póżniej zrobię końcową funkcję.

    tab[i]=sample;
    }
    return *tab;
}

double lowp(double tab[], int range){
    //y(n)=x(n)-x(n-7)
    int N=7;
    double tab_lowp[range+1];
        for(int j=N;j<range;j++){
            tab_lowp[j]=(tab[j]-tab[j-N]);
            tab_lowp[j] *= 200;
            tab_lowp[j]=round(tab_lowp[j]*1000)/1000;
            //cout << tab_lowp[j] << " = " << 200*tab[j] << " - " << 200*tab[j-N] << endl; //sprawdzenie czy działa
            //cout << tab_lowp[j] << endl; // Również sprawdzenie czy działa
        }
    return *tab_lowp;
}

double meansignal(double tab[], int range){

//y[n]=1/(N-1) - suma od k=0 do N-1 z y[n-k]
    int N=8;
    double dodawanie=0;
    doube mean_tab[range];

    for (int i = 0; i < range / 8; i++) {
        dodawanie = 0;
        for (int k = N - 8; k < N; k++) {
            dodawanie += tab[k];
        }
        double mean = dodawanie / 8.0;
        tab[i] = mean * 200;
       cout << i << "       " << tab[i] << "      " << mean << endl; // Tylko żeby sprawdzić czy działa to czy nie XDDD
        N += 8;
    }
    return *mean_tab;
}

double square(double tab[], int range) {
    double tab_squared[range];
    for(int i=0;i<range;i++) {
        tab_squared[i] = 200*(tab[i]*tab[i]);
        cout << tab_squared[i] << "      " << tab[i] << endl;
    }
    return *tab_squared;
}

void chart_four(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, int range){

    const int width = 80;
    int height = range;

    char grid[range][width];

   char** grid = new char*[height]; //tablica dynamiczna
    for (int i = 0; i < height; i++) {
        grid[r] = new char[width];
        for (int c = 0; c < width; c++) {
            grid[r][c] = ' '; // Wypełnienie utworzoenj tablicy spacjami
        }
    }

    int* tabs[4] = {tab, tab_lowp, mean_tab, tab_squared};
    char symbols[4] = {'*', '$', '#', '&'};

    int max_value = -50000; //poniezej szukamy wartosci najmniejszej i najweikszej
    int min_value = 50000;

    for(int j=0; j<4; j++) {
        for(int l=0; l<range; l++) {
            if(tabs[j][l]<min_value){
                min_value = tabs[j][l];
            }
            if(tabs[j][l]>max_value){
                max_value=tabs[j][l];
            }
        }
    }

    int range = max_value - min_value;

    //iteracja po wszystkich tablicach 
    for(int i=0; i<range; i++){
        int r =i; //to wiersz wykresu

        for (int j=0;j<4;j++){
            int val = tablice[j][i];

            int c = 0;
            if(range>0) {
            C = ((val-min_val)*(width-1))/range; //skalowanie
            } else {
                c = width/2;
            }
            if (c>=0 && c < width) {
                grid[r][c] = symbols[j];
            }
        }
    }

    for (int m=0;m<height;m++){
        for(int n=0;n<width;n++){
            cout<<grid[m][n];
        }
        cout<<"\n";
    }

}

int main() {
    int range = 100;
    string temp;
    double tab_main[range+1];

    ifstream reader("samples.csv");

    getline(reader, temp);
    getline(reader, temp);

    double min_value = numeric_limits<double>::infinity();
    double max_value = -numeric_limits<double>::infinity();

    for (int i = 0; i < range && getline(reader, temp, ','); i++) {
        string value_str;
        if (!getline(reader, value_str)) break;

        try {
            double val = 200.0 * stod(value_str);
            if (val < min_value) min_value = val;
            if (val > max_value) max_value = val;
        } catch (const exception&) {
            continue;
        }
    }
    reader.close();

    //getlineinfo(range, min_value, max_value);
    gettable(tab_main,range, min_value, max_value);
    lowp(tab_main, range);
    //meansignal(tab_main,range);
    //square(tab_main, range);
    /*for(int i = 0; i < range; i++){
    cout<<tab_main[i]<<endl;
    }*/
    return 0;
}


void getlineinfo(int range, double min_value, double max_value) {
    ifstream reader("samples.csv");

    string temp;
    getline(reader, temp);
    getline(reader, temp);

    const int chart_width = 80;
    const int center = chart_width / 2;

    double zero_point = (max_value + min_value) / 2.0;
    double half_range = (max_value - min_value) / 2.0;

    for (int i = 0; i < range && getline(reader, temp, ','); i++) {
        if (!getline(reader, temp)) break;

        double sample;
        try {
            sample = 200.0 * stod(temp);
        } catch (const exception&) {
            continue;
        }

        int offset = 0;
        if (half_range != 0) {
            offset = static_cast<int>(round((sample - zero_point) / half_range * center));
        }

        int pos = center + offset;
        pos = max(0, min(chart_width - 1, pos));

        for (int k = 0; k < chart_width; k++) {
            if (k == pos) {
                cout << '*';
            } else if (k == center) {
                cout << '|';
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }
}
