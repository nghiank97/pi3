cmd_/home/knghia/dev/driver/uart/uart.mod := printf '%s\n'   uart.o | awk '!x[$$0]++ { print("/home/knghia/dev/driver/uart/"$$0) }' > /home/knghia/dev/driver/uart/uart.mod
