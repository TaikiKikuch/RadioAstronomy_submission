#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;



/* Creat geometric images include a point source, a uniform
plane, a Gaussian, two point sources, a ring */
vector<vector<double>> Create_Img(
        int Ny = 256,
        int Nx = 256,
        const string& Type = "PointSource",   // option : "PointSource", "Uniform", "2DGaussian", "TwoPointSource", "Ring"
        double sigma = 20.0,      // StandarDeviation of the Gaussian
        double ring_radius = 40.0,
        double ring_width = 3.0
    )
{
    vector<vector<double>> img(Ny, vector<double>(Nx, 0.0)); // Initialized image

    // central position
    int cy = Ny / 2; 
    int cx = Nx / 2;

    if (Type == "PointSource") {

        img[cy][cx] = 1.0;

    } else if (Type == "Uniform") {

        for (int y = 0; y < Ny; y++)
            for (int x = 0; x < Nx; x++)
                img[y][x] = 1.0;

    } else if (Type == "2DGaussian") {

        for (int y = 0; y < Ny; y++) {
            for (int x = 0; x < Nx; x++) {
                // deviation from the center 
                double dy = y - cy;
                double dx = x - cx;
                img[y][x] = exp(-(dx*dx + dy*dy) / (2.0 * sigma * sigma)); // gaussian e^{-(x^2+y^2)/(2sigma^2)}
            }
        }

    } else if (Type == "TwoPointSource"){

        img[cy/2][cx] = 1.0;
        img[3*cy/2][cx] = 1.0;

    } else if (Type == "Ring"){
        
        for (int y = 0; y < Ny; y++) {
            for (int x = 0; x < Nx; x++) {
                double dy = y - cy;
                double dx = x - cx;
                double r = sqrt(dx*dx + dy*dy);

                // fill   ring-0.5width < r < ring+0.5width
                if (fabs(r - ring_radius) <= ring_width * 0.5 ) {
                    img[y][x] = 1.0;
                }
            }
        }
    } else {

    }

    return img;
}


int main(int argc , char* argv[]) {
    // const string type = "PointSource";
    const string type = argv[1]; // recieve type from python
    vector<vector<double>> img = Create_Img(256,256,type, 5);

    // output image.txt
    ofstream fout("image.txt");
    for (int y=0; y<img.size(); y++) {
        for (int x=0; x<img[0].size(); x++) {
            fout << img[y][x] << " " ;
        }
        fout << "\n";
    }
}

