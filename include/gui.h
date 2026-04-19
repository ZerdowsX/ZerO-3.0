#ifndef GUI_H
#define GUI_H

void gui_init(void);
void gui_draw(void);
void gui_mouse_move(int dx, int dy);
void gui_mouse_click(int left_down);
void gui_keyboard_input(char c);
void gui_set_mouse_ready(int ready);

#endif
