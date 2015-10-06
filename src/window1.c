#include <pebble.h>
#include "window1.h"
//#define TO_RECIEVE
#define PD_DATA 0
//#define KEY_CONDITIONS 1

static int battPerc = 100;
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_background_2;
static GFont s_res_squares_bold_12;
static GFont s_res_squares_bold_35;
static GFont s_res_squares_bold_15;
static BitmapLayer *s_bitmaplayer_1;
static TextLayer *time_layer;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static Layer *battery_Layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_background_2 = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_2);
  s_res_squares_bold_12 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SQUARES_BOLD_12));
  s_res_squares_bold_35 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SQUARES_BOLD_35));
  s_res_squares_bold_15 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SQUARES_BOLD_15));
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_background_2);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // time_layer
  time_layer = text_layer_create(GRect(22, 4, 103, 39));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_squares_bold_35);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(22, 45, 100, 19));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_squares_bold_12);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(10, 114, 126, 53));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text_color(s_textlayer_2, GColorWhite);
  text_layer_set_text(s_textlayer_2, "Loading...");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_2, s_res_squares_bold_15);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // battery_Layer
  battery_Layer = layer_create(GRect(0, 0, 144, 168));
  layer_add_child(window_get_root_layer(s_window), (Layer *)battery_Layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_1);
  text_layer_destroy(time_layer);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  layer_destroy(battery_Layer);
  gbitmap_destroy(s_res_background_2);
  fonts_unload_custom_font(s_res_squares_bold_12);
  fonts_unload_custom_font(s_res_squares_bold_35);
  fonts_unload_custom_font(s_res_squares_bold_15);
}
// END AUTO-GENERATED UI CODE

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) { 
  static char s_time_buffer[16];
  static char s_date_buffer[16];
  if (clock_is_24h_style()) {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%H:%M", tick_time);
  } else {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%l:%M", tick_time);
  }
  if (!clock_is_24h_style() && (s_time_buffer[0] == ' ')) {
    memmove(s_time_buffer, &s_time_buffer[1], sizeof(s_time_buffer) - 1);
  }
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %d", tick_time);
  text_layer_set_text(time_layer, s_time_buffer);
  text_layer_set_text(s_textlayer_1, s_date_buffer);
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 5 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}

static void battery_handler(BatteryChargeState new_state) {
  // Write to buffer and display
  battPerc = new_state.charge_percent;
  layer_mark_dirty(battery_Layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  //static char temperature_buffer[8];
  //static char conditions_buffer[32];
  static char pd_schedule_buffer[32];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    /*case KEY_TEMPERATURE:
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
      break;*/
    case PD_DATA:
      snprintf(pd_schedule_buffer, sizeof(pd_schedule_buffer), "%s", t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // Assemble full string and display
  //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  //snprintf(pd_schedule_buffer, sizeof(pd_schedule_buffer), "%s", )
  text_layer_set_text(s_textlayer_2, pd_schedule_buffer);
  /*GPoint p0 = GPoint(0, 0);
  GPoint p1 = GPoint(100, 100);                          <<<<<<<<<<WORK ON THIS!!!!!!!!!!
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p0, p1);*/
  
  
  //GPoint p0 = GPoint(0, 0);
  //GPoint p1 = GPoint(100, 100); 
  //graphics_context_set_stroke_color( ,GColorBlack);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success");
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void draw_line(Layer *this_layer, GContext *ctx) {
  // Draw things here using ctx
  GPoint p0 = GPoint(22+((100-battPerc)/2), 111);
  GPoint p1 = GPoint(122-((100-battPerc)/2), 111);
  GPoint p2 = GPoint(22+((100-battPerc)/2)+1, 110);
  GPoint p3 = GPoint(122-((100-battPerc)/2)-1, 110);
  GPoint p4 = GPoint(22+((100-battPerc)/2)+1, 112);
  GPoint p5 = GPoint(122-((100-battPerc)/2)-1, 112);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_line(ctx, p0, p1);
  graphics_draw_line(ctx, p2, p3);
  graphics_draw_line(ctx, p4, p5);
  APP_LOG(APP_LOG_LEVEL_INFO, "Line drawn");
  /*static char buf[8];
  snprintf(buf, sizeof(buf), "%d", battPerc);
  APP_LOG(APP_LOG_LEVEL_INFO, buf);*/
}

void show_window1(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
  BatteryChargeState charge_state = battery_state_service_peek();
  battPerc = charge_state.charge_percent;
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  layer_set_update_proc(battery_Layer, draw_line);
}

void hide_window1(void) {
  window_stack_remove(s_window, true);
}
