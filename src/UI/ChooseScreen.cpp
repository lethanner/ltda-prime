#include "UI.h"

bool LEDUI::ChooseScreen::confirmation = false;
void *LEDUI::ChooseScreen::_params = NULL;

bool LEDUI::ChooseScreen::init(void *params)
{
    title_xCoord = getCenterCoordinate(_title);
    confirmation = (_neg == NULL);
    _params = params;

    return true;
}

void LEDUI::ChooseScreen::onHold() {}

/* onClick() реализуется только при наследовании от класса ChooseScreen */

void LEDUI::ChooseScreen::render()
{
    display.clear();
    // заголовок
    display.invertText(1);
    display.rect(0, 0, 127, 7, OLED_FILL);
    display.setCursor(title_xCoord, 0);
    display.print(_title);
    display.invertText(0);

    // текст
    display.setCursor(0, 1);
    display.print(_text);

    // варианты
    if (_neg != NULL) {
        display.setCursor(0, 6);
        display.invertText(!confirmation);
        display.print("> ");
        display.print(_neg);
    }

    display.setCursor(0, 7);
    display.invertText(confirmation);
    display.print("> ");
    display.print(_pos);
}

void LEDUI::ChooseScreen::onTurn(int8_t dir) {
    confirmation = (dir > 0) || (_neg == NULL);
}