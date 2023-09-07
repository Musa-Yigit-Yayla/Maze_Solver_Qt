#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//main container which will act like a scene
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
