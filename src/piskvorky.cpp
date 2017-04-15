#include <cstdio>
#include <sciter-x-window.hpp>


static RECT wrc = { 100, 100, 800, 800 };

class frame: public sciter::window {
public:
  frame() : window( SW_MAIN | SW_TITLEBAR | SW_CONTROLS | SW_ENABLE_DEBUG, wrc) {}

  BEGIN_FUNCTION_MAP
      FUNCTION_2("ui_click", ui_click);

  END_FUNCTION_MAP

    sciter::value ui_click(sciter::value cur_x, sciter::value cur_y) {
        int x = cur_x.get<int>();
        int y = cur_y.get<int>();

        printf("Clicked at %d, %d\n", x, y);
        return sciter::value();
    }
//
//  int architecture() {
//    // this function is here just for the demo purposes,
//    // it shows native function callable from script as view.architecture();
//#if defined(TARGET_32)
//    return 32;
//#elif defined(TARGET_64)
//    return 64;
//#endif
//  }
};

#include "resources.h" // packed /res/ folder

int uimain(std::function<int()> run ) {

  sciter::archive::instance().open(aux::elements_of(resources)); // bind resources[] (defined in "resources.cpp") with the archive

  frame *pwin = new frame();

  // note: this:://app URL schema is dedicated to the sciter::archive content associated with the application
  pwin->load( WSTR("this://app/default.htm") );

  pwin->expand();

  return run();
}
