// Game_simple.c - 简化版本
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include "gfx.h"

typedef struct {
    int x, y;
    int size;
    char letter;
    int speed;
    int active;
    int color_r, color_g, color_b;
} FallingObject;

void draw_object(FallingObject *obj) {
    if (!obj->active) return;

    gfx_color(obj->color_r, obj->color_g, obj->color_b);

    int half = obj->size / 2;
    gfx_line(obj->x - half, obj->y - half, obj->x + half, obj->y - half);
    gfx_line(obj->x + half, obj->y - half, obj->x + half, obj->y + half);
    gfx_line(obj->x + half, obj->y + half, obj->x - half, obj->y + half);
    gfx_line(obj->x - half, obj->y + half, obj->x - half, obj->y - half);

    // 字母用黑色
    gfx_color(0, 0, 0);
    char letter_str[2] = {obj->letter, '\0'};
    gfx_text(obj->x - 8, obj->y - 12, letter_str);
}

void init_object(FallingObject *obj, int width) {
    obj->x = 50 + rand() % (width - 100);
    obj->y = -50;
    obj->size = 40 + rand() % 40;
    obj->letter = 'A' + rand() % 8;
    obj->speed = 2 + rand() % 4;
    obj->active = 1;
    obj->color_r = rand() % 200 + 55;
    obj->color_g = rand() % 200 + 55;
    obj->color_b = rand() % 200 + 55;
}

int main() {
    const int width = 800, height = 600;
    int score = 0;
    int game_running = 1;
    int game_started = 0;

    srand(time(NULL));

    gfx_open(width, height, "Falling Letters Game");

    printf("=== FALLING LETTERS GAME ===\n");
    printf("Click window to start\n");
    printf("Press matching letter keys\n");
    printf("Press 'q' to quit\n");
    printf("============================\n");

    // 创建第一个对象
    FallingObject obj;
    init_object(&obj, width);

    while(game_running) {
        // 清屏
        gfx_clear();

        // 绘制对象（如果活跃）
        if (obj.active) {
            draw_object(&obj);
        }

        // 绘制分数
        gfx_color(0, 0, 0);
        char score_text[50];
        sprintf(score_text, "Score: %d", score);
        gfx_text(20, 30, score_text);

        // 绘制提示
        if (!game_started) {
            gfx_text(width/2 - 100, height/2, "Click window to start");
        } else {
            char hint[50];
            sprintf(hint, "Press: %c", obj.letter);
            gfx_text(20, 60, hint);
        }

        // 刷新
        gfx_flush();

        // === 关键：使用新的非阻塞函数 ===

        // 检查鼠标点击
        if (gfx_check_mouse_click()) {
            game_started = 1;
            printf("Game started!\n");
        }

        // 检查按键
        char key = gfx_check_key();
        if (key != 0) {
            printf("Got key: %c\n", key);

            if (key == 'q' || key == 'Q') {
                game_running = 0;
                printf("Final score: %d\n", score);
            }
            else if (game_started && obj.active) {
                // 检查是否按对了字母（不区分大小写）
                if (key == obj.letter || key == obj.letter + 32 ||
                    key == obj.letter - 32) {
                    printf("Hit! Score +10\n");
                    score += 10;
                    obj.active = 0;

                    // 创建新对象
                    init_object(&obj, width);
                } else if ((key >= 'A' && key <= 'Z') ||
                          (key >= 'a' && key <= 'z')) {
                    printf("Wrong key!\n");
                }
            }
        }

        // 更新游戏状态
        if (game_started && obj.active) {
            obj.y += obj.speed;

            if (obj.y > height + obj.size) {
                printf("Missed! Score -5\n");
                score -= 5;
                obj.active = 0;
                init_object(&obj, width);
            }
        }

        // 延迟
        Sleep(30);
    }

    return 0;
}