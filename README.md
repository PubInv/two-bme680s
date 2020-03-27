# two-bme680s
A short Arduino Sketech demonstrating reading from two Adafuit BME680s at the same time with I2C

Note there is a trick that is not too well publicized: Tie SD0 to GND, and the I2C as is 0x76, other wise 0x77. Alas, no obvious way to handle more than two!
