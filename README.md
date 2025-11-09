<p align="center">
    <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://github.com/lethanner/ltda-prime/assets/50079644/8ea13b10-f0b7-4201-a2f7-b5a1af4f5483">
        <source media="(prefers-color-scheme: light)" srcset="https://github.com/lethanner/ltda-prime/assets/50079644/ec0cc92e-7ae2-45f1-b50d-efc96040dc95">
        <img alt="Project logo" src="https://github.com/lethanner/ltda-prime/assets/50079644/ec0cc92e-7ae2-45f1-b50d-efc96040dc95">
    </picture><br>
</p>

<h3 align="center"><a href="README-ru.md">Прочитать на русском</a></h3>
<h1>Lethanner Tech Diversity Audio</h1>
<p>An LTDA Prime concept is a great upgrade of my <a href="https://github.com/lethanner/da50x">DA50X</a> project but with completely digital audio processing and 10x greater output power than previous.
It can operate both as an audio amplifier and a mixing console, do basic audio processing and apply some audio effects to the sound.</p>


> [!WARNING]
> Hobby-level usage only! Consider purchasing certified professional audio equipment if you want to work responsibly with audio.<br>
> According to <a href="https://github.com/lethanner/ltda-prime/blob/fw-dev/LICENSE">GNU GPL license</a>, this software comes WITHOUT ANY WARRANTIES and the developer is not liable for any possible damage after using this product.


<h2>Development</h2>
<p>
    Release 1.0.0 is still under construction.<br>
</p>

<h2>Specifications</h2>
<ul>
    <li><b>Peak output power:</b> 2 * 325W (10% THD)</li>
    <li><b>Digital stereo inputs:</b> USB, Bluetooth A2DP, S/PDIF</li>
    <li><b>Analog mono inputs (AUX):</b> 2x XLR+Jack 6.3 (PCM1808 ADC, 24 bit)</li>
    <li><b>Additional AUX:</b> 4x Jack 6.3 (internal SigmaDSP ADC, 10 bit) </li>
    <li><b>AUX pre-amplifier:</b> 0, +6, +12, +20, +30, +40, +50, +60 dB</li>
    <li><b>Stereo output:</b> independent Master, Submix, S/PDIF</li>
    <li><b>Coding:</b> 48 kHz 24 bit</li>
</ul>
<h3>Features</h3>
<ul>
    <li><b>Remote control using <a href="https://github.com/lethanner/ltda-console">LTDA Console</a> (through Wi-Fi)</b></li>
    <li>Independent sends control to Master, Submix and S/PDIF outputs</li>
    <li>LED level indicator driven by software</li>
    <li>Signal level monitoring for every single channel</li>
    <li>Volume control for every single channel</li>
    <li>Reverb and Pitch FX sends</li>
    <li>Stereo balance control for every single channel</li>
    <li>Bassboost feature on Master channel</li>
    <li>Forced mono and Vocal suppression modes for stereo channels</li>
    <li>Bluetooth device volume synced with DSP volume</li>
    <li><a href="https://en.wikipedia.org/wiki/Bi-amping_and_tri-amping">Bi-Amping</a> feature
        <ol>
            <li><b>Mono Bi-Amping:</b> LF routed to left, HF routed to right (Submix are not affected)</li>
            <li><b>Stereo Bi-Amping:</b> LF routed to Master, HF routed forcibly from Master to Submix (needs an external amplifier)</li>
        </ol>
    </li>
</ul>
<p><b>WARNING!</b> This list is not final.</p>
<h3>Hardware</h3>
<ul>
    <li><b>MCU:</b> ESP-WROOM-32</li>
    <li><b>DSP:</b> SigmaDSP ADAU1452</li>
    <li><b>Amplifier:</b> TPA3255 (Class D)</li>
    <li><b>DAC:</b> 2x PCM5102</li>
    <li><b>Display:</b> SSD1306</li>
    <li><b>USB-I2S/UART controller:</b> based on STM32F401CCU6</li>
    <li><b>Level indicators:</b> 3x 74HC595, 24 LEDs</li>
</ul>

<h2></h2>
<p align="center">
    <img alt="OLED display content" width=70% src="https://github.com/user-attachments/assets/a1ef3c86-bd66-4be4-be83-0b3e4d365d2a">
    <img alt="LED indicator of an LTDA Prime" width=70% src="https://github.com/user-attachments/assets/3ef2f27e-fa10-4131-a50e-683b00dadbdb">
</p>

<h2>Credits</h2>
<p>Without them the project would not exist.</p>
<h4>Physical support</h4>
<ul>
    <li><a href="https://vk.com/ssau_kontur">Electronics amateur club "Kontur"</a> and it's members</li>
    <li>AI assistants (e.g. ChatGPT) - <i>helped me fixing errors and learn Qt basics very quickly</i></li>
</ul>

<h4>Inspiration</h4>
<ul>
    <li><a href="https://vk.com/prisma_ssau">Theatrical and experimental studio «Prisma: refracting reality»</a></li>
    <li><a href="https://chipdip.ru">"Chip i Dip" electronics shop</a></li>
    <li>My childhood dreams</li>
</ul>