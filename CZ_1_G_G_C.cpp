#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cmath>
using namespace std;

double* gettable(int range);
double* lowp(double tab[], int range);
double* meansignal(double tab[], int range);
double* square(double tab[], int range);
double* zakres(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, int range);
void chart_four(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, const double *tab_range, int range);

int main() {
    int range = 2000;

    double* tab_main    = gettable(range);
    double* tab_lowp    = lowp(tab_main, range);
    double* mean_tab    = meansignal(tab_lowp, range);
    double* tab_squared = square(mean_tab, range);
    double* tab_range = zakres(tab_main, tab_lowp, mean_tab, tab_squared, range);
    chart_four(tab_main, tab_lowp, mean_tab, tab_squared, tab_range, range);

    delete[] tab_main;
    delete[] tab_lowp;
    delete[] mean_tab;
    delete[] tab_squared;
    delete[] tab_range;

    return 0;
}





double* gettable(int range) {
    double* tab = new double[range];

string temp;

    ifstream reader("samples.csv");

    getline(reader, temp);
    getline(reader, temp);

tab[range+1];
float sample;

for(int i=0;reader.good() && i<range; i++){
    getline(reader,temp,',');
    getline(reader,temp);
    sample=200*stof(temp);
    tab[i]=sample;
    }
    return tab;
}

double* lowp(double tab[], int range){
    double* tab_lowp = new double[range];
    //y(n)=x(n)-x(n-7)
    int N=7;
        for(int j=N;j<range;j++){
            tab_lowp[j]=(tab[j]-tab[j-N]);
            tab_lowp[j]=round(tab_lowp[j]*1000)/1000;
        }
    return tab_lowp;
}

double* meansignal(double tab[], int range){
//y[n]=1/(N-1) - addition from k=0 to N-1 of y[n-k]
    double* mean_tab = new double[range];
    for (int i=0; i<range; i++) {
        mean_tab[i] = tab[i];
    }
    int N=8;
    double addition=0;

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

double* square(double tab[], int range) {
    double* tab_squared = new double[range];
    for (int i=0; i<range; i++) {
        tab_squared[i] = tab[i];
    }
    for(int i=0;i<range;i++) {
        tab_squared[i] = (tab[i]*tab[i]);
    }
    return tab_squared;
}

double* zakres(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, int range){

    double* tab_range=new double[4];
    double* tabs[4] = {tab, tab_lowp, mean_tab, tab_squared};

    int min1, min2, min3, min4; // MIN VALUES
    min1=min2=min3=min4=numeric_limits<int>::infinity();
    int max1, max2, max3, max4; // MAX VALUES
    max1=max2=max3=max4=-numeric_limits<int>::infinity();

    int mins[4] ={min1,min2,min3,min4};
    int maxs[4] ={max1,max2,max3,max4};

    for(int i=0;i<4;i++) {
        for(int j=0; j<range; j++) {
            if(mins[i]>tabs[i][j]) mins[i]=tabs[i][j];
            if(maxs[i]<tabs[i][j]) maxs[i]=tabs[i][j];
        }
        tab_range[i]=(maxs[i]-mins[i])/80;
    }
    return tab_range;
}

void chart_four(double *tab, double *tab_lowp, double *mean_tab, double *tab_squared, const double *tab_range, int range) {
    double* tabs[4] = {tab, tab_lowp, mean_tab, tab_squared};
    const int width = 80;
    int height = range;
    char symbols[4] = {'*', '$', '#', '&'};

    double mins[4];
    for (int j = 0; j < 4; j++) {
        mins[j] = tabs[j][0];
        for (int l = 1; l < range; l++) {
            if (tabs[j][l] < mins[j]) {
                mins[j] = tabs[j][l];
            }
        }
    }

    //siatka
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

            if (tab_range[j] > 0) {
                c = static_cast<int>(round((val - mins[j]) / tab_range[j]));
            } else {
                c = width / 2;
            }

            if (c < 0) c = 0;
            if (c >= width) c = width - 1;

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