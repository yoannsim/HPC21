# HPC labo2
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


## Analyse du programme base line

### Influence de la taille du fichier
On remarque que sans surprise que plus le ficher est gros plus le temps de transfaire et long

	Taille | débit 1ère exécution | débit 2ème exécution
	-------|----------------------|--------------------
	1KB    | 157.963104 bytes/sec | 532.896939 bytes/sec
	8KB    | 171.030316 bytes/sec | 771.414529 bytes/sec
	16KB   | 165.854538 bytes/sec | 733.589807 bytes/sec
	32KB   | 164.504776 bytes/sec | 542.734884 bytes/sec




Le lien entre taille sur ficher et temps de transfère (le débit) et plus ou moins constant à partir de 8KB.

on remarque aussi que la deuxième exécution offre des débits bien plus rapide, après avoir effectuer plusieurs test j ai trouvé que cela viens principalement du fait que le fichier srv_data.bin est déjà créé avec les bonnes valeurs (cette différence et sûrement dû à une optimisation du syscall write) .

si on supprime client_data.binet que l' on relance le programme on retombe plus ou moins sur les même résultat que la première exécution.

### Hypothèse pour la suite des tests

Pour la suite des analyses j' ai choisi de prendre 8KB comme taille de fichier car pour les tests le temps de transfère reste relativement court (je pourrait augmenter la taille du fichier plus tard en fonction de l' amélioration des débit).

J ai aussi posé comme hypothèse que le cas le plus courant d' utilisation réelle serai que le serveur à déjà envoyer le ficher (à d' autre client) mais que le client ne l' a jamais demandé.
donc il faut effacer client_data.bin mais pas srv_data.bin entre chaque execution du programme.

### Analyse des syscall read et write.

cmd utilisé `sudo strace -f -o out.txt -e trace=write -r -T ./main`
temps totale passé dans un syscall `cut -d " " -f8 out.txt | cut -d "<" -f2 |cut -d ">" -f1 | grep '^\0'| awk '{s+=$1} END {print s}'`

Avec cette cmd le temps de transfère d' un fichier de 8KB est de 66 sec débit 122.423717 bytes/sec (on remarque que le strace dégrade un peu les performances).

Temps passé dans le syscall read environ 0.4096 s (< 0,5% du temps).
Temps passé dans le syscall write environ 65,5 s (~99% du temps).
le temps passé dans les autre syscall est négligeable pour cette version du programme.

suite à cette analyse il est évidant qu il faut optimisé les appelle a write 

## Première optimisation.

une premier optimisation du code consiste a diminuer le nombre d' appel a write (ainsi qu' a read send etc) en utilisant un buffer (de même taille coté client que serveur).

### Taille du buffer
La taille du buffer est pour cette version est limité par le nombre de bytes que la fonction send peut envoyer.

![](/im/erreur.PNG)

la taille max et de 66536 bit (normalement il n y a pas vraiment de limite cela doit venire de la vm).

## Analyse de la premier solution

Pour la suite des analyses j' ai choisi de prendre 128MB comme taille de fichier car le débit a été multiplier par environ 20000

### Analyse des syscall read et write et autre.

cmd utilisé `sudo strace -f -o out.txt -e trace=write -r -T ./main`

temps totale passé dans un syscall `cut -d " " -f8 out.txt | cut -d "<" -f2 |cut -d ">" -f1 | grep '^\0'| awk '{s+=$1} END {print s}'`

Avec cette cmd le temps de transfère d' un fichier de 128MB est de 12 sec débit 10722243.736379 bytes/sec.

Temps passé dans le syscall read environ 0.4 s (~3% du temps).
Temps passé dans le syscall write environ 11.25 s (~93% du temps).
le temps passé dans les syscall sendto et recivfrom 0.2 s (~1.5% du temps) .

suite à cette analyse il est évidant qu il faut encore optimisé les appels a write. 

## Deuxième optimisation.

Pour encore plus diminuer le nombre d' appel a write une solution est d' utilisé un plus gros buffer coté client et d' aller le vider sur le disque seulement après qu un certain nombre de paquet réseau ai été reçu.

### Taille du buffer
Je met pour commencer une taille de 16 X 66536 bit (1MB) je ferai d' autre test avec un plus gros buffer

## Analyse de la deuxième solution

Le nouveau débit est de 71668339.633263 bytes/sec  soit environ 7 foi plus rapide que pour la première optimisation et 149000 que pour le code fourni


### Analyse du syscall write.

cmd utilisé `sudo strace -f -o out.txt -e trace=write -r -T ./main`

temps totale passé dans un syscall `cut -d " " -f8 out.txt | cut -d "<" -f2 |cut -d ">" -f1 | grep '^\0'| awk '{s+=$1} END {print s}'`

Avec cette cmd le temps de transfère d' un fichier de 512MB est de 7,5sec débit 71668339.633263 bytes/sec

Temps passé dans le syscall write environ 5.60341 s (~74% du temps).

suite à cette analyse on remarque que le write occupe encore la majorité (3/4) du temps cpu. 

#### Test avec un plus gros buffer de 4MB (64 * 65536):
temps de transfère d' un fichier de 512MB est de 8.6sec débit 62375791.121654 bytes/sec

Temps passé dans le syscall write environ 6.9782 s (~80% du temps).

Suite à cette analyse on remarque que augmenter plus la taille du buffer n' augment pas les performances du programme pire elle les dégrades.

## conclusion 

Comme les syscall write occupe encore la majorité du temps (environ 75%) on ne pourrai encore augmenter le débit que de environ 25% car on est limité par le write.


![](/im/hotspot.PNG)

L' écriture sur le disque qui est de ~90 MB/s (512MB/5.6s) environ  devrait être normalement beaucoup plus rapide car mon ssd (SSD WDC green 256GB ) à normalement une vitesse en écriture de 450MB/s source(https://www.lesnumeriques.com/ssd/western-digital-green-240-go-p35613/test.html) cela doit surment venire de la VM.