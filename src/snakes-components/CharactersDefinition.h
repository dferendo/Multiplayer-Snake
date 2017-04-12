//
// Created by dylan on 12/04/2017.
//

#ifndef SNAKES_CHARACTERSDEFINITION_H
#define SNAKES_CHARACTERSDEFINITION_H

#define SNAKE_BODY '#'
#define BORDER_CHARACTER '&'

typedef enum {
    F_NORMAL = 0
} Type;

static const chtype stringFromFruit(Type type) {
    const chtype TYPE_STRINGS[] = { 'o' };
    return TYPE_STRINGS[type];
}

#endif //SNAKES_CHARACTERSDEFINITION_H
