#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include "rpi_motioncam/components/RaspiOverlayRenderer.h"
#include "rpi_motioncam/callbacks/MotionVectorCallback.h"
#include "rpi_motioncam/RPiMotionCam.h"
#include <fstream>

using namespace std;
using namespace rpi_motioncam;

RPIMOTIONCAM_OPTION_S options = RPiMotionCam::createMotionCamDefaultOptions();
MotionVectorCallback callback(options);

void printRegions(vector< shared_ptr< MotionRegion > > regions) {
    int grid[ options.resizer_height / 16 ][ options.resizer_width / 16];
    for (int row = 0; row < options.resizer_height / 16; row++) {
        for (int col = 0; col < options.resizer_width / 16; col++) {
            grid[row][col] = 0;
        }
    }
    int regionNum = 0;
    for (auto it = regions.begin(); it != regions.end(); ++it) {
        regionNum++;
        shared_ptr< MotionRegion > region = *it;
        for (int row = region->row; row < region->row + region->height; row++) {
            for (int col = region->col; col < region->col + region->width; col++) {
                grid[row][col] = regionNum;
            }
        }
    }
    cout << setw(5) << "   ";
    for (int col = 0; col < options.resizer_width / 16; col++) {
        cout << setw(3) << col;
    }
    cout << endl;

    for (int row = 0; row < options.resizer_height / 16; row++) {
        cout << setw(4) << row << ":"; 
        for (int col = 0; col < options.resizer_width / 16; col++) {
            cout << setw(3) << grid[row][col];
        }
        cout << endl;
    }

}

void printMatrix(uint8_t* frame) {
    int first_row = -1;
    int first_col = -1;
    cout << setw(5) << "   ";
    for (int col = 0; col < options.resizer_width / 16; col++) {
        cout << setw(3) << col;
    }
    cout << endl;

    for (int row = 0; row < options.resizer_height / 16; row++) {
        cout << setw(4) << row << ":"; 
        for (int col = 0; col < options.resizer_width / 16; col++) {
            if (frame[callback.buffer_pos(row, col)] > MOTION_THRESHOLD_DEFAULT) {
                cout << setw(3) << "xx";
                if (first_row == -1) {
                    first_row = row;
                    first_col = col;
                }
            } else {
                cout << setw(3) << "  ";
            }
            //cout << setw(4) << (int)frame[callback.buffer_pos(row, col)];
        }
        cout << endl;
    }
    // cout << "First hit at row " << first_row << " column " << first_col << endl;
} 

void print_region(MotionRegion region) {
    cout << "Region: row " << region.row << " column " << region.col << " width " << region.width << " height " << region.height << endl;
}

int main(int argc, char** argv) {
    ifstream vectorFile("vectors", ios::in | ios::binary);
    char frame[4920];
    MMAL_BUFFER_HEADER_T buffer;
    buffer.data = (uint8_t*)frame;
    buffer.flags = buffer.flags | MMAL_BUFFER_HEADER_FLAG_CODECSIDEINFO;
    for (int i = 0; i < 100; i++) {
        vectorFile.read(frame, 4920);
        cout << "*** Frame " << i << "***" << endl;
        printMatrix((uint8_t*)frame); 
        callback.callback(NULL, &buffer);
        printRegions(callback.lastFrame.regions);
        cout << endl;
   }
}
