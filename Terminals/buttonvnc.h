#ifndef BUTTONVNC_H
#define BUTTONVNC_H

#include <QPushButton>
#include <QCommandLinkButton>

class ButtonVNC : public QCommandLinkButton
{
public:
    ButtonVNC(int connID);
    int getButtonID();
private:
    int buttonID;
};

#endif // BUTTONVNC_H
