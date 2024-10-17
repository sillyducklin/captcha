#include "raylib.h"
#include <math.h>
#include <raymath.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846 /* pi */

// main struct that stores everything
typedef struct Lilac {
  int pos;
  float time;
  float animTime;
  Color color;
  Color bg;
  Texture2D texture;
  Vector2 positions[12];
  float offset;
} Lilac;

// just convient way to store two colors
typedef struct Combo {
  Color bg;
  Color color;
} Combo;

// random colors i decided were good enough
static int correct;
static int index = 0;
const static Combo colors[] = {
    {{191, 191, 191, 255}, {221, 147, 221, 255}},
    {{170, 186, 106, 65}, {0, 255, 48, 140}},
    {{200, 200, 200, 255}, {0, 228, 48, 155}},
    {{233, 13, 56, 68}, {86, 111, 200, 101}},
    {{11, 225, 239, 194}, {84, 41, 190, 126}},
    {{198, 248, 223, 253}, {201, 199, 113, 161}},
};

// the correct answers to the above colors
// the indicies are aligned
const static Color expected[] = {
    {169, 235, 180, 255}, {240, 153, 224, 255}, {235, 169, 230, 255},
    {237, 182, 147, 255}, {29, 242, 210, 255},  {199, 235, 229, 255},
};

// randomly decided to abandon the struct and define things statically
static Color answerButtons[4];

// decides where to draw the individual circles
void InitLilac(Lilac *lilac, float w, float h) {
  int radius = h * .2;
  int len = 12;
  for (int i = 0; i < len; i++) {
    float angle = (float)i * (2 * M_PI / len);
    lilac->positions[i] = (Vector2){(w * .5) + radius * cos(angle),
                                    (h * .45) + radius * sin(angle)};
  }
}

// returns a random color
Color getRandomColor() {
  return (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                 GetRandomValue(0, 255), 255};
}
// ensures the colors are not close to the correct answer
float Distance(Color c1, Color c2) {
  float dif =
      sqrt(pow(c1.r - c2.r, 2) + pow(c1.g - c2.g, 2) + pow(c1.b - c2.b, 2));
  return dif;
}

bool checkDistances(int idx) {
  float threshold = 100;
  if (idx == correct) {
    return true;
  }
  for (int i = 0; i < 4; i++) {
    if (i != idx) {
      if (Distance(answerButtons[idx], answerButtons[i]) < threshold) {
        return false;
      }
    }
  }
  return true;
}

void randomizeColorsNorm() {
  for (int i = 0; i < 4; i++) {
    while (!checkDistances(i)) {
      answerButtons[i] = getRandomColor();
    }
  }
}

// inits the structure and other global vars
Lilac NewLilac() {
  Image lilacI = LoadImage("./resources/circle.png");
  Texture2D lilacT = LoadTextureFromImage(lilacI);

  float w = GetScreenWidth();
  float h = GetScreenHeight();
  // index into the correct / color array
  index = GetRandomValue(0, 5);
  correct = GetRandomValue(0, 3);
  for (int i = 0; i < 4; i++) {
    answerButtons[i] = getRandomColor();
  }
  answerButtons[correct] = expected[index];
  randomizeColorsNorm();

  Lilac lilac = {.pos = 0,
                 .time = .15,
                 .animTime = .15,
                 .color = colors[index].color,
                 .bg = colors[index].bg,
                 .texture = lilacT,
                 .offset = 0};
  InitLilac(&lilac, w, h);
  return lilac;
}

// used for testing/creating things
/*Color invertColor(Color c) {
  return (Color){255 - c.r, 255 - c.g, 255 - c.b, c.a};
}*/

// random const static, classic
const static bool spin = true;
static bool go = true;
// main logic for the illusion, this draws everything and updates the moving
// parts. all the math is terrible and arbitrary but somehow looks decent i
// think
int LilacChaser(Lilac *lilac, Font azuki, float deltaTime) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  int radius = h * .18;
  int len = 12;
  int lilacWidth = 70;
  int lilacHeight = 70;
  float scale = .45;
  Color textColor = BLACK;
  if (IsWindowResized()) {
    InitLilac(lilac, w, h);
  }

  if (go) {
    if (spin) {
      lilac->offset = fmod(lilac->offset + deltaTime * 20, 360);
      for (int i = 0; i < len; i++) {
        float angle = (float)(i + lilac->offset) * (2 * M_PI / len);
        lilac->positions[i] = (Vector2){(w * .5) + radius * cos(angle),
                                        (h * .45) + radius * sin(angle)};
      }
    } else {
      lilac->time -= deltaTime * 1.5;
      if (lilac->time < 0) {
        lilac->time = lilac->animTime;
        lilac->pos = (lilac->pos + 1) % 12;
      }
    }
  }

  // switching for testing
  /*if (IsKeyPressed(KEY_R)) {
     index = (index + 1) % 6;
    lilac->bg = colors[index].bg;
    lilac->color = colors[index].color;
    answerButtons[0] = getRandomColor();
    answerButtons[1] = getRandomColor();
    answerButtons[2] = getRandomColor();
    answerButtons[3] = getRandomColor();
    answerButtons[correct] = expected[index];
    randomizeColorsNorm();
  }*/
  /*if (IsKeyPressed(KEY_N)) {
    go = !go;
  }*/
  // testing purposes
  /*DrawRectangleLines(25, 45, 50, 100, BLACK);
  DrawRectangle(30, 50, 40, 40, lilac->bg);
  DrawRectangle(30, 100, 40, 40, lilac->color);
  DrawText(TextFormat("%d", index), 10, 10, 20, WHITE);*/

  // for geting colors
  /*if (IsMouseButtonPressed(0)) {
    lilac->bg = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                        GetRandomValue(0, 255), GetRandomValue(0, 255)};
  }
  if (IsMouseButtonPressed(1)) {
    lilac->color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                           GetRandomValue(0, 255), GetRandomValue(100, 255)};
  }
  if (IsKeyDown(KEY_C)) {
    Color c1 = lilac->bg;
    Color c2 = lilac->color;
    SetClipboardText(
        TextFormat("(Combo){(Color){%d, %d, %d, %d}, (Color){%d, %d, %d, %d}},",
                   c1.r, c1.g, c1.b, c1.a, c2.r, c2.g, c2.b, c2.a));
  }*/

  // more random math to draw things where i need them
  float offset = radius + .06 * h;
  Rectangle rec = {
      (w / 2) - offset,
      (h * .45) - offset,
      offset * 2,
      offset * 2,
  };
  DrawRectangleRec(rec, lilac->bg);
  for (int i = 0; i < len; i++) {
    if (i == lilac->pos) {
      continue;
    }
    Rectangle src = {0, 0, lilacWidth, lilacHeight};
    Rectangle dest = {lilac->positions[i].x, lilac->positions[i].y,
                      radius * scale, radius * scale};
    // draws the circles that are on the screen
    DrawTexturePro(lilac->texture, src, dest,
                   (Vector2){((radius * scale) / 2), ((radius * scale) / 2)}, 0,
                   lilac->color);
    // DrawCircleGradient(dest.x, dest.y, .04 * h, (Color){221, 147, 221, 180},
    // DrawCircleGradient(dest.x, dest.y, .04 * h, lilac->color, lilac->bg);
  }
  // inner dot of illusiont
  DrawRectangle(w / 2, h * .45, 4, 4, BLACK);
  // outside box of illusion
  DrawRectangleLinesEx(rec, 3, BLACK);

  // Text at the top of the screen that gives directions (math is used to update
  // pos according to text/window size)
  char topText[] = "Look at the black dot in the middle.";
  Vector2 textSize = MeasureTextEx(azuki, topText, 24, 1);
  DrawTextEx(azuki, topText, (Vector2){(w / 2) - (textSize.x / 2), h * .1}, 24,
             1, textColor);
  char bottomText[] = "Which color appears in the absence of the dots?";
  textSize = MeasureTextEx(azuki, bottomText, 24, 1);
  DrawTextEx(azuki, bottomText,
             (Vector2){(w / 2) - (textSize.x / 2), h * .1 + (textSize.y * 1.5)},
             24, 1, textColor);

  // Draw the buttons at the bottom
  // This is the end of the other section, so the top of the button section
  // 100 - 25 - 45 = .3 left of the v space.
  // 10

  // variables used for button placement math to make it scale with window size
  // (technially only need run when the screen size is upadted but computers are
  // fast)
  float top = (h * .45) + offset + 1;
  float vSpace = (h - top);
  float vGap = vSpace * .1;
  float buttonW = w * .2;
  float buttonH = vSpace * .35;
  float centerX = w / 2;
  float roundness = .7;
  Vector2 mousePos = GetMousePosition();
  // Color buttonOutline = BLACK;
  //  top left
  Rectangle topLeft = {centerX - .025 * w - buttonW, top + vGap, buttonW,
                       buttonH};
  DrawRectangleRounded(topLeft, roundness, 0, answerButtons[0]);

  //  top right
  Rectangle topRight = {.025 * w + centerX, top + vGap, buttonW, buttonH};
  DrawRectangleRounded(topRight, roundness, 0, answerButtons[1]);

  //  bot left
  Rectangle botLeft = {centerX - .025 * w - buttonW, top + vGap * 2 + buttonH,
                       buttonW, buttonH};
  DrawRectangleRounded(botLeft, roundness, 0, answerButtons[2]);

  //  bot right
  Rectangle botRight = {
      .025 * w + centerX,
      top + vGap * 2 + buttonH,
      buttonW,
      buttonH,
  };
  DrawRectangleRounded(botRight, roundness, 0, answerButtons[3]);

  // DrawRectangleRoundedLines(botRight, roundness, 0, thickness,
  // buttonOutline);
  //  Check if clicked
  if (IsMouseButtonPressed(0)) {
    // track if clicked is in the answer boxes
    int clicked = -1;
    if (CheckCollisionPointRec(mousePos, topLeft)) {
      clicked = 0;
    }
    if (CheckCollisionPointRec(mousePos, topRight)) {
      clicked = 1;
    }
    if (CheckCollisionPointRec(mousePos, botLeft)) {
      clicked = 2;
    }
    if (CheckCollisionPointRec(mousePos, botRight)) {
      clicked = 3;
    }

    // this is the return were it advances the enum
    if (clicked != -1) {
      if (clicked == correct) {
        return 1;
      }
      return 2;
    }
  }

  return 0;
}
