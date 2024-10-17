#include "raylib.h"
#include <raymath.h>

const static int NUM_STARS = 1000;

typedef struct Star {
  Vector2 origin;
  Vector2 dest;
  Color color;
  float time;
  bool alive;
} Star;

static Font font;
static Vector2 textSize;
const static char text[] = "FORTNITE";
static Star stars[1000];
static Vector2 center;
static Vector2 pos;
static Vector3 speed;
static float rot;

void InitStars(Star stars[NUM_STARS]);
void DrawStars(Star stars[NUM_STARS]);
void UpdateTitle(Vector2 *pos, Vector3 *speed, float *rot, float deltaTime);
void DrawTitle(Vector2 pos, float rot);
void UpdateStars(Star stars[NUM_STARS], Vector2 center, float deltaTime);

void InitEnd(Font a) {
  font = a;
  textSize = MeasureTextEx(font, text, 48, 1);
  float w = GetScreenHeight();
  float h = GetScreenWidth();
  pos = (Vector2){(float)w / 2, (float)h / 2};
  speed = (Vector3){200, 200, 50};
  center = (Vector2){(float)w / 2, (float)h / 2};

  InitStars(stars);
}

Color GetRandomColor() {
  return (Color){GetRandomValue(1, 255), GetRandomValue(1, 255),
                 GetRandomValue(1, 255), GetRandomValue(200, 255)};
}

void RunEnd(float deltaTime, float w, float h) {
  center.x = w / 2;
  center.y = h / 2;
  ClearBackground(BLACK);
  DrawStars(stars);
  UpdateStars(stars, center, deltaTime);
  UpdateTitle(&pos, &speed, &rot, deltaTime);
  DrawTitle(pos, rot);
}

void InitStars(Star stars[NUM_STARS]) {
  for (int i = 0; i < NUM_STARS; ++i) {
    Vector2 origin = (Vector2){GetRandomValue(0, GetScreenWidth()),
                               GetRandomValue(0, GetScreenHeight())};
    stars[i] = (Star){.origin = origin,
                      .dest = origin,
                      .time = 4.0,
                      .color = GetRandomColor(),
                      .alive = true};
  }
}

void DrawStars(Star stars[NUM_STARS]) {
  for (int i = 0; i < NUM_STARS; ++i) {
    if (stars[i].alive) {
      DrawLineV(stars[i].origin, stars[i].dest, stars[i].color);
    } else {
      // DrawLineV(stars[i].origin, stars[i].dest, RED);
    }
  }
}

void UpdateTitle(Vector2 *pos, Vector3 *speed, float *rot, float deltaTime) {

  if (IsWindowResized()) {
    float w = GetScreenWidth();
    float h = GetScreenHeight();
    if (pos->x <= 0 || pos->x >= w || pos->y <= 0 || pos->y >= h) {
      pos->x = w / 2;
      pos->y = h / 2;
    }
  }
  if (pos->x <= 0 || pos->x + textSize.x >= GetScreenWidth()) {
    speed->x *= -1;
    pos->x += speed->x * deltaTime;
  }
  if (pos->y <= 0 || pos->y + textSize.y >= GetScreenHeight()) {
    speed->y *= -1;
    pos->y += speed->y * deltaTime;
  }
  pos->x += speed->x * deltaTime;
  pos->y += speed->y * deltaTime;
  *rot = *rot + speed->z * deltaTime;
  *rot = fmod(*rot, 360);
}

void DrawTitle(Vector2 pos, float rot) {
  DrawTextEx(font, text, pos, 48, 1, WHITE);
}

void UpdateStars(Star stars[NUM_STARS], Vector2 center, float deltaTime) {
  for (int i = 0; i < NUM_STARS; ++i) {
    if (stars[i].alive) {
      Vector2 diff = Vector2Subtract(stars[i].origin, center);
      Vector2 norm = Vector2Normalize(diff);
      Vector2 scale = Vector2Scale(norm, deltaTime * 100);
      stars[i].origin = Vector2Add(stars[i].origin, scale);
      scale = Vector2Scale(scale, 2);
      stars[i].dest = Vector2Add(stars[i].dest, scale);

      if (stars[i].origin.x <= 0 || stars[i].origin.x > GetScreenWidth() ||
          stars[i].origin.y < 0 || stars[i].origin.y > GetScreenHeight()) {
        stars[i].alive = false;
      }
      stars[i].time -=
          400 * deltaTime / Vector2Distance(stars[i].origin, center);
      if (stars[i].time <= 0) {
        stars[i].alive = false;
      }

    } else {
      Vector2 origin = (Vector2){GetRandomValue(0, GetScreenWidth()),
                                 GetRandomValue(0, GetScreenHeight())};
      stars[i] = (Star){.origin = origin,
                        .dest = origin,
                        .time = 4.0,
                        .color = GetRandomColor(),
                        .alive = true};
    }
  }
}
