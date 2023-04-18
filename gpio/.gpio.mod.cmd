cmd_/home/knghia/dev/driver/gpio/gpio.mod := printf '%s\n'   gpio.o | awk '!x[$$0]++ { print("/home/knghia/dev/driver/gpio/"$$0) }' > /home/knghia/dev/driver/gpio/gpio.mod
