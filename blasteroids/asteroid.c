#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "asteroid.h"
#include "draw_utils.h"
#include "log_utils.h"

void _log_asteroid(char *reason, Asteroid *a) {
    char *text = malloc(sizeof(char)*300);
    sprintf(text, "asteroid %s sx:%f sy:%f heading:%f speed:%f rot_velocity:%f scale:%f gone:%i", reason, a->sx, a->sy, a->heading, a->speed, a->rot_velocity, a->scale, a->gone);
    debug(text);
    free(text);
}

void blasteroids_asteroid_draw(Asteroid *a) {
    if (!a->gone) {
        ALLEGRO_TRANSFORM transform;
        al_identity_transform(&transform);
        al_rotate_transform(&transform, deg2rad(a->heading));
        al_translate_transform(&transform, a->sx, a->sy);
        al_scale_transform(&transform, a->scale, a->scale);
        al_use_transform(&transform);
        al_draw_line(-20, 20, -25, 5, a->color, 2.0f);
        al_draw_line(-25, 5, -25, -10, a->color, 2.0f);
        al_draw_line(-25, -10, -5, -10, a->color, 2.0f);
        al_draw_line(-5, -10, -10, -20, a->color, 2.0f);
        al_draw_line(-10, -20, 5, -20, a->color, 2.0f);
        al_draw_line(5, -20, 20, -10, a->color, 2.0f);
        al_draw_line(20, -10, 20, -5, a->color, 2.0f);
        al_draw_line(20, -5, 0, 0, a->color, 2.0f);
        al_draw_line(0, 0, 20, 10, a->color, 2.0f);
        al_draw_line(20, 10, 10, 20, a->color, 2.0f);
        al_draw_line(10, 20, 0, 15, a->color, 2.0f);
        al_draw_line(0, 15, -20, 20, a->color, 2.0f);
    }
}

void blasteroids_AsteroidNode_draw_all(AsteroidNode *node) {
    AsteroidNode *this = node;
    while(this != NULL) {
        blasteroids_asteroid_draw(this->this);
        this = this->next;
    }
}
void blasteroids_asteroid_update(Asteroid *a) {
    float deltax, deltay;
    _log_asteroid("before", a);
    a->heading = a->heading + a->rot_velocity;
    blasteroids_get_delta(&deltax, &deltay, a->speed, a->heading);
    a->sx = a->sx + deltax;
    a->sy = a->sy + deltay;
    _log_asteroid("after", a);
}

void blasteroids_AsteroidNode_update_all(AsteroidNode *node) {
    AsteroidNode *this = node;
    while(this != NULL) {
        blasteroids_asteroid_update(this->this);
        this = this->next;
    }
}

void blasteroids_destroy_AsteroidNode(AsteroidNode *node) {
    AsteroidNode *last, *this;
    last = node;
    while(last != NULL) {
        this = node->next;
        free(last->this);
        free(last);
        last = this;
    }
}

void blasteroids_AsteroidNode_gc(AsteroidNode *node) {
    AsteroidNode *this, *last;
    last = node;
    this = node->next;
    while (this != NULL) {
        if (this->this->gone) {
            last->next = this->next; // Pula esse nó
            free(this->this); // E libera ele do heap
            free(this);
        }
    }
}
