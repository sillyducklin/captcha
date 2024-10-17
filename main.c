#include "captcha.c"
#include "end.c"
#include "raylib.h"
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846 /* pi */

// Game Structs
enum SCREEN { INTRO, LOAD, DETECT, CAPTCHA, END, END_ROBOT };

// GLOBAL DEFINES
// sets base window height/width with great var names.

// Functions to draw the different screens on the program.
int DrawIntro();
int DrawLoad(float *loadTime, float deltaTime);
int DrawDetect(float *loadTime, float deltaTime, Texture2D image);
int DrawSelect();
int DrawCaptcha(Lilac *l, Color bg, float deltaTime);
int DrawEnd(float deltaTime);
int DrawEndRobot();

// font used throughout so make it global or something.
static Font font;

// main
int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1000, 650, "cool");

  // data init
  font = LoadFont("./resources/fonts/azuki.ttf");
  // font =
  // LoadFont("./resources/fonts/aDaddyTimeMonoNerdFontPropo-Regular.ttf");
  Image i = LoadImage("./resources/alarm.png");
  Texture2D image = LoadTextureFromImage(i);
  float loadTime = 0.0;
  Color bg = LIGHTGRAY;
  Lilac l = NewLilac();
  InitEnd(font);
  enum SCREEN currentScreen = INTRO;

  // Run program
  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    // main logic pretty self explainatory, advances the enum through return
    // values of the various drawing functions, and the main logic is just a
    // switch on the enum.
    // (note i don't manage memory that is the kernels job to clean after im
    // done.)
    BeginDrawing();
    {
      switch (currentScreen) {
      case (INTRO): {
        ClearBackground(bg);
        currentScreen += DrawIntro();
      } break;
      case (LOAD): {
        ClearBackground(bg);
        currentScreen += DrawLoad(&loadTime, deltaTime);
      } break;
      case DETECT: {
        ClearBackground(bg);
        currentScreen += DrawDetect(&loadTime, deltaTime, image);
      } break;
      case CAPTCHA: {
        ClearBackground(bg);
        currentScreen += DrawCaptcha(&l, bg, deltaTime);
      } break;
      case END: {
        // ClearBackground(bg);
        currentScreen += DrawEnd(deltaTime);
      } break;
      case END_ROBOT: {
        ClearBackground(bg);
        currentScreen += DrawEndRobot();
      } break;
      }
    }
    // DrawFPS(5, 5);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}

// Draws the intro screen. wow.
int DrawIntro() {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  Color textColor = BLACK;

  // Draw the stuff on the screen using questionable maths
  // the text size is just to make sure the text is centered
  char intro[] = "Welcome to the program !";
  Vector2 textSize = MeasureTextEx(font, intro, 24, 1);
  DrawTextEx(font, intro,
             (Vector2){(w / 2) - (textSize.x / 2), h / 2 - (textSize.y * 2.5)},
             24, 1, textColor);
  char review[] =
      "The reviews are in and some are saying its the best thing ever.";
  textSize = MeasureTextEx(font, review, 24, 1);
  DrawTextEx(font, review,
             (Vector2){(w / 2) - (textSize.x / 2), h / 2 - (textSize.y)}, 24, 1,
             textColor);
  char human[] = "only one problem, its for humans only, no robots.";
  textSize = MeasureTextEx(font, human, 24, 1);
  DrawTextEx(font, human,
             (Vector2){(w / 2) - (textSize.x / 2), h / 2 + (textSize.y * 0.5)},
             24, 1, textColor);

  // places things where i want them
  float width = w * .4;
  float height = h * .15;
  float x = w * .3;
  float y = h * .7;
  Rectangle button = (Rectangle){x, y, width, height};
  DrawRectangleRounded(button, .5, 4, BLACK);
  char buttonText[] = "Click to Enter";
  textSize = MeasureTextEx(font, buttonText, 24, 1);
  DrawTextEx(
      font, buttonText,
      (Vector2){(w / 2) - (textSize.x / 2), y + (height / 2 - textSize.y / 2)},
      24, 1, WHITE);
  // if mouse is pressed in the button box then advance to the next screen
  if (IsMouseButtonDown(0)) {
    if (CheckCollisionPointRec(GetMousePosition(), button)) {
      return 1;
    }
  }
  return 0;
}

void DrawBar(float loadTime, float totalTime, float w, float h, char name[]) {
  Color textColor = BLACK;
  Color barColor = WHITE;
  float width = w * .8;
  float height = h * .1;
  float x = w * .1;
  float y = h * .8;
  char buffer[48] = "Done.";
  float percent = 1;
  // if not done loading, get the percent it has loaded and make the string
  if (loadTime < totalTime) {
    percent = loadTime / totalTime;
    snprintf(buffer, 48, "Loading %s (%d%%)", name, (int)(100 * percent));
  }
  // draw the loading bar and text
  Vector2 textSize = MeasureTextEx(font, buffer, 24, 1);
  DrawTextEx(font, buffer,
             (Vector2){(w / 2) - (textSize.x / 2), y - (textSize.y * 1.5)}, 24,
             1, textColor);
  // outside rectangle
  DrawRectangleLinesEx((Rectangle){x, y, width, height}, 5, barColor);
  // inner rectangle
  DrawRectangleRec(
      (Rectangle){x + 8, y + 8, (percent) * (width - 16), (height - 16)},
      barColor);
}

// draws the loading screen
int DrawLoad(float *loadTime, float deltaTime) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();

  char text[] = "One moment please.";
  Vector2 textSize = MeasureTextEx(font, text, 24, 1);
  DrawTextEx(font, text,
             (Vector2){(w / 2) - (textSize.x / 2), h / 2 - (textSize.y / 2)},
             24, 1, BLACK);

  if (IsKeyDown(KEY_SPACE)) {
    *loadTime += deltaTime * 3;
  } else {
    *loadTime += deltaTime;
  }
  float totalTime = 4.0;
  DrawBar(*loadTime, totalTime, w, h, "Cool Thing");

  if (*loadTime > (totalTime + 1)) {
    *loadTime = 0;
    return 1;
  }
  return 0;
}

// draws the fake detect (no matter what it `detects` a robot and makes you do
// the captcha, kinda the whole point of the program.)
int DrawDetect(float *loadTime, float deltaTime, Texture2D image) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  float totalTime = 3.0;
  char text[] = "ROBOT DETECTED AWAITING CAPTCHA";
  Vector2 textSize = MeasureTextEx(font, text, 24, 1);
  float offsetX = (w / 2) - (textSize.x / 2);
  float offsetY = h / 2 - (textSize.y);
  DrawTextEx(font, text, (Vector2){offsetX, offsetY}, 24, 1, BLACK);

  float scale = .1;
  Rectangle src = {0, 0, 512, 512};
  Rectangle dest = {offsetX - w * .03, offsetY + (512 * scale / 4), 512 * scale,
                    512 * scale};
  //  Rectangle dest = {50, 50, 512 * scale, 512 * scale};
  // draws the circles that are on the screen
  DrawTexturePro(image, src, dest,
                 (Vector2){((512 * scale) / 2), ((512 * scale) / 2)}, 0, WHITE);

  Rectangle dest2 = {offsetX + textSize.x + w * .03,
                     offsetY + (512 * scale / 4), 512 * scale, 512 * scale};
  DrawTexturePro(image, src, dest2,
                 (Vector2){((512 * scale) / 2), ((512 * scale) / 2)}, 0, WHITE);
  if (IsKeyDown(KEY_SPACE)) {
    *loadTime += deltaTime * 3;
  } else {
    *loadTime += deltaTime;
  }

  DrawBar(*loadTime, totalTime, w, h, "Captcha");
  if (*loadTime >= totalTime) {
    return 1;
  }
  return 0;
}

// runs the selected captcha
int DrawCaptcha(Lilac *l, Color bg, float deltaTime) {
  return LilacChaser(l, font, deltaTime);
}

// draws the final screen if ur no a robot
int DrawEnd(float deltaTime) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  RunEnd(deltaTime, w, h);
  return 0;
}

// draws the final screen (regardless if your a robot or not)
int DrawEndRobot() {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  char text[] = "ur a robot i think, you missed out on the amazing thing";
  Vector2 textSize = MeasureTextEx(font, text, 24, 1);
  DrawTextEx(font, text,
             (Vector2){(w / 2) - (textSize.x / 2), h / 2 - (textSize.y)}, 24, 1,
             BLACK);
  char text2[] =
      "if you want to see the cool thing anyways just click the screen.";
  Vector2 textSize2 = MeasureTextEx(font, text2, 24, 1);
  DrawTextEx(font, text2,
             (Vector2){(w / 2) - (textSize2.x / 2), h / 2 + (textSize2.y)}, 24,
             1, BLACK);

  if (IsMouseButtonPressed(0) || IsMouseButtonPressed(1)) {
    // allow user to go back and see it anyways
    return -1;
  }
  return 0;
}
