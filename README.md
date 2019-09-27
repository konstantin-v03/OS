# Simple operating system for x86 architecture

### Getting started
How to run operating system on qemu?
1. Install qemu
2. Make the project using make command
3. Run qemu with following command

```
    qemu-system-i386 -fda os-image.bin
```

If you want run on real hardware?
1. Make sure that at least 128 MB of RAM is installed on your computer
2. Make the project using make command
3. Connect the astorage device to your computer
4. Load the operating system to your storage device using the following command

```
    sudo dd if=os-image.bin of=/dev/sdb bs=30000 count=1
```

If it doen't work try to check all connected devices and replace /dev/sdb with your device

```
      sudo fdisk -l
```

5.Reboot your computer and select the storage device with os in boot menu
