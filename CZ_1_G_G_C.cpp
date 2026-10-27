#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cmath>
using namespace std;

//deklaracje funkcji
double* gettable(int range);
double* lowp(double tab[], int range);
double* meansignal(double tab[], int range);
double* square(double tab[], int range);
double* zakres(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, int range);
void chart_four(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, const double *tab_range, int range);

int main() {
    int range = 2000; //ustawianie zakresu (ilosci probek wczytywanych)

    double* tab_main    = gettable(range); //wczytanie danych z csv
    double* tab_lowp    = lowp(tab_main, range); //filtracja dolnoprzepustowa
    double* mean_tab    = meansignal(tab_lowp, range); //wyznaczenie sygnalu sredniego
    double* tab_squared = square(mean_tab, range); //kwadrat sygnalu
    double* tab_range = zakres(tab_main, tab_lowp, mean_tab, tab_squared, range); //skalowanie
    chart_four(tab_main, tab_lowp, mean_tab, tab_squared, tab_range, range); //wykres 

    //zwolnienie zaalokowanej pamięci
    delete[] tab_main;
    delete[] tab_lowp;
    delete[] mean_tab;
    delete[] tab_squared;
    delete[] tab_range;

    return 0;
}





double* gettable(int range) { //funkcja wczytujaca dane z csv do tablicy dynamicznej
    double* tab = new double[range];

string temp;

    ifstream reader("samples.csv");

    //pominiecie 2 pierwszych linii naglowkowych
    getline(reader, temp);
    getline(reader, temp);

tab[range+1];
float sample;

    //wczytywanie danych
for(int i=0;reader.good() && i<range; i++){
    getline(reader,temp,',');
    getline(reader,temp);
    sample=200*stof(temp); //wstepne skalowanie probek
    tab[i]=sample;
    }
    return tab;
}

double* lowp(double tab[], int range){ //filtr dolnoprzepustowy  y(n)=x(n)-x(n-7)
    double* tab_lowp = new double[range];
    
    int N=7;
        for(int j=N;j<range;j++){
            tab_lowp[j]=(tab[j]-tab[j-N]);
            tab_lowp[j]=round(tab_lowp[j]*1000)/1000;//zaokraglenie do 3 miejsc po przecinku
        }
    return tab_lowp;
}

double* meansignal(double tab[], int range){ //wyznaczenie sredniej ruchomej
//y[n]=1/(N-1) - addition from k=0 to N-1 of y[n-k]
    double* mean_tab = new double[range];
    for (int i=0; i<range; i++) { //kopiownaie danych wejsciowych
        mean_tab[i] = tab[i];
    }
    int N=8;
    double addition=0;

    //przetwarzanie w oknach po 8 probek
    for (int i = 0; i < range / 8; i++) {
        addition = 0;
        for (int k = N - 8; k < N; k++) {
            addition += mean_tab[k];
        }
        double mean = addition / 8.0;
        mean_tab[i] = mean;
        N += 8;
    }
    return mean_tab;
}

double* square(double tab[], int range) { //podnoszenie wartosci sygnalu do kwadratu
    double* tab_squared = new double[range];
    for (int i=0; i<range; i++) {
        tab_squared[i] = tab[i];
    }
    for(int i=0;i<range;i++) {
        tab_squared[i] = (tab[i]*tab[i]);
    }
    return tab_squared;
}

//wyznaczanie zakresu (minimalnych i maksymalnych wartosci) oraz skalowanie dla kazdej z 4 tablic
double* zakres(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, int range){

    double* tab_range=new double[4];
    double* tabs[4] = {tab, tab_lowp, mean_tab, tab_squared};

    int min1, min2, min3, min4; // MIN VALUES
    min1=min2=min3=min4=numeric_limits<int>::infinity(); //inicjalizacja skrajnymi wartosciami (infinity)
    int max1, max2, max3, max4; // MAX VALUES
    max1=max2=max3=max4=-numeric_limits<int>::infinity();

    int mins[4] ={min1,min2,min3,min4};
    int maxs[4] ={max1,max2,max3,max4};

    //szukanie minimum i maksimum dla kazej z tablic
    for(int i=0;i<4;i++) {
        for(int j=0; j<range; j++) {
            if(mins[i]>tabs[i][j]) mins[i]=tabs[i][j];
            if(maxs[i]<tabs[i][j]) maxs[i]=tabs[i][j];
        }
        tab_range[i]=(maxs[i]-mins[i])/80; //skalowanie do 80 kolumn na wykresie
    }
    return tab_range;
}

//rysowanie wszystkich 4 wykresow w konsoli
void chart_four(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, const double *tab_range, int range) {
    double* tabs[4] = {tab, tab_lowp, mean_tab, tab_squared};
    const int width = 80;
    int height = range;
    char symbols[4] = {'*', '$', '#', '&'}; //inne symbole dla kazdego z wykresow

    //wyznaczenie minimow lokalnych aby znormalizowac os X
    double mins[4];
    for (int j = 0; j < 4; j++) {
        mins[j] = tabs[j][0];
        for (int l = 1; l < range; l++) {
            if (tabs[j][l] < mins[j]) {
                mins[j] = tabs[j][l];
            }
        }
    }

    //siatka 2D z pustymi znakami (spacjami)
    char** grid = new char*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
        for (int c = 0; c < width; c++) {
            grid[i][c] = ' ';
        }
    }

    // Rysowanie i skalowanie indywidualne
    for (int i = 0; i < range; i++) {
        for (int j = 0; j < 4; j++) {
            double val = tabs[j][i];
            int c = 0;

            //skalowanie do szerokosci 80
            if (tab_range[j] > 0) {
                c = static_cast<int>(round((val - mins[j]) / tab_range[j]));
            } else {
                c = width / 2; //jesli sygnal jest staly to rysuje na srodku szerokosci
            }

            //zabezpieczenie przed wyjsciem poza zakres szerokosci
            if (c < 0) c = 0;
            if (c >= width) c = width - 1;

            //nakladanie sie wykresow (oznaczane '+')
            if (grid[i][c] != ' ' && grid[i][c] != symbols[j]) {
                grid[i][c] = '+';
            } else {
                grid[i][c] = symbols[j];
            }
        }
    }

    // Wyświetlanie wykresu
    for (int m = 0; m < height; m++) {
        for (int n = 0; n < width; n++) {
            cout << grid[m][n];
        }
        cout << "\n";
    }

    // Zwolnienie pamięci siatki
    for (int i = 0; i < height; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}
