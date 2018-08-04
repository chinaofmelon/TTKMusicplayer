#include <qmmp/qmmp.h>
#include "visualflashmeterfactory.h"
#include "flashmeter.h"

const VisualProperties VisualFlashMeterFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flash Meter Plugin");
    properties.shortName = "flashmeter";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualFlashMeterFactory::create(QWidget *parent)
{
    return new FlashMeter(parent);
}
