/*
 * Automatically generated by make menuconfig: don't edit
 */
#define AUTOCONF_INCLUDED
#define CONFIG_UCLINUX 1
#define CONFIG_UID16 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#undef  CONFIG_RWSEM_XCHGADD_ALGORITHM

/*
 * Code maturity level options
 */
#define CONFIG_EXPERIMENTAL 1

/*
 * Loadable module support
 */
#undef  CONFIG_MODULES

/*
 * Processor type and features
 */
#undef  CONFIG_M68000
#undef  CONFIG_M68EN302
#undef  CONFIG_M68328
#undef  CONFIG_M68EZ328
#undef  CONFIG_M68VZ328
#undef  CONFIG_M68332
#undef  CONFIG_M68360
#undef  CONFIG_M5204
#undef  CONFIG_M5206
#undef  CONFIG_M5206e
#undef  CONFIG_M5249
#undef  CONFIG_M5272
#undef  CONFIG_M5280
#undef  CONFIG_M5282
#undef  CONFIG_M5307
#define CONFIG_M5407 1
#define CONFIG_COLDFIRE 1
#define CONFIG_CLOCK_AUTO 1
#undef  CONFIG_CLOCK_11MHz
#undef  CONFIG_CLOCK_16MHz
#undef  CONFIG_CLOCK_20MHz
#undef  CONFIG_CLOCK_24MHz
#undef  CONFIG_CLOCK_25MHz
#undef  CONFIG_CLOCK_29_5MHz
#undef  CONFIG_CLOCK_33MHz
#undef  CONFIG_CLOCK_40MHz
#undef  CONFIG_CLOCK_45MHz
#undef  CONFIG_CLOCK_48MHz
#undef  CONFIG_CLOCK_50MHz
#undef  CONFIG_CLOCK_54MHz
#undef  CONFIG_CLOCK_60MHz
#undef  CONFIG_CLOCK_64MHz
#undef  CONFIG_CLOCK_66MHz
#undef  CONFIG_CLOCK_70MHz
#undef  CONFIG_CLOCK_140MHz
#define CONFIG_M5407C3 1
#undef  CONFIG_CLEOPATRA
#define CONFIG_MOTOROLA 1
#define CONFIG_RAMAUTO 1
#undef  CONFIG_RAM4MB
#undef  CONFIG_RAM8MB
#undef  CONFIG_RAM16MB
#undef  CONFIG_RAM32MB
#undef  CONFIG_RAM64MB
#define CONFIG_AUTOBIT 1
#undef  CONFIG_RAM8BIT
#undef  CONFIG_RAM16BIT
#undef  CONFIG_RAM32bit
#define CONFIG_RAMKERNEL 1
#undef  CONFIG_ROMKERNEL
#undef  CONFIG_HIMEMKERNEL

/*
 * General setup
 */
#define CONFIG_NET 1
#undef  CONFIG_VISWS
#undef  CONFIG_PCI
#undef  CONFIG_MCA
#undef  CONFIG_HOTPLUG
#undef  CONFIG_PCMCIA
#undef  CONFIG_SYSVIPC
#undef  CONFIG_BSD_PROCESS_ACCT
#undef  CONFIG_SYSCTL
#undef  CONFIG_KCORE_ELF
#define CONFIG_KCORE_AOUT 1
#define CONFIG_BINFMT_FLAT 1
#undef  CONFIG_BINFMT_ZFLAT
#undef  CONFIG_BINFMT_SHARED_FLAT
#undef  CONFIG_BINFMT_AOUT
#undef  CONFIG_BINFMT_ELF
#undef  CONFIG_BINFMT_MISC
#undef  CONFIG_PM
#undef  CONFIG_ACPI
#undef  CONFIG_APM

/*
 * Memory Technology Devices (MTD)
 */
#undef  CONFIG_MTD

/*
 * Parallel port support
 */
#undef  CONFIG_PARPORT

/*
 * Plug and Play configuration
 */
#undef  CONFIG_PNP
#undef  CONFIG_ISAPNP

/*
 * Block devices
 */
#undef  CONFIG_BLK_DEV_FD
#undef  CONFIG_BLK_DEV_XD
#undef  CONFIG_PARIDE
#undef  CONFIG_BLK_CPQ_DA
#undef  CONFIG_BLK_CPQ_CISS_DA
#undef  CONFIG_CISS_SCSI_TAPE
#undef  CONFIG_CISS_MONITOR_THREAD
#undef  CONFIG_BLK_DEV_DAC960
#undef  CONFIG_BLK_DEV_UMEM
#undef  CONFIG_BLK_DEV_LOOP
#undef  CONFIG_BLK_DEV_NBD
#define CONFIG_BLK_DEV_RAM 1
#define CONFIG_BLK_DEV_RAM_SIZE (4096)
#undef  CONFIG_BLK_DEV_INITRD
#undef  CONFIG_BLK_DEV_RAMDISK_DATA
#define CONFIG_BLK_DEV_BLKMEM 1
#define CONFIG_NOFLASH 1
#undef  CONFIG_AMDFLASH
#undef  CONFIG_INTELFLASH
#undef  CONFIG_BLK_STATS

/*
 * Networking options
 */
#define CONFIG_PACKET 1
#undef  CONFIG_PACKET_MMAP
#undef  CONFIG_NETLINK_DEV
#undef  CONFIG_NETFILTER
#undef  CONFIG_FILTER
#define CONFIG_UNIX 1
#define CONFIG_INET 1
#undef  CONFIG_IP_MULTICAST
#undef  CONFIG_IP_ADVANCED_ROUTER
#undef  CONFIG_IP_PNP
#undef  CONFIG_NET_ARP_LIMIT
#undef  CONFIG_NET_IPIP
#undef  CONFIG_NET_IPGRE
#undef  CONFIG_ARPD
#undef  CONFIG_INET_ECN
#undef  CONFIG_SYN_COOKIES
#undef  CONFIG_IPV6
#undef  CONFIG_KHTTPD

/*
 *    SCTP Configuration (EXPERIMENTAL)
 */
#define CONFIG_IPV6_SCTP__ 1
#undef  CONFIG_IP_SCTP
#undef  CONFIG_ATM
#undef  CONFIG_VLAN_8021Q
#undef  CONFIG_IPX
#undef  CONFIG_ATALK

/*
 * Appletalk devices
 */
#undef  CONFIG_DEV_APPLETALK
#undef  CONFIG_DECNET
#undef  CONFIG_BRIDGE
#undef  CONFIG_X25
#undef  CONFIG_LAPB
#undef  CONFIG_LLC
#undef  CONFIG_NET_DIVERT
#undef  CONFIG_ECONET
#undef  CONFIG_WAN_ROUTER
#undef  CONFIG_NET_FASTROUTE
#undef  CONFIG_NET_HW_FLOWCONTROL

/*
 * QoS and/or fair queueing
 */
#undef  CONFIG_NET_SCHED
#undef  CONFIG_IPSEC

/*
 * Network testing
 */
#undef  CONFIG_NET_PKTGEN

/*
 * Telephony Support
 */
#undef  CONFIG_PHONE
#undef  CONFIG_PHONE_IXJ
#undef  CONFIG_PHONE_IXJ_PCMCIA

/*
 * ATA/IDE/MFM/RLL support
 */
#undef  CONFIG_IDE
#undef  CONFIG_BLK_DEV_IDE_MODES
#undef  CONFIG_BLK_DEV_HD

/*
 * SCSI support
 */
#undef  CONFIG_SCSI

/*
 * I2O device support
 */
#undef  CONFIG_I2O
#undef  CONFIG_I2O_BLOCK
#undef  CONFIG_I2O_LAN
#undef  CONFIG_I2O_SCSI
#undef  CONFIG_I2O_PROC

/*
 * Network device support
 */
#define CONFIG_NETDEVICES 1

/*
 * ARCnet devices
 */
#undef  CONFIG_ARCNET
#undef  CONFIG_DUMMY
#undef  CONFIG_BONDING
#undef  CONFIG_EQUALIZER
#undef  CONFIG_TUN
#undef  CONFIG_ETHERTAP

/*
 * Ethernet (10 or 100Mbit)
 */
#define CONFIG_NET_ETHERNET 1
#undef  CONFIG_SUNLANCE
#undef  CONFIG_SUNBMAC
#undef  CONFIG_SUNQE
#undef  CONFIG_SUNGEM
#undef  CONFIG_NET_VENDOR_3COM
#undef  CONFIG_LANCE
#undef  CONFIG_NET_VENDOR_SMC
#undef  CONFIG_NET_VENDOR_RACAL
#define CONFIG_NET_ISA 1
#undef  CONFIG_E2100
#undef  CONFIG_EWRK3
#undef  CONFIG_EEXPRESS
#undef  CONFIG_EEXPRESS_PRO
#undef  CONFIG_HPLAN_PLUS
#undef  CONFIG_HPLAN
#undef  CONFIG_LP486E
#undef  CONFIG_ETH16I
#define CONFIG_NE2000 1
#undef  CONFIG_NET_PCI
#undef  CONFIG_NET_POCKET
#undef  CONFIG_FEC
#undef  CONFIG_CS89x0
#undef  CONFIG_UCCS8900

/*
 * Ethernet (1000 Mbit)
 */
#undef  CONFIG_ACENIC
#undef  CONFIG_DL2K
#undef  CONFIG_E1000
#undef  CONFIG_MYRI_SBUS
#undef  CONFIG_NS83820
#undef  CONFIG_HAMACHI
#undef  CONFIG_YELLOWFIN
#undef  CONFIG_R8169
#undef  CONFIG_SK98LIN
#undef  CONFIG_TIGON3
#undef  CONFIG_FDDI
#undef  CONFIG_HIPPI
#undef  CONFIG_PLIP
#define CONFIG_PPP 1
#undef  CONFIG_PPP_MULTILINK
#undef  CONFIG_PPP_FILTER
#define CONFIG_PPP_ASYNC 1
#undef  CONFIG_PPP_SYNC_TTY
#undef  CONFIG_PPP_DEFLATE
#undef  CONFIG_PPP_BSDCOMP
#undef  CONFIG_PPPOE
#define CONFIG_SLIP 1
#define CONFIG_SLIP_COMPRESSED 1
#undef  CONFIG_SLIP_SMART
#undef  CONFIG_SLIP_MODE_SLIP6

/*
 * Wireless LAN (non-hamradio)
 */
#undef  CONFIG_NET_RADIO

/*
 * Token Ring devices
 */
#undef  CONFIG_TR
#undef  CONFIG_NET_FC
#undef  CONFIG_RCPCI
#undef  CONFIG_SHAPER

/*
 * Wan interfaces
 */
#undef  CONFIG_WAN

/*
 * Amateur Radio support
 */
#undef  CONFIG_HAMRADIO

/*
 * IrDA (infrared) support
 */
#undef  CONFIG_IRDA

/*
 * ISDN subsystem
 */
#undef  CONFIG_ISDN

/*
 * Old CD-ROM drivers (not SCSI, not IDE)
 */
#undef  CONFIG_CD_NO_IDESCSI

/*
 * Character devices
 */
#undef  CONFIG_COLDFIRE_USB
#define CONFIG_COLDFIRE_SERIAL 1
#undef  CONFIG_MCF_MBUS
#undef  CONFIG_MCF_QSPI
#undef  CONFIG_LCDTXT
#undef  CONFIG_LCDDMA
#undef  CONFIG_DAC0800
#undef  CONFIG_M5249AUDIO
#undef  CONFIG_M5272AUDIO
#undef  CONFIG_DACI2S
#undef  CONFIG_T6963
#undef  CONFIG_EXP
#undef  CONFIG_RESETSWITCH
#undef  CONFIG_MCFWATCHDOG
#undef  CONFIG_LEDMAN
#undef  CONFIG_DS1302
#undef  CONFIG_VT
#undef  CONFIG_SERIAL
#undef  CONFIG_SERIAL_EXTENDED
#undef  CONFIG_SERIAL_NONSTANDARD
#undef  CONFIG_UNIX98_PTYS

/*
 * I2C support
 */
#undef  CONFIG_I2C

/*
 * Mice
 */
#undef  CONFIG_BUSMOUSE
#undef  CONFIG_MOUSE

/*
 * Joysticks
 */
#undef  CONFIG_INPUT_GAMEPORT
#undef  CONFIG_INPUT_NS558
#undef  CONFIG_INPUT_LIGHTNING
#undef  CONFIG_INPUT_PCIGAME
#undef  CONFIG_INPUT_CS461X
#undef  CONFIG_INPUT_EMU10K1
#undef  CONFIG_INPUT_SERIO
#undef  CONFIG_INPUT_SERPORT
#undef  CONFIG_INPUT_ANALOG
#undef  CONFIG_INPUT_A3D
#undef  CONFIG_INPUT_ADI
#undef  CONFIG_INPUT_COBRA
#undef  CONFIG_INPUT_GF2K
#undef  CONFIG_INPUT_GRIP
#undef  CONFIG_INPUT_INTERACT
#undef  CONFIG_INPUT_TMDC
#undef  CONFIG_INPUT_SIDEWINDER
#undef  CONFIG_INPUT_IFORCE_USB
#undef  CONFIG_INPUT_IFORCE_232
#undef  CONFIG_INPUT_WARRIOR
#undef  CONFIG_INPUT_MAGELLAN
#undef  CONFIG_INPUT_SPACEORB
#undef  CONFIG_INPUT_SPACEBALL
#undef  CONFIG_INPUT_STINGER
#undef  CONFIG_INPUT_DB9
#undef  CONFIG_INPUT_GAMECON
#undef  CONFIG_INPUT_TURBOGRAFX
#undef  CONFIG_QIC02_TAPE
#undef  CONFIG_IPMI_HANDLER
#undef  CONFIG_IPMI_PANIC_EVENT
#undef  CONFIG_IPMI_DEVICE_INTERFACE
#undef  CONFIG_IPMI_KCS
#undef  CONFIG_IPMI_WATCHDOG

/*
 * Controller Area Network Cards/Chips
 */
#undef  CONFIG_CAN4LINUX

/*
 * Watchdog Cards
 */
#undef  CONFIG_WATCHDOG
#undef  CONFIG_SCx200_GPIO
#undef  CONFIG_AMD_PM768
#undef  CONFIG_NVRAM
#undef  CONFIG_RTC
#undef  CONFIG_DTLK
#undef  CONFIG_R3964
#undef  CONFIG_APPLICOM

/*
 * Ftape, the floppy tape device driver
 */
#undef  CONFIG_FTAPE
#undef  CONFIG_AGP

/*
 * Direct Rendering Manager (XFree86 DRI support)
 */
#undef  CONFIG_DRM

/*
 * File systems
 */
#undef  CONFIG_QUOTA
#undef  CONFIG_QFMT_V2
#undef  CONFIG_AUTOFS_FS
#undef  CONFIG_AUTOFS4_FS
#undef  CONFIG_REISERFS_FS
#undef  CONFIG_REISERFS_CHECK
#undef  CONFIG_REISERFS_PROC_INFO
#undef  CONFIG_ADFS_FS
#undef  CONFIG_ADFS_FS_RW
#undef  CONFIG_AFFS_FS
#undef  CONFIG_HFS_FS
#undef  CONFIG_HFSPLUS_FS
#undef  CONFIG_BEFS_FS
#undef  CONFIG_BEFS_DEBUG
#undef  CONFIG_BFS_FS
#undef  CONFIG_EXT3_FS
#undef  CONFIG_JBD
#undef  CONFIG_JBD_DEBUG
#define CONFIG_FAT_FS 1
#undef  CONFIG_MSDOS_FS
#undef  CONFIG_UMSDOS_FS
#undef  CONFIG_VFAT_FS
#undef  CONFIG_EFS_FS
#undef  CONFIG_JFFS_FS
#undef  CONFIG_JFFS2_FS
#undef  CONFIG_CRAMFS
#undef  CONFIG_TMPFS
#define CONFIG_RAMFS 1
#undef  CONFIG_ISO9660_FS
#undef  CONFIG_JOLIET
#undef  CONFIG_ZISOFS
#undef  CONFIG_JFS_FS
#undef  CONFIG_JFS_DEBUG
#undef  CONFIG_JFS_STATISTICS
#undef  CONFIG_MINIX_FS
#undef  CONFIG_VXFS_FS
#undef  CONFIG_NTFS_FS
#undef  CONFIG_NTFS_RW
#undef  CONFIG_HPFS_FS
#define CONFIG_PROC_FS 1
#undef  CONFIG_DEVFS_FS
#undef  CONFIG_DEVFS_MOUNT
#undef  CONFIG_DEVFS_DEBUG
#undef  CONFIG_DEVPTS_FS
#undef  CONFIG_QNX4FS_FS
#undef  CONFIG_QNX4FS_RW
#define CONFIG_ROMFS_FS 1
#define CONFIG_EXT2_FS 1
#undef  CONFIG_SYSV_FS
#undef  CONFIG_UDF_FS
#undef  CONFIG_UDF_RW
#undef  CONFIG_UFS_FS
#undef  CONFIG_UFS_FS_WRITE

/*
 * Network File Systems
 */
#undef  CONFIG_CODA_FS
#undef  CONFIG_INTERMEZZO_FS
#undef  CONFIG_NFS_FS
#undef  CONFIG_NFS_V3
#undef  CONFIG_NFS_DIRECTIO
#undef  CONFIG_ROOT_NFS
#undef  CONFIG_NFSD
#undef  CONFIG_NFSD_V3
#undef  CONFIG_NFSD_TCP
#undef  CONFIG_SUNRPC
#undef  CONFIG_LOCKD
#undef  CONFIG_SMB_FS
#undef  CONFIG_NCP_FS
#undef  CONFIG_NCPFS_PACKET_SIGNING
#undef  CONFIG_NCPFS_IOCTL_LOCKING
#undef  CONFIG_NCPFS_STRONG
#undef  CONFIG_NCPFS_NFS_NS
#undef  CONFIG_NCPFS_OS2_NS
#undef  CONFIG_NCPFS_SMALLDOS
#undef  CONFIG_NCPFS_NLS
#undef  CONFIG_NCPFS_EXTRAS
#undef  CONFIG_ZISOFS_FS

/*
 * Partition Types
 */
#undef  CONFIG_PARTITION_ADVANCED
#undef  CONFIG_SMB_NLS
#define CONFIG_NLS 1

/*
 * Native Language Support
 */
#define CONFIG_NLS_DEFAULT "iso8859-1"
#define CONFIG_NLS_CODEPAGE_437 1
#undef  CONFIG_NLS_CODEPAGE_737
#undef  CONFIG_NLS_CODEPAGE_775
#undef  CONFIG_NLS_CODEPAGE_850
#undef  CONFIG_NLS_CODEPAGE_852
#undef  CONFIG_NLS_CODEPAGE_855
#undef  CONFIG_NLS_CODEPAGE_857
#undef  CONFIG_NLS_CODEPAGE_860
#undef  CONFIG_NLS_CODEPAGE_861
#undef  CONFIG_NLS_CODEPAGE_862
#undef  CONFIG_NLS_CODEPAGE_863
#undef  CONFIG_NLS_CODEPAGE_864
#undef  CONFIG_NLS_CODEPAGE_865
#undef  CONFIG_NLS_CODEPAGE_866
#undef  CONFIG_NLS_CODEPAGE_869
#undef  CONFIG_NLS_CODEPAGE_936
#undef  CONFIG_NLS_CODEPAGE_950
#undef  CONFIG_NLS_CODEPAGE_932
#undef  CONFIG_NLS_CODEPAGE_949
#undef  CONFIG_NLS_CODEPAGE_874
#undef  CONFIG_NLS_ISO8859_8
#undef  CONFIG_NLS_CODEPAGE_1250
#undef  CONFIG_NLS_CODEPAGE_1251
#undef  CONFIG_NLS_ISO8859_1
#undef  CONFIG_NLS_ISO8859_2
#undef  CONFIG_NLS_ISO8859_3
#undef  CONFIG_NLS_ISO8859_4
#undef  CONFIG_NLS_ISO8859_5
#undef  CONFIG_NLS_ISO8859_6
#undef  CONFIG_NLS_ISO8859_7
#undef  CONFIG_NLS_ISO8859_9
#undef  CONFIG_NLS_ISO8859_13
#undef  CONFIG_NLS_ISO8859_14
#undef  CONFIG_NLS_ISO8859_15
#undef  CONFIG_NLS_KOI8_R
#undef  CONFIG_NLS_KOI8_U
#undef  CONFIG_NLS_UTF8

/*
 * Sound
 */
#undef  CONFIG_SOUND

/*
 * USB support
 */
#undef  CONFIG_USB

/*
 * Support for USB gadgets
 */
#undef  CONFIG_USB_GADGET

/*
 * Kernel hacking
 */
#define CONFIG_FULLDEBUG 1
#undef  CONFIG_MAGIC_SYSRQ
#undef  CONFIG_CONTIGUOUS_PAGE_ALLOC
#undef  CONFIG_MEM_MAP
#undef  CONFIG_PROFILE
#undef  CONFIG_DUMPTOFLASH
#define CONFIG_NO_KERNEL_MSG 1
#undef  CONFIG_SMALL_TASKS
#undef  CONFIG_BOOTPARAM

/*
 * Cryptographic options
 */
#undef  CONFIG_CRYPTO

/*
 * Library routines
 */
#undef  CONFIG_CRC32
#undef  CONFIG_ZLIB_INFLATE
#undef  CONFIG_ZLIB_DEFLATE
