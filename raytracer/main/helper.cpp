#include "helper.h"
#include "raytracer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QtMath>
#include <qmath.h>
#include <QTimer>


Helper::Helper(QObject *parent) : QObject(parent)
{
    effect = new raytracer();
}

Helper::~Helper() {
    delete effect;
}

void Helper::tick()
{
    effect->update();
}

void Helper::paint(QPainter *painter)
{
    painter->drawImage(0,0, effect->framebuffer);
}