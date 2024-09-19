<p align="center">
    <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://github.com/lethanner/ltda-prime/assets/50079644/8ea13b10-f0b7-4201-a2f7-b5a1af4f5483">
        <source media="(prefers-color-scheme: light)" srcset="https://github.com/lethanner/ltda-prime/assets/50079644/ec0cc92e-7ae2-45f1-b50d-efc96040dc95">
        <img alt="Project logo" src="https://github.com/lethanner/ltda-prime/assets/50079644/ec0cc92e-7ae2-45f1-b50d-efc96040dc95">
    </picture><br>
</p>

<h1>Lethanner Tech Diversity Audio</h1>
<p>Концепт LTDA Prime представляет собой более удачное продолжение проекта <a href="https://github.com/lethanner/da50x">DA50X</a> с полностью цифровой обработкой звука,
а также совмещением функционала усилителя звука и микшерного пульта. LTDA Prime имеет повышенную в 10 раз выходную мощность, увеличенное количество разъёмов для источников
сигнала, а также встроенные средства обработки звука и наложения на него различных эффектов.</p>

<h2>Разработка</h2>
<p>
    Текущая версия - v0.4-beta. Полноценная первая версия находится в стадии разработки.<br>
    Для просмотра полной истории изменений перейдите в ветку <a href="https://github.com/lethanner/ltda-prime/tree/fw-dev">fw-dev.</a>
</p>

<h2>Характеристики</h2>
<ul>
    <li><b>Пиковая мощность:</b> 2 * 325 Вт (10% THD)</li>
    <li><b>Рекомендуемое питание:</b> DC 48 вольт 15 ампер</li>
    <li><b>Цифровые входы:</b> USB, I2S, Bluetooth, S/PDIF</li>
    <li><b>Входы AUX:</b> 6x моно (48 kHz 10 bit АЦП) <i>(временно отсутствуют!)</i></li>
    <li><b>Стереовыходы:</b> независимые Master + Submix</li>
    <li><b>Кодирование:</b> 48 кГц 24 бит</li>
</ul>
<h3>Особенности</h3>
<ul>
    <li>Независимое управление стереовыходами Master и Submix</li>
    <li>Программно контролируемый светодиодный индикатор уровня</li>
    <li>Отображение уровней сигнала каждого канала</li>
    <li>Индивидуальное управление громкостью каждого канала</li>
    <li>Управляемые эффекты реверберации и изменения высоты тона</li>
    <li>Управление стереобалансом каждого канала</li>
    <li>Функция усиления басов (bassboost)</li>
    <li>Синхронизация громкости Bluetooth-устройства с виртуальным микшером</li>
    <li>OLED-дисплей и единственный энкодер для управления</li>
</ul>
<p><b>ВНИМАНИЕ!</b> Список будет пополняться по мере реализации функционала.</p>
<h3>Комплектующие</h3>
<ul>
    <li><b>Микроконтроллер:</b> ESP-WROOM-32</li>
    <li><b>Сигнальный процессор:</b> SigmaDSP ADAU1452</li>
    <li><b>Усилитель:</b> TPA3255 (класс D)</li>
    <li><b>ЦАП:</b> PCM5102 и ES9023</li>
    <li><b>Дисплей:</b> SSD1306</li>
    <li><b>Индикаторы уровня:</b> 24 светодиода, 3x 74HC595</li>
</ul>

<h2></h2>
<p align="center">
    <img alt="OLED display content" width=70% src="https://github.com/user-attachments/assets/a1ef3c86-bd66-4be4-be83-0b3e4d365d2a">
    <img alt="LED indicator of an LTDA Prime" width=70% src="https://github.com/user-attachments/assets/3ef2f27e-fa10-4131-a50e-683b00dadbdb">
</p>

<h2>Благодарности</h2>
<h4>Материальная поддержка</h4>
<a href="https://vk.com/ssau_kontur">Клуб любителей электроники "Контур"</a> и его участники
<h4>Моральная поддержка</h4>
<a href="https://vk.com/prisma_ssau">Театрально-экспериментальная студия "Призма"</a>

