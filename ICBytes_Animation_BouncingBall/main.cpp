#include "icb_gui.h"


ICBYTES image ;
int FRM;

// Animation variables
int ballX;
int ballY;
int radius = 15;
int velocityY = 0;
bool isBouncing = true;
int stepHeight = 60; // Height of each step
float gravity = 9.81; // Gravity force to apply
float bounce_factor = 0.7;
int currentStep = 0; // To track which step the ball is on
// Ball position limits for each step
int stepPositionsY[3] = { 220, 260, 300 }; // Y positions of the tops of the steps


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

ICBYTES hole = { {400,380,440,400},{400,380,480,380},{480,380,520,400},{440,400,520,400} };


void DrawHole(ICBYTES* image) {
    // Draw the hole with a darker color to simulate depth
    Line(*image,hole , 0x555555);


    // Görüntüyü güncelle
    DisplayImage(FRM, *image);
}


void UpdateBallPosition() {
    // Apply gravity
    velocityY += gravity;

    // Update ball's vertical position
    ballY += (int)velocityY;

    // Handle ball bouncing on each step
    if (ballY >= stepPositionsY[currentStep] - radius) {
        ballY = stepPositionsY[currentStep] - radius; // Ball hits the step

        // Bounce the ball
        if (velocityY > 0) { // If the ball is falling downwards
            velocityY = -velocityY * bounce_factor; // Reverse velocity and apply bounce factor
        }

        // Check if ball should drop to next step
        if (velocityY > -2.0f) { // Minimal velocity threshold to stop bouncing
            currentStep++;
            if (currentStep > 2) { // Ball has fallen beyond the last step
                isBouncing = false; // End bouncing
            }
        }
    }
}

void ICGUI_main() {
    FRM = ICG_FrameMedium(5, 5, 755, 535); //755*535 pixel frame
    CreateImage(image, 755, 535, ICB_UINT); //  755*535*32 bit pxel image matrix


    // Initialize ball position
    ballX = 320; // Top center of first step
    ballY = 20; // Start above the first step

    while (isBouncing) {
        Draw3DStairs(&image);
        DrawHole(&image);
        UpdateBallPosition(); // Update the ball's position each frame
        Draw3DBall(&image); // Draw ball after updating position
    }
   
}
