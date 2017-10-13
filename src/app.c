#include <pebble.h>
#include <app.h>
#include <stdlib.h>


/**
 * Used to initialize the category menu items & sections
 */
 void createMenu(){



   for(int i=0;i<8;i++){

    c_items[i]=(SimpleMenuItem){
      .title=categories[i],
      .callback=menu_select_callback,
      .icon=menu_icons_bitmaps[i]
      

    };
  }

  c_sections[0] = (SimpleMenuSection){
    .title="Explore",
    .num_items = 8,
    .items = c_items
  };



}


/**
 * Used to initialize the Results menu items
 * @param count 
 */
 void createResultsMenu(int count){
  if(count<=0){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] No results found"); 


    r_items[0]=(SimpleMenuItem){
      .title="No results found",
      .subtitle=":("
    };
    count++;


  }
  else{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] %d results found",count); 

    for(int i=0;i<count;i++){
      r_items[i]=(SimpleMenuItem){
        .title=names[i],
        .subtitle=ratings[i],
        .callback=result_select_callback
        

      };
    }
  }

  r_sections[0] = (SimpleMenuSection){
    .title=categories[g_index],
    .num_items = count,
    .items = r_items
  };

}


//AppMessage handlers

/**
 * Called when message is sent to PebbleKitJS
 * @param sent
 * @param context
 */
 void out_sent_handler(DictionaryIterator *sent, void *context) { 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message delivered");
}


/**
 * Called when message is not sent to PebbleKitJS
 * @param failed
 * @param reason
 * @param context
 */
 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) { 

  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message delivery failed"); 
}

/**
 * Called when message is received from PebbleKitJS
 * @param received 
 * @param context
 */
 void in_received_handler(DictionaryIterator *received, void *context) {  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message received"); 


  
  
  int count = 0;
  Tuple *tuple = dict_read_first(received);
  while(tuple){
    count ++;
    tuple = dict_read_next(received);
  }

  count=count-1;

  
  
  for (int i=0;i<count;i++) {
    Tuple *place = dict_find(received, i); 


    if (place) {
      char* name=place->value->cstring;

          //Formatting
      char* rating = name;
      while(*rating!= '|') {
        rating++;
      }


      *rating++ = '\0';


      char *body =rating;
      while(*body!= '|') {
        body++;
      }
      *body++ = '\0';


      char *lat=body;

      while(*lat!= '|') {
        lat++;
      }
      *lat++ = '\0';

      char* lng=lat;

      while(*lng!= '|') {
        lng++;
      }
      *lng++ = '\0';




  //End of Formatting
      


      
      


      
      names[i]=name;
      ratings[i]=rating;
      detail[i]=body;
      lats[i]=lat;
      lngs[i]=lng;
      
          //APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] From JAVASCRIPT - venue: \n name %s\n rating%s\n detail %s\n lat %s\n lng%s", names[i],ratings[i],detail[i],lats[i],lngs[i]);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] From JAVASCRIPT - venue:\n lat %s\n lng%s",lats[i],lngs[i]);
    }
  }
  

  createResultsMenu(count);

  
  


  bounds = layer_get_frame(window_get_root_layer(r_window));

  r_menu_layer = simple_menu_layer_create(bounds,r_window, r_sections, 1, NULL);


  layer_add_child(window_get_root_layer(r_window), simple_menu_layer_get_layer(r_menu_layer));
      window_stack_pop(animated);  // pop out searching window
      window_stack_push(r_window, animated);  // pusg results window


    }

    


/**
 * Called when message from PebbleKitJS is dropped due to some reason
 * @param reason
 * @param context
 */
 void in_dropped_handler(AppMessageResult reason, void *context) {  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message dropped"+reason); 


}


/**
 * Called when a category menu item is selected 
 * @param index
 * @param ctx
 */
 static void menu_select_callback(int index, void *ctx) { 
  msg="Searching.. ";


  window_stack_push(m_window, animated); 
  

  g_index=index;

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Selected Index%d",index); 
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  Tuplet value1= TupletCString(100, codes[index]);
  Tuplet value2= TupletCString(300, "PebbleKitJS");
  dict_write_tuplet(iter, &value1);
  dict_write_tuplet(iter, &value2);

  app_message_outbox_send();
}

/**
 * Called when a result item is selected
 * @param index
 * @param ctx
 */
static void result_select_callback(int index, void *ctx) { //Called when a caregory menu item is selected 
  g_index=index;
  

  window_stack_push(d_window, animated); 
  text_layer_set_text(d_text_layer, detail[index]);
  GSize max_size = text_layer_get_content_size(d_text_layer);


  scroll_layer_set_content_size (d_scroll_layer, max_size );


}


//Window  load & unload Handlers


 /**
  * Called when category window is loaded
  * @param window
  */
  static void c_window_load(Window *window) {
    createMenu();

    
    bounds = layer_get_frame(window_get_root_layer(c_window));
    c_menu_layer = simple_menu_layer_create(bounds,c_window, c_sections, 1, NULL);
    layer_add_child(window_get_root_layer(c_window), simple_menu_layer_get_layer(c_menu_layer));


  }

/**
 * Called when category window is unloaded
 * @param window
 */
 static void c_window_unload(Window *window) {

  layer_destroy((Layer *)c_menu_layer);
  
}

/**
 * Called when result window is unloaded
 * @param window
 */
 static void r_window_unload(Window *window) {
  layer_destroy((Layer *)r_menu_layer);
}

/**
 * Called when searching window is loaded
 * @param window
 */
 static void m_window_load(Window *window) {





  bounds = layer_get_frame(window_get_root_layer(m_window));

  //Searching Layer
  text_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 2000 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_text(text_layer, msg);


  layer_add_child(window_get_root_layer(m_window), text_layer_get_layer(text_layer)); 

}

static void d_select_click_handler(ClickRecognizerRef recognizer, void *context) {

  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] d_select_click_handler"); 
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  Tuplet value1= TupletCString(200, lats[g_index]);
  Tuplet value2= TupletCString(201, lngs[g_index]);
  Tuplet value3= TupletCString(300, "PebbleKitAndroid");
  dict_write_tuplet(iter, &value1);
  dict_write_tuplet(iter, &value2);
  dict_write_tuplet(iter, &value3);

  app_message_outbox_send();
  
}

void config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, d_select_click_handler);
}




/**
 * Called when detail window is loaded
 * @param d_window
 */
 static void d_window_load(Window *d_window) {
// Init Resource

  GBitmap *up=gbitmap_create_with_resource(RESOURCE_IDS[0]);
  GBitmap *select=gbitmap_create_with_resource(RESOURCE_IDS[3]);
  GBitmap *down=gbitmap_create_with_resource(RESOURCE_IDS[2]);



    // Action Bar

  action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(action_bar, d_window);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, up);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, select);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, down);



  


  Layer* d_layer;
  d_layer=window_get_root_layer(d_window);
  bounds = layer_get_frame(d_layer);

  //Detail Layer
  d_scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(d_scroll_layer, d_window);
  cbacks.click_config_provider = &config_provider;

  scroll_layer_set_callbacks(d_scroll_layer, cbacks);





  d_text_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w-20, 2000} });

  text_layer_set_font(d_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));


  scroll_layer_add_child(d_scroll_layer, text_layer_get_layer(d_text_layer));



  layer_add_child(d_layer, scroll_layer_get_layer(d_scroll_layer)); 


  


}

/**
 * Called when detail window is unloaded
 * @param window
 */
 static void d_window_unload(Window *window) {
  layer_destroy((Layer *)d_scroll_layer);
  layer_destroy((Layer *)d_text_layer);
}



void bluetooth_connection_callback(bool connected) {
 APP_LOG(APP_LOG_LEVEL_INFO, "bluetooth connected=%d", (int) connected);
 if(!connected){
  msg="Bluetooth connection \n lost";
  window_stack_pop_all(animated);
  window_stack_push(m_window, animated); 


}


}


/**
 * Initializer function to be called when WatchApp is started
 */
 static void init(void) {

  for(int i=0;i<8;i++)
  menu_icons_bitmaps[i]=gbitmap_create_with_resource(menu_icons[i]);




  m_window = window_create(); 

  window_set_window_handlers(m_window, (WindowHandlers) {

   .load = m_window_load, 
 });



  if (bluetooth_connection_service_peek()) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is connected!");


    bluetooth_connection_service_subscribe(bluetooth_connection_callback);

    
    c_window = window_create(); 
    r_window = window_create(); 
    d_window = window_create(); 
    window_set_window_handlers(c_window, (WindowHandlers) {
      .load = c_window_load,
      .unload = c_window_unload, 

    });

    window_set_window_handlers(r_window, (WindowHandlers) {

     .unload = r_window_unload, 
   });

    
    window_set_window_handlers(d_window, (WindowHandlers) {

     .load = d_window_load, 
     .unload=d_window_unload
   });

    

    

  //Registering handlers
    app_message_register_inbox_received(in_received_handler); 
    app_message_register_inbox_dropped(in_dropped_handler);
    app_message_register_outbox_sent(out_sent_handler); 
    app_message_register_outbox_failed(out_failed_handler);

    const uint32_t inbound_size = app_message_inbox_size_maximum(); 
    const uint32_t outbound_size = app_message_outbox_size_maximum();

  //open channel
    app_message_open(inbound_size, outbound_size);

  //create category menu

    window_stack_push(c_window, animated); 

  } else {
    msg="Connect your phone to pebble";
    window_stack_push(m_window, animated); 

  }
  
}



/**
 * Deinitializer function to be called when WatchApp is closed
 */
 static void deinit(void) {
  window_destroy(c_window);
  window_destroy(r_window);
  window_destroy(m_window);
  window_destroy(d_window);
}

/**
 * Main function
 * @return
 */
 int main(void) {
  init();
  app_event_loop();
  deinit();
}


