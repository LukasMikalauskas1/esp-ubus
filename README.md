Second task. UBUS module for controlling ESP micro controller

For this task you will create a program which will let you control connected ESP micro controllers to the router over ubus.

Ask your internship manager to give you ESP micro controller. ESP controller will already have a
special firmware uploaded onto it which will let you control ESP pins over the serial. Documentation on how to control the micro controller can be found here.

Ubus should provide these methods:

    devices
    on
    off

Devices method should return an array of devices with this information about devices:

    port
    vendor id
    product id

More than one device can be connected at a time.

On method should be used to turn on the pin and accept only these arguments:

    port
    pin

Off method should be used to turn off the pin and accept only these arguments:

    port
    pin​​​​​​​

If something went wrong, ubus should return an error message. Your program should automatically start after router reboot. Your program should work without UCI configuration file. All messages should be returned in JSON format using blob messages library.

To accomplish this task, you can use hotplug, some kind of USB or serial library, or maybe you will find some other way to identify ESP micro controllers.