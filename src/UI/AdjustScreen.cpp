#include "UI.h"

void LEDUI::AdjustScreen::init(void* params)
{
    title_xCoord = getCenterCoordinate(_title);
}

void LEDUI::AdjustScreen::render()
{
    display.clear();
    // заголовок
    display.invertText(1);
    display.rect(0, 0, 127, 7, OLED_FILL);
    display.setCursor(title_xCoord, 0);
    display.print(_title);
    // единица измерения и значение
    display.invertText(0);
    printValue(*_value, _unit, 0, 24, true);
    // полосочка
    // TODO: сделать так, чтобы эта дичь могла рисовать эту полоску и для отрицательных значений
    byte x_end = map(*_value, __min, __max, 10, 116);
    display.rect(10, 44, x_end, 49, OLED_FILL);
}

void LEDUI::AdjustScreen::onHold()
{
    // возврат в меню
    open(MenuScreen::active);
}

void LEDUI::AdjustScreen::onClick()
{
    // to be continued... возможно
}

/* onTurn() реализуется только при наследовании от класса AdjustScreen */