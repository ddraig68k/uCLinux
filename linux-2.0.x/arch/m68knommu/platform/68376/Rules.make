#
# 68EZ328/Makefile
#
# This file is included by the global makefile so that you can add your own
# platform-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2000  Lineo, Inc.  (www.lineo.com) 
# Copyright (C) 1998,1999  D. Jeff Dionne <jeff@lineo.ca>
# Copyright (C) 1998       Kenneth Albanowski <kjahds@kjahds.com>
# Copyright (C) 1994 by Hamish Macdonald
#

CROSS_COMPILE = m68k-elf-

LIBGCC = `$(CC) -v 2>&1 | grep specs | sed -e "s/Reading specs from //" | sed -e s/specs/m68000\\\/libgcc.a/`

CFLAGS := $(CFLAGS) -pipe -DNO_MM -DNO_FPU  -mcpu32 -D__ELF__ -DMAGIC_ROM_PTR -DNO_FORGET -DUTS_SYSNAME='"uClinux"'
#CFLAGS := $(CFLAGS) -pipe -DNO_MM -DNO_FPU -m68000 -D__ELF__ -DMAGIC_ROM_PTR -DNO_FORGET -DUTS_SYSNAME='"uClinux"'
AFLAGS := $(AFLAGS) -pipe -DNO_MM -DNO_FPU -mcpu32 -D__ELF__ -DMAGIC_ROM_PTR -DUTS_SYSNAME='"uClinux"'
#AFLAGS := $(AFLAGS) -pipe -DNO_MM -DNO_FPU -m68000 -D__ELF__ -DMAGIC_ROM_PTR -DUTS_SYSNAME='"uClinux"'


LINKFLAGS = -T arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)/$(MODEL).ld

HEAD := arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)/crt0_$(MODEL).o

#LINKFLAGS = -T arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD).ld
#HEAD := arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)head.o

SUBDIRS := arch/$(ARCH)/kernel arch/$(ARCH)/mm arch/$(ARCH)/lib \
           arch/$(ARCH)/platform/$(PLATFORM) $(SUBDIRS)
ARCHIVES := arch/$(ARCH)/kernel/kernel.o arch/$(ARCH)/mm/mm.o \
            arch/$(ARCH)/platform/$(PLATFORM)/platform.o	$(ARCHIVES)
LIBS += arch/$(ARCH)/lib/lib.a $(LIBGCC)

ifdef CONFIG_FRAMEBUFFER
SUBDIRS := $(SUBDIRS) arch/$(ARCH)/console
ARCHIVES := $(ARCHIVES) arch/$(ARCH)/console/console.a
endif

MAKEBOOT = $(MAKE) -C arch/$(ARCH)/boot

romfs.s19: romfs.img arch/$(ARCH)/empty.o
	$(CROSS_COMPILE)objcopy -v -R .text -R .data -R .bss --add-section=.fs=romfs.img --adjust-section-vma=.fs=0xa0000 arch/$(ARCH)/empty.o romfs.s19
	$(CROSS_COMPILE)objcopy -O srec romfs.s19

linux.data: linux
	$(CROSS_COMPILE)objcopy -O binary --remove-section=.romvec --remove-section=.text --remove-section=.ramvec --remove-section=.bss --remove-section=.eram linux linux.data

linux.text: linux
	$(CROSS_COMPILE)objcopy -O binary --remove-section=.ramvec --remove-section=.bss --remove-section=.data --remove-section=.eram --set-section-flags=.romvec=CONTENTS,ALLOC,LOAD,READONLY,CODE linux linux.text

romfs.img:
	echo creating a vmlinux rom image without root filesystem!

linux.bin: linux.text linux.data romfs.img
	if [ -f romfs.img ]; then\
		cat linux.text linux.data romfs.img > linux.bin;\
	else\
		cat linux.text linux.data > linux.bin;\
	fi

linux.trg linux.rom: linux.bin
	perl arch/$(ARCH)/platform/$(PLATFORM)/fixup.pl

archclean:
	@$(MAKEBOOT) clean
	rm -f linux.text linux.data linux.bin linux.rom linux.trg
	rm -f linux.s19 romfs.s19
	rm -f linux.img romdisk.img
