#include "icb_gui.h"


ICBYTES imageBall ;
ICBYTES imageStairs;
ICBYTES image;

int FRM;

// Animation variables
int ballX=280;
int ballY=20;
int radius = 15;
double velocityY = 0;
bool isBouncing = true;
int stepHeight = 60; // Height of each step
double gravity = 9.81; // Gravity force to apply
double bounce_factor = 0.7;
int currentStep = 0; // To track which step the ball is on
// Ball position limits for each step
int stepPositionsY[3] = { 223, 244, 292 }; // Y positions of the tops of the steps


void ICGUI_Create() {
    ICG_MWTitle("Merdivenden Dusen Top Animasyonu");
    ICG_MWSize(800, 600); // Pencere boyutlarını ayarlıyoruz.
}


void Draw3DStairs(ICBYTES* image) {

    // Thrid step 
    FillRect(*image, 300, 300, 200, 60, 0xA0A0A0);
    HalfRect(*image, 300, 360, 40, -20, 0x000000);
    HalfRect(*image, 500, 300, -40, +20, 0x000000);

    //Second step
    FillRect(*image, 260, 260, 200, 80, 0xA0A0A0);
    HalfRect(*image, 260, 340, 40, -20, 0x000000);
    HalfRect(*image, 460, 260, -40, +20, 0x000000);

    //First step
    FillRect(*image, 220, 220, 200, 100, 0xA0A0A0);
    HalfRect(*image, 220,320, 40, -20, 0x000000);
    HalfRect(*image, 420, 220, -40, +20, 0x000000);
 
    //First step
    Line(*image, 220, 220, 260, 240, 0x000000);
    Line(*image, 260, 240, 260, 260, 0x000000);
    Line(*image, 260, 240, 420, 240, 0x000000);

    //Second step
    Line(*image, 260, 260, 300, 280, 0x000000);
    Line(*image, 300, 280, 300, 300, 0x000000);
    Line(*image, 300, 280, 460, 280, 0x000000);


    // Thrid step 
    Line(*image, 300, 300, 340, 320, 0x000000);
    Line(*image, 340, 320, 500, 320, 0x000000);
    Line(*image, 340, 320, 340, 360, 0x000000);


    DisplayImage(FRM, *image);

   
    
}

void Draw3DBall(ICBYTES* image) {
 

    for (int r = radius; r > 0; r--) {
        // Calculate color based on the radius
        unsigned int color = (0xFF << 16) | (0x00 << 8) | (0x00); // Start with red color
        color = (color & 0xFFFFFF) | (((r * 0xFF) / radius) << 24); // Adjust alpha for smoother gradient

        FillCircle(*image, ballX, ballY, r, color);
    }

    // Add a white highlight for reflection
    FillCircle(*image, ballX - 5, ballY - 5, 5, 0xFFFFFF); // White highlight

    // Görüntüyü güncelle
    DisplayImage(FRM, *image);


}

ICBYTES hole = { {380,400,500,550},{380,400,530,400},{530,400,710,600} };


void DrawHole(ICBYTES* image) {
    // Draw the hole with a darker color to simulate depth
    Line(*image,hole , 0x555555);

    int i;
    int x1=430, x2=590, y1=400, y2=600;
    for (i=0;i<70;i++)
    {

        Line(*image, x1,y1,x2,y2, 0x555555);
        x1 += 1;
        x2 += 1;

    }
    // Görüntüyü güncelle
    DisplayImage(FRM, *image);
}



// Define the coordinates for the rectangular hole
int holeX1 = 380; // Left X coordinate
int holeX2 = 700; // Right X coordinate
int holeY1 = 400; // Top Y coordinate
int holeY2 = 550; // Bottom Y coordinate 

static void UpdateBallPosition() {


 

    // Check for bouncing on steps
    if (currentStep < 3 && ballY >= stepPositionsY[currentStep] ) {
        ballY = stepPositionsY[currentStep] ; // Ball hits the step
   
        // Bounce the ball
        if (velocityY > 0) { // Only bounce if falling down
            velocityY = - velocityY * bounce_factor; // Reverse and reduce velocity

                
           
        }

        // Move to the next step if the bounce is small
        if (velocityY < -2.0f) { // Minimal velocity threshold to stop bouncing
            currentStep++;
            if (currentStep > 3) { // Check if on the last step
                isBouncing = false; // Stop bouncing if beyond last step
            }
        }
       
       
    }
  
      
    else {
        // If the ball is near the hole, initiate rolling into the hole
        if (ballX >= holeX1 && ballX <= holeX2 && ballY >= holeY1) {
            // Move the ball into the hole
            if (ballY < holeY2) {
                // Move downwards to simulate rolling into the hole
                ballY += 10; // Adjust this value for speed of rolling into the hole
                ballX += 7;
            }
            else {
                // Reset when fully in the hole
                ballX = (holeX1 + holeX2) / 2; // Center of the hole
                ballY = holeY2; // Bottom of the hole
                isBouncing = false; // Stop bouncing as it is in the hole
                currentStep = 0; // Reset the current step for the next loop
            }
        }
        else { // Apply gravity if not rolling into the hole
            velocityY += gravity; // Simulate gravity over a small time step
            if (currentStep > 0) {
                ballX += 10; // Move right if on the second or third step
            }
            // Update ball's vertical position
            ballY += (int)velocityY;
        }
    }


 
    
}

bool isRunning = true;
DWORD WINAPI AnimationThread(LPVOID lpParam) {

    while (isRunning) {
        while (isBouncing) {
            FillRect(image, 5, 5, 755, 535, 0x000000);
            Draw3DStairs(&image);
            DrawHole(&image);

            Draw3DBall(&image);
            UpdateBallPosition();
            Sleep(100); // Animation speed control
        }
        if (!isBouncing) {
            ballX = 280;
            ballY = 20;
            velocityY = 0;
            currentStep = 0;
            isBouncing = true; // Restart the bouncing loop
        }
    }
    return 0;
}

void ICGUI_main() {
    FRM = ICG_FrameMedium(5, 5, 755, 535); // Frame
    CreateImage(image, 755, 535, ICB_UINT); // Image matrix

    Draw3DStairs(&image);
    DrawHole(&image);

    // Start the animation in a separate thread
    DWORD threadId;
    HANDLE hThread = CreateThread(NULL, 0, AnimationThread, NULL, 0, &threadId);
    if (hThread == NULL) {
        MessageBox(NULL, "Failed to create thread", "Error", MB_OK);
        return;
    }
    // Close the thread handle (optional, depends on cleanup requirements)
    CloseHandle(hThread);
}
