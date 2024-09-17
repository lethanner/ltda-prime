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
    if (__min < 0) { // для значений, которые могут быть отрицательными
        if (*_value > 0) { // если текущее значение больше нуля, рисуем полоску вправо
            byte x_end = map(*_value, 0, __max, 63, 116);
            display.rect(63, 44, x_end, 49, OLED_FILL);
        } else { // иначе рисуем полоску влево
            byte x_start = map(*_value, __min, 0, 10, 63);
            display.rect(x_start, 44, 63, 49, OLED_FILL);
        }
    } else { // если значение не может быть отрицательным, делаем всё как обычно
        byte x_end = map(*_value, __min, __max, 10, 116);
        display.rect(10, 44, x_end, 49, OLED_FILL);
    }
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