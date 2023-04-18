cmd_/home/knghia/dev/driver/timer/timer.mod := printf '%s\n'   timer.o | awk '!x[$$0]++ { print("/home/knghia/dev/driver/timer/"$$0) }' > /home/knghia/dev/driver/timer/timer.mod
