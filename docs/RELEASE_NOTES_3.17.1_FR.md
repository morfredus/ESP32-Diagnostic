# Notes de version — 3.17.1 (2025-12-05)

- Rafraîchissement du pin mapping ESP32-S3 : GPS RX=18/TX=17/PPS=8 ; TFT MOSI=11/SCLK=12/CS=10/DC=9/RST=13/BL=7 ; LED RGB R=47/G=48/B=45 ; capteurs mis à jour (PWM/Buzzer=14, DHT=5, Mouvement=4, Lumière=19, HC-SR04 TRIG=3/ECHO=6) ; boutons inchangés (BTN1=1, BTN2=2).
- Mise à jour du pin mapping ESP32 Classic : GPS RX=16/TX=17/PPS=4 ; TFT MOSI=23/SCLK=18/CS=19/DC=27/RST=26/BL=13 ; LED RGB R=12/G=14/B=15 ; capteurs mis à jour (PWM/Buzzer=5, DHT=25, Mouvement=36, Lumière=2, HC-SR04 TRIG=32/ECHO=33) ; boutons BTN1=34/BTN2=35.
- Documentation alignée (README EN/FR, guides de mapping, matrices de fonctionnalités, guides d'utilisation et de build) et `PROJECT_VERSION` porté à `3.17.1` dans `platformio.ini`.
- Type de release : Patch (3.17.0 → 3.17.1) ; pas de changement fonctionnel en dehors des nouveaux défauts de broches.
