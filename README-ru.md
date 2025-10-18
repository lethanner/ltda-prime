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


> [!WARNING]
> ТОЛЬКО для любительского использования. Для ответственной работы со звуком следует приобрести сертифицированное профессиональное оборудование.<br>
> В соответствии с текстом <a href="https://github.com/lethanner/ltda-prime/blob/fw-dev/LICENSE">лицензии Свободного программного обеспечения GNU GPL</a>, разработчик не предоставляет никаких гарантий и не несёт никакой ответственности за возможный ущерб, причинённый в результате использования данного продукта.


<h2>Разработка</h2>
<p>
    Релиз 1.0.0 находится в стадии разработки.<br>
    Полный список изменений отражён в ветке <a href="https://github.com/lethanner/ltda-prime/tree/fw-dev">fw-dev.</a>
</p>

<h2>Характеристики</h2>
<ul>
    <li><b>Пиковая мощность:</b> 2 * 325 Вт (10% THD)</li>
    <li><b>Цифровые входы:</b> USB, Bluetooth A2DP, S/PDIF</li>
    <li><b>Входы AUX:</b> 2x XLR+Jack 6.3 (АЦП PCM1808, 24 бит)</li>
    <li><b>Доп. AUX:</b> 4x Jack 6.3 (встроенный в SigmaDSP АЦП, 10 бит) </li>
    <li><b>Предусилитель AUX:</b> 0, +6, +12, +20, +30, +40, +50, +60 дБ</li>
    <li><b>Стереовыходы:</b> независимые Master, Submix и S/PDIF</li>
    <li><b>Кодирование:</b> 48 кГц 24 бит</li>
</ul>
<h3>Функциональность</h3>
<ul>
    <li><b>Удалённое управление в программе <a href="https://github.com/lethanner/ltda-console">LTDA Console</a> (через Wi-Fi)</b></li>
    <li>Независимое управление посылами на Master, Submix и S/PDIF</li>
    <li>Программно управляемый светодиодный индикатор уровня</li>
    <li>Отображение уровней сигнала каждого канала</li>
    <li>Индивидуальное управление громкостью каждого канала</li>
    <li>Посылы эффектов реверберации и изменения высоты тона</li>
    <li>Управление стереобалансом каждого канала</li>
    <li>Функция усиления басов (bassboost)</li>
    <li>Режимы "принудительное моно" или "подавление голоса" для стереоканалов</li>
    <li>Синхронизация громкости Bluetooth-устройства с виртуальным микшером</li>
</ul>
<p><b>ВНИМАНИЕ!</b> Список будет пополняться по мере реализации функционала.</p>
<h3>Комплектующие</h3>
<ul>
    <li><b>Микроконтроллер:</b> ESP-WROOM-32</li>
    <li><b>Сигнальный процессор:</b> SigmaDSP ADAU1452</li>
    <li><b>Усилитель:</b> TPA3255 (класс D)</li>
    <li><b>ЦАП:</b> 2x PCM5102</li>
    <li><b>Дисплей:</b> SSD1306</li>
    <li><b>Контроллер USB-I2S/UART:</b> на основе STM32F401CCU6<li>
    <li><b>Индикаторы уровня:</b> 24 светодиода, 3x 74HC595</li>
</ul>

<h2></h2>
<p align="center">
    <img alt="OLED display content" width=70% src="https://github.com/user-attachments/assets/a1ef3c86-bd66-4be4-be83-0b3e4d365d2a">
    <img alt="LED indicator of an LTDA Prime" width=70% src="https://github.com/user-attachments/assets/3ef2f27e-fa10-4131-a50e-683b00dadbdb">
</p>

<h2>Благодарности</h2>
<p>Без них проект бы не существовал.</p>
<h4>Материальная поддержка</h4>
<ul>
    <li><a href="https://vk.com/ssau_kontur">Клуб любителей электроники "Контур"</a> и его участники</li>
    <li>ИИ-ассистенты (ChatGPT и т.д.) - <i>помогали исправлять ошибки и очень быстро осваивать азы Qt</i></li>
</ul>

<h4>Вдохновение</h4>
<ul>
    <li><a href="https://vk.com/prisma_ssau">Театрально-экспериментальная студия "Призма: преломляя реальность"</a></li>
    <li><a href="https://chipdip.ru">Магазин электроники "Чип и Дип"</a></li>
    <li>Мои детские фантазии</li>
</ul>