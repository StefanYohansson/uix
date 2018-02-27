#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo.h>

#include <uix/event_handler.h>
#include <uix/button.h>

void button_change_state(struct button_t* s, xcb_generic_event_t* event) {
  switch (event->response_type & ~0x80) {
    case UIX_MOUSE_PRESS:
      s->state = UIX_BUTTON_CLICK;
      break;
    case UIX_MOUSE_RELEASE:
      s->state = UIX_BUTTON_RELEASE;
    case XCB_KEY_PRESS:
      break;
  }
}

void button_render(cairo_t* context, struct button_t* b) {
  // int16_t pa = b->padding;
  // int16_t pa2 = pa * 2;

  struct argb_t* bg   = &(b->background);
  // struct argb_t* fg   = &(b->foreground);
  struct vec_t*  p    = &(b->frame.position);
  struct size_t* size = &(b->frame.size);

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_rectangle(context,
                  p->x, p->y,
                  size->w, size->h);

  if (b->state == UIX_BUTTON_CLICK) {
    cairo_pattern_t* pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, 256.0);
    cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
    cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
    cairo_set_source(context, pat);
    cairo_pattern_destroy (pat);
    b->state = UIX_BUTTON_NORMAL;
  } else if (b->state == UIX_BUTTON_KEYUP) {
    b->state = UIX_BUTTON_NORMAL;
  }

  cairo_fill(context);

  cairo_save(context);
  cairo_translate(context, p->x, p->y);
  label_render_centered_to(context, &(b->label), &(b->frame));

  cairo_restore(context);
}
