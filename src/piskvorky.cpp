#include <cstdio>
#include <memory>

#include "resources.h" // packed /res/ folder
#include "Frame.h"
#include "../test/TestGame.h"


int uimain(std::function<int()> run) {
    testEntryPoint();

    sciter::archive::instance().open(aux::elements_of(resources)); // bind resources[] (defined in "resources.cpp") with the archive

    Frame *pwin = new Frame();

    // note: this:://app URL schema is dedicated to the sciter::archive content associated with the application
    pwin->load( WSTR("this://app/default.htm") );
    pwin->expand();

    return run();
}
