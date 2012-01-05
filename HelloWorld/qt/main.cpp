#include "AppDelegate.h"

int main(int argc, char *argv[])
{
    // create the application instance
    AppDelegate app(argc, argv);
    cocos2d::CCApplication::sharedApplication().setApplicationName("cocos2d HelloWorld");
    return cocos2d::CCApplication::sharedApplication().run();
}
