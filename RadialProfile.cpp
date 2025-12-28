#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;


int main(){
    // Load amplitude.txt
    ifstream fin("amplitude.txt");
    vector<vector<double>> amp;
    vector<double> row;

    string line;
    while (getline(fin, line)) {
        row.clear();
        stringstream ss(line);
        double val;
        while (ss >> val) row.push_back(val);
        amp.push_back(row);
    }

    int Ny = amp.size();
    int Nx = amp[0].size();

    int cx = Nx / 2;   // center (fftshifted is assumed)
    int cy = Ny / 2;

    ofstream fout("r_amp_list.txt");

    // compute r for each pixel and output (r, amp)
    for (int y = 0; y < Ny; y++) {
        for (int x = 0; x < Nx; x++) {

            double dx = x - cx;
            double dy = y - cy;
            double r = sqrt(dx*dx + dy*dy);

            fout << r << " " << amp[y][x] << "\n";
        }
    }

    return 0;
}

