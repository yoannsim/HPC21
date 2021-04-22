# HPC labo3

yoann simonet



## Matérielle utilisé

### HW:

	cpu : i7-7500U 2.70GHz

	disk: SSD WDC green 256GB

	RAM : 16GB DDR3 

 

### VM:



	                         system     VirtualBox

	/0                        bus        VirtualBox

	/0/0                      memory     128KiB BIOS

	/0/1                      memory     8GiB System memory

	/0/2                      processor  Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz

	/0/100                    bridge     440FX - 82441FX PMC [Natoma]

	/0/100/1                  bridge     82371SB PIIX3 ISA [Natoma/Triton II]

	/0/100/1.1                storage    82371AB/EB/MB PIIX4 IDE

	/0/100/2                  display    SVGA II Adapter

	/0/100/3      enp0s3      network    82540EM Gigabit Ethernet Controller

	/0/100/4                  generic    VirtualBox Guest Service

	/0/100/7                  bridge     82371AB/EB/MB PIIX4 ACPI

	/0/100/c                  bus        7 Series/C210 Series Chipset Family USB xHC

	/0/100/c/0    usb1        bus        xHCI Host Controller

	/0/100/c/0/1              input      USB Tablet

	/0/100/c/1    usb2        bus        xHCI Host Controller

	/0/100/d                  storage    82801HM/HEM (ICH8M/ICH8M-E) SATA Controller

	/0/3                      input      PnP device PNP0303

	/0/4                      input      PnP device PNP0f03

	/0/5          scsi0       storage    

	/0/5/0.0.0    /dev/cdrom  disk       CD-ROM

	/0/5/0.1.0    /dev/sr1    disk       CD-ROM

	/0/5/0.1.0/0  /dev/sr1    disk       

	/0/6          scsi2       storage    

	/0/6/0.0.0    /dev/sda    disk       53GB VBOX HARDDISK

	/0/6/0.0.0/1  /dev/sda1   volume     47GiB EXT4 volume

	/0/6/0.0.0/2  /dev/sda2   volume     2049MiB Linux swap volume

	/1            tap_qemu    network    Ethernet interface





## time



### commande



	time ./sort array 100000

	time ./sort list 100000



### résultat de type



	real	0m28.145s

	user	0m28.099s

	sys		0m0.004s

	

dans le fichier exel j ai mis le `real` pour le deux type de données



## strace



### commande



	strace -tt ./sort array 100000

	strace -tt ./sort list  100000





### résultat de type



	10:02:37.307345 brk(NULL)               = 0x55dff8e6b000

	10:02:37.307694 arch_prctl(0x3001 /* ARCH_??? */, 0x7ffd415159b0) = -1 EINVAL (Invalid argument)

	10:02:37.308102 access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directory)

	10:02:37.308425 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3

	..

	

dans le fichier exel j ai mis la soustraction du temps de la première ligne avec la dernier 



## strace



### commande



	strace -tt ./sort array 100000

	strace -tt ./sort list  100000





### résultat de type



	10:02:37.307345 brk(NULL)               = 0x55dff8e6b000

	10:02:37.307694 arch_prctl(0x3001 /* ARCH_??? */, 0x7ffd415159b0) = -1 EINVAL (Invalid argument)

	10:02:37.308102 access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directory)

	10:02:37.308425 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3

	..

	

dans le fichier excel j ai mis la soustraction du temps de la première ligne avec la dernier 



## gprof



### commande

il faut recompiler avec l' option -pg

	gcc -pg sort.c array_util.c list_util.c  -I../include -o sort

	

Ensuite exécuter le programme et la cmd gprof

	./sort array 100000

	gprof sort gmon.out

	./sort list  100000

	gprof sort gmon.out

	

### résultat de type



	Flat profile:



	Each sample counts as 0.01 seconds.

	  %   cumulative   self              self     total           

	 time   seconds   seconds    calls  ms/call  ms/call  name    

	100.66      0.24     0.24        1   241.58   241.58  array_sort

	  0.00      0.24     0.00        1     0.00     0.00  array_clear

	  0.00      0.24     0.00        1     0.00     0.00  array_init

	.....



dans le fichier excel j ai mis le résultat de la fonction (car les autres partie du programme on un temps négligeable)



## Question: Pourquoi l’une de ces deux fonctions est plus performante que l’autre ?


La fonction **matrix_mult_xchg** est plus performante que le fonction de base car elle utilise mieux la cache du système.

En traitent le donnée ligne par ligne (et pas ligne par colonne) les donnée seront à coter les une des autre et pourront rester dans la cache.

la fonction **matrix_mult_xchg** améliore la localité des donnée. 
