#include "ITG3200.h"
 
Serial pc(USBTX, USBRX);
ITG3200 gyro(p9, p10);
 
int main() {
 
    pc.printf("Now starting ITG-3200 test...\n");
    
    //Set highest bandwidth.
    gyro.setLpBandwidth(LPFBW_42HZ);
 
    while (1) {
 
        //Arbitrary wait for printf clarity.
        wait(0.1);
        pc.printf("%i, %i, %i\n", gyro.getGyroX(),
                  gyro.getGyroY(), gyro.getGyroZ());
 
    }
 
}