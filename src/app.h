

char* categories[8]={"Food","Drinks","Coffee","Shops","Arts","Outdoors","Sights","Specials"};
char* codes[8]={"food","drinks","coffee","shops","arts","outdoors","trending","specials"};

char *names[10];
char *ratings[10];
char *detail[10];
char *lats[10];
char *lngs[10];

static Window *c_window; // Category Window
static Window *r_window; //Results Window
static Window *m_window; //Message Window
static Window *d_window; // Detail Window

static SimpleMenuLayer *c_menu_layer; //Category Menu
static SimpleMenuLayer *r_menu_layer; //Result Menu
static TextLayer *text_layer; //Text Layer for searching window
static ScrollLayer *d_scroll_layer; //Scroll Layer for detail window
static TextLayer *d_text_layer; //Text Layer for detail window



static SimpleMenuItem c_items[10]; //Holds category items
static SimpleMenuSection c_sections[1]; //Holds category sections
static SimpleMenuItem r_items[10]; //Holds result items
static SimpleMenuSection r_sections[1]; //Holds results sections

ScrollLayerCallbacks cbacks;

ActionBarLayer *action_bar;


static const int RESOURCE_IDS[4] = {
  RESOURCE_ID_IMAGE_BUTTON_UP,
  RESOURCE_ID_IMAGE_BUTTON_SELECT,
  RESOURCE_ID_IMAGE_BUTTON_DOWN,
  RESOURCE_ID_IMAGE_BUTTON_CAR
};
static const int menu_icons[8] = {
  RESOURCE_ID_FOOD,
  RESOURCE_ID_DRINKS,
  RESOURCE_ID_COFFEE,
  RESOURCE_ID_SHOPS,
  RESOURCE_ID_ARTS,
  RESOURCE_ID_OUTDOORS,
  RESOURCE_ID_SIGHTS,
  RESOURCE_ID_SPECIALS,
};


  GBitmap *menu_icons_bitmaps[8];

  

char *msg; // Holds message to be displayed in message window

static GRect bounds;

int g_index; //Last select category index
const bool animated = true; 




static void menu_select_callback(int index, void *ctx) ;
static void result_select_callback(int index, void *ctx) ;
