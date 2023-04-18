cmd_/home/knghia/dev/driver/sysfs/sysfs.mod := printf '%s\n'   sysfs.o | awk '!x[$$0]++ { print("/home/knghia/dev/driver/sysfs/"$$0) }' > /home/knghia/dev/driver/sysfs/sysfs.mod
