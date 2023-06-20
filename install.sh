echo "/dts-v1/;\n/plugin/;\n/ {\n    compatible = \"brcm,bcm2708\";\n\n    fragment@0 {\n        target = <&spidev0>;\n\n        __overlay__ {\n            status = \"disabled\";\n        };\n    };\n};" > spidev_disabler.dts
dtc spidev_disabler.dts -O dtb >spidev_disabler.dtbo
sudo dtoverlay -d . spidev_disabler

sudo insmod ./mcp3208dev/mcp3208_spi_dev.ko
sudo insmod ./light/simple_motor.ko
sudo insmod ./gas/gas_mod.ko
sudo insmod ./alert/alert_mod.ko
sudo insmod ./fan_con/fan_con.ko
sudo insmod ./door/simple_door.ko
sudo sh ./mcp3208dev/mknod.sh
sudo sh ./gas/mknod.sh
sudo sh ./alert/mknod.sh
sudo sh ./light/mknod_motor.sh
sudo sh ./fan_con/mknod_fan_con.sh
sudo sh ./door/mknod_door.sh
sudo chmod 755 ./gas/gas_mod_app
sudo chmod 755 ./light/simple_light_app
sudo chmod 755 ./fan_con/fan_con_app
sudo chmod 755 ./door/simple_door_app
sudo ./gas/gas_mod_app &
sudo ./light/simple_light_app &
sudo ./fan_con/fan_con_app &
sudo ./door/simple_door_app &
