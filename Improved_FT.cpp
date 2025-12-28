#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const double PI = acos(-1.0);

// Tables of cos, sin calculations
vector<vector<double>> cosx, sinx;
vector<vector<double>> cosy, siny;

void precompute_trig(int Nx, int Ny) {

    cosx.assign(Nx, vector<double>(Nx));
    sinx.assign(Nx, vector<double>(Nx));
    cosy.assign(Ny, vector<double>(Ny));
    siny.assign(Ny, vector<double>(Ny));

    for (int kx = 0; kx < Nx; kx++) {
        for (int x = 0; x < Nx; x++) {
            double theta = -2.0 * PI * kx * x / Nx;
            cosx[kx][x] = cos(theta);
            sinx[kx][x] = sin(theta);
        }
    }

    for (int ky = 0; ky < Ny; ky++) {
        for (int y = 0; y < Ny; y++) {
            double theta = -2.0 * PI * ky * y / Ny;
            cosy[ky][y] = cos(theta);
            siny[ky][y] = sin(theta);
        }
    }
}



// 2D Fourier Transform (kx, ky) component
pair<double,double> calc_one_freq(
    const vector<vector<double>>& image,
    int kx, int ky
){
    int Ny = image.size(); // height
    int Nx = image[0].size(); // width

    double real_sum = 0.0; // integral of real part / initialization
    double imag_sum = 0.0; // integral of imaginary part / initialization

    for (int y = 0; y < Ny; y++) {
        for (int x = 0; x < Nx; x++) {

            real_sum += image[y][x] * (cosx[kx][x] * cosy[ky][y] - sinx[kx][x] * siny[ky][y]);
            imag_sum += image[y][x] * (sinx[kx][x] * cosy[ky][y] + cosx[kx][x] * siny[ky][y]);

        }
    }

    return {real_sum, imag_sum};
}


// 2D Fourier Transform 
vector<vector<pair<double,double>>> D2_FT(
    const vector<vector<double>>& image)
{
    int Ny = image.size(); // height
    int Nx = image[0].size(); // width

    vector<vector<pair<double,double>>> F(
        Ny, vector<pair<double,double>>(Nx)
    );

    for (int ky = 0; ky < Ny; ky++) {
        for (int kx = 0; kx < Nx; kx++) {
            F[ky][kx] = calc_one_freq(image, kx, ky);
        }
    }

    return F;
}

// Calculate the ampletude map of the FT image
vector<vector<double>> calc_amp(const vector<vector<pair<double,double>>>& FT) {
    int Ny = FT.size();
    int Nx = FT[0].size();

    vector<vector<double>> amp(Ny, vector<double>(Nx, 0.0));

    for (int y = 0; y < Ny; y++) {
        for (int x = 0; x < Nx; x++) {

            double real = FT[y][x].first;
            double imag = FT[y][x].second;

            amp[y][x] = sqrt(real*real + imag*imag);
        }
    }

    return amp;
}

// FTshift for visualization
vector<vector<pair<double,double>>> ftshift(const vector<vector<pair<double,double>>>& FT) {
    int Ny = FT.size();
    int Nx = FT[0].size();

    vector<vector<pair<double,double>>> out(Ny, vector<pair<double,double>>(Nx));

    int cy = Ny / 2;
    int cx = Nx / 2;

    for (int y = 0; y < Ny; y++) {
        for (int x = 0; x < Nx; x++) {
            // new position
            int yy = (y + cy) % Ny;
            int xx = (x + cx) % Nx;
            out[yy][xx] = FT[y][x];
        }
    }
    return out;
}


int main() {
    // Load image.txt
    ifstream fin("image.txt");
    vector<vector<double>> img;
    double v;
    vector<double> row;

    string line;
    while (getline(fin, line)) {
        row.clear();
        double val;
        stringstream ss(line);
        while (ss >> val) row.push_back(val);
        img.push_back(row);
    }

    // compute cos,sin in advance 
    int Ny = img.size();
    int Nx = img[0].size();
    precompute_trig(Nx, Ny);
    
    // FourierTransform, FTSHIFT
    auto FT = D2_FT(img);
    auto FT_shift = ftshift(FT); 

/*
    for (int ky = 0; ky < img.size(); ky++) {
        for (int kx = 0; kx < img[0].size(); kx++) {
            cout << "(" << FT_shift[ky][kx].first << ", "
                       << FT_shift[ky][kx].second << ") ";
        }
        cout << endl;
    }
    return 0;
    ofstream fout("FT.txt");
    for (int ky=0; ky<img.size(); ky++){
        for (int kx=0; kx<img[0].size(); kx++){
            fout << FT_shift[ky][kx].first << " " << FT_shift[ky][kx].second << "  ";
        }
        fout << "\n";
    }
*/  

    // compute amplitude |S_\nu(u,v)|
    auto amp = calc_amp(FT_shift);

    // output 
    ofstream fout("amplitude.txt");
    for (int ky=0; ky<amp.size(); ky++) {
        for (int kx=0; kx<amp[0].size(); kx++) {
            fout << amp[ky][kx] << " ";
        }
        fout << "\n";
    }

    return 0;
}

