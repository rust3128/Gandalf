#include "buttonvnc.h"

ButtonVNC::ButtonVNC(int connID)
{
    buttonID = connID;
}

int ButtonVNC::getButtonID()
{
    return buttonID;
}
