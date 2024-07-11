source ~/zephyrproject/.venv/bin/activate
west build -p always -b stm32f3_disco Micromouse_Project

west flash

sudo minicom -s