Este documento foi elaborado seguindo os passos de **Docs.txt/install-guide-unix.txt** do CD do SPEC CPU2006

#### passo 3 - montar a imagem
```bash
[william@odysseuss ~]$ sudo mount -o loop /home/william/projects/particular/nvmsim/vendor/spec_cpu2006v1.01.iso /home/william/projects/particular/nvmsim/shared/media/
[sudo] senha para william: 
mount: /home/william/projects/particular/nvmsim/shared/media: AVISO: dispositivo protegido contra gravação; montado como somente-leitura.
```

#### passo 5 - instalação do SPEC CPU2006
```bash
[william@odysseuss ~]$ docker run -it --rm --privileged -v $NVMSIMPATH/shared:/nvmsim/ nvmsim:1.0.0
root@ff3fd267a660:/nvmsim# cd media/
root@ff3fd267a660:/nvmsim/media# ./install.sh 
SPEC CPU2006 Installation

Top of the CPU2006 tree is '/nvmsim/media'
Enter the directory you wish to install to (e.g. /usr/cpu2006)
/opt/spec_cpu2006

These appear to be valid toolsets:

linux-suse101-i386            For 32-bit i386 Linux systems.
                              Built on SuSE Linux 10.1 with 
                              gcc V4.1.0 (SUSE Linux)

linux-suse101-AMD64           For 64-bit AMD64 Linux systems.
                              Built on SuSE Linux 10.1 with 
                              gcc V4.1.0 (SUSE Linux)
                              Should also be usable on 64 bit EM64T systems

linux-redhat62-ia32           For x86-based Linux systems with GLIBC 2.1.3+
                              Built on RedHat 6.2 with gcc 2.95.3


Enter the architecture you are using:
linux-suse101-AMD64

Unpacking CPU2006 base files (113.1 MB)
Unpacking 400.perlbench benchmark and data files (61.5 MB)
Unpacking 401.bzip2 benchmark and data files (110.7 MB)
Unpacking 403.gcc benchmark and data files (43 MB)
Unpacking 410.bwaves benchmark and data files (0.1 MB)
Unpacking 416.gamess benchmark and data files (16.8 MB)
Unpacking 429.mcf benchmark and data files (6.9 MB)
Unpacking 433.milc benchmark and data files (0.6 MB)
Unpacking 434.zeusmp benchmark and data files (1.1 MB)
Unpacking 435.gromacs benchmark and data files (13 MB)
Unpacking 436.cactusADM benchmark and data files (3.3 MB)
Unpacking 437.leslie3d benchmark and data files (0.3 MB)
Unpacking 444.namd benchmark and data files (7.6 MB)
Unpacking 445.gobmk benchmark and data files (9.2 MB)
Unpacking 447.dealII benchmark and data files (70.2 MB)
Unpacking 450.soplex benchmark and data files (321 MB)
Unpacking 453.povray benchmark and data files (10.3 MB)
Unpacking 454.calculix benchmark and data files (26 MB)
Unpacking 456.hmmer benchmark and data files (57 MB)
Unpacking 458.sjeng benchmark and data files (0.4 MB)
Unpacking 459.GemsFDTD benchmark and data files (2.6 MB)
Unpacking 462.libquantum benchmark and data files (0.2 MB)
Unpacking 464.h264ref benchmark and data files (52.9 MB)
Unpacking 465.tonto benchmark and data files (6.9 MB)
Unpacking 470.lbm benchmark and data files (4.7 MB)
Unpacking 471.omnetpp benchmark and data files (2.8 MB)
Unpacking 473.astar benchmark and data files (5.8 MB)
Unpacking 481.wrf benchmark and data files (67.1 MB)
Unpacking 482.sphinx3 benchmark and data files (51.7 MB)
Unpacking 483.xalancbmk benchmark and data files (213.7 MB)
Unpacking 998.specrand benchmark and data files (4.1 MB)
Unpacking 999.specrand benchmark and data files (4.1 MB)

Checking the integrity of your source tree...


Checksums are all okay.
Unpacking binary tools for linux-suse101-AMD64...
Checking the integrity of your binary tools...

Checksums are all okay.
Top of SPEC benchmark tree is '/opt/spec_cpu2006'
Everything looks okay.  cd to /opt/spec_cpu2006,
source the shrc file and have at it!
root@ff3fd267a660:/nvmsim/media# 
```

#### passo 6 - configuração das variáveis de ambiente
```bash
root@ff7ebe41cbd5:/nvmsim/media# export
declare -x HOME="/root"
declare -x HOSTNAME="ff7ebe41cbd5"
declare -x OLDPWD="/nvmsim"
declare -x PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
declare -x PIN_HOME="/opt/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux"
declare -x PWD="/nvmsim/media"
declare -x SHLVL="1"
declare -x SNIPER_HOME="/opt/sniper-7.2"
declare -x TERM="xterm"
root@ff7ebe41cbd5:/nvmsim/media# cd /opt/spec_cpu2006/
root@ff7ebe41cbd5:/opt/spec_cpu2006# . ./shrc
root@ff7ebe41cbd5:/opt/spec_cpu2006# export
declare -x DYLD_LIBRARY_PATH
declare -x HOME="/root"
declare -x HOSTNAME="ff7ebe41cbd5"
declare -x LC_ALL="C"
declare -x LC_LANG="C"
declare -x LD_LIBRARY_PATH
declare -x OLDPWD="/nvmsim/media"
declare -x PATH="/opt/spec_cpu2006/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
declare -x PIN_HOME="/opt/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux"
declare -x PWD="/opt/spec_cpu2006"
declare -x SHLVL="1"
declare -x SNIPER_HOME="/opt/sniper-7.2"
declare -x SPEC="/opt/spec_cpu2006"
declare -x SPECPERLLIB="/opt/spec_cpu2006/bin:/opt/spec_cpu2006/bin/lib"
declare -x TERM="xterm"
root@ff7ebe41cbd5:/opt/spec_cpu2006# 
```

## criando novo arquivo de configuração

#### verificando versão do GCC
```bash
root@fb722a23170c:/opt/spec_cpu2006# gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/6/lto-wrapper
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Debian 6.3.0-18+deb9u1' --with-bugurl=file:///usr/share/doc/gcc-6/README.Bugs --enable-languages=c,ada,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-6 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-libmpx --enable-plugin --enable-default-pie --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-6-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-6-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-6-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --with-target-system-zlib --enable-objc-gc=auto --enable-multiarch --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 6.3.0 20170516 (Debian 6.3.0-18+deb9u1) 
root@fb722a23170c:/opt/spec_cpu2006# cp config/linux64-amd64-gcc42.cfg /opt/spec_cpu2006/config/linux64-amd64-gcc63.cfg
root@fb722a23170c:/opt/spec_cpu2006# ls
Docs      LICENSE.txt  README.txt  benchspec  cshrc    redistributable_sources  shrc.bat
Docs.txt  MANIFEST     Revisions   bin        install.bat  result           uninstall.sh
LICENSE   README       SUMS.tools  config     install.sh   shrc             version.txt
```

#### verificando localização do GCC
```bash
root@fb722a23170c:/opt/spec_cpu2006# find / -iname gcc
/usr/lib/gcc
/usr/share/lintian/overrides/gcc
/usr/share/doc/gcc-6-base/gcc
/usr/share/doc/gcc
/usr/include/boost/mpl/aux_/preprocessed/gcc
/usr/bin/gcc
```

#### verificando localização do g++ 
```bash
root@fb722a23170c:/opt/spec_cpu2006# find / -iname g++
/usr/share/lintian/overrides/g++
/usr/share/doc/g++
/usr/bin/g++
```

#### verificando localização do gfortran
```bash
root@fb722a23170c:/opt/spec_cpu2006# find / -iname gfortran
```

Como o **gfortran** não foi localizado, ele deve ser instalado.

#### instalando o gfortran
```bash
root@fb722a23170c:/opt/spec_cpu2006# apt-get update
Ign:1 http://deb.debian.org/debian stretch InRelease
Get:2 http://deb.debian.org/debian stretch-updates InRelease [91.0 kB]
Get:3 http://deb.debian.org/debian stretch Release [118 kB]                                   
Get:4 http://security.debian.org/debian-security stretch/updates InRelease [94.3 kB]          
Get:5 http://deb.debian.org/debian stretch Release.gpg [2434 B]                           
Get:6 http://security.debian.org/debian-security stretch/updates/main i386 Packages [480 kB]
Get:7 http://deb.debian.org/debian stretch-updates/main amd64 Packages [11.1 kB]  
Get:8 http://security.debian.org/debian-security stretch/updates/main amd64 Packages [478 kB]
Get:9 http://deb.debian.org/debian stretch-updates/main i386 Packages [11.4 kB]
Get:10 http://deb.debian.org/debian stretch/main i386 Packages [7064 kB]
Get:11 http://deb.debian.org/debian stretch/main amd64 Packages [7084 kB]
Fetched 15.4 MB in 6s (2336 kB/s)                                                                                                     
Reading package lists... Done
root@fb722a23170c:/opt/spec_cpu2006# apt-cache search gfortran
libgraflib1-gfortran - CERNLIB data analysis suite - graphical library
libgrafx11-1-gfortran - CERNLIB data analysis suite - interface to X11 and PostScript
libkernlib1-gfortran - CERNLIB data analysis suite - core library of basic functions
libmathlib2-gfortran - CERNLIB data analysis suite - core mathematical library
libpacklib-lesstif1-gfortran - CERNLIB data analysis suite - core GUI library
libpacklib1-gfortran - CERNLIB data analysis suite - core library
gfortran-6 - GNU Fortran compiler
gfortran-6-multilib - GNU Fortran compiler (multilib support)
lib32gfortran-6-dev - Runtime library for GNU Fortran applications (32bit development files)
lib32gfortran3 - Runtime library for GNU Fortran applications (32bit)
lib32gfortran3-dbg - Runtime library for GNU Fortran applications (32 bit debug symbols)
libgfortran-6-dev - Runtime library for GNU Fortran applications (development files)
libgfortran3 - Runtime library for GNU Fortran applications
libgfortran3-dbg - Runtime library for GNU Fortran applications (debug symbols)
libx32gfortran-6-dev - Runtime library for GNU Fortran applications (x32 development files)
libx32gfortran3 - Runtime library for GNU Fortran applications (x32)
libx32gfortran3-dbg - Runtime library for GNU Fortran applications (x32 debug symbols)
gfortran-6-aarch64-linux-gnu - GNU Fortran compiler
gfortran-6-arm-linux-gnueabi - GNU Fortran compiler
gfortran-6-arm-linux-gnueabihf - GNU Fortran compiler
gfortran-6-mips-linux-gnu - GNU Fortran compiler
gfortran-6-mips64el-linux-gnuabi64 - GNU Fortran compiler
gfortran-6-mipsel-linux-gnu - GNU Fortran compiler
gfortran-6-multilib-mips-linux-gnu - GNU Fortran compiler (multilib support)
gfortran-6-multilib-mips64el-linux-gnuabi64 - GNU Fortran compiler (multilib support)
gfortran-6-multilib-mipsel-linux-gnu - GNU Fortran compiler (multilib support)
gfortran-6-multilib-s390x-linux-gnu - GNU Fortran compiler (multilib support)
gfortran-6-powerpc64le-linux-gnu - GNU Fortran compiler
gfortran-6-s390x-linux-gnu - GNU Fortran compiler
lib32gfortran-6-dev-mips64el-cross - Runtime library for GNU Fortran applications (32bit development files)
lib32gfortran-6-dev-s390x-cross - Runtime library for GNU Fortran applications (32bit development files)
lib32gfortran3-mips64el-cross - Runtime library for GNU Fortran applications (32bit)
lib32gfortran3-s390x-cross - Runtime library for GNU Fortran applications (32bit)
lib64gfortran-6-dev-mips-cross - Runtime library for GNU Fortran applications (64bit development files)
lib64gfortran-6-dev-mipsel-cross - Runtime library for GNU Fortran applications (64bit development files)
lib64gfortran3-mips-cross - Runtime library for GNU Fortran applications (64bit)
lib64gfortran3-mipsel-cross - Runtime library for GNU Fortran applications (64bit)
libgfortran-6-dev-arm64-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-armel-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-armhf-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-mips-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-mips64el-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-mipsel-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-ppc64el-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-s390x-cross - Runtime library for GNU Fortran applications (development files)
libgfortran3-arm64-cross - Runtime library for GNU Fortran applications
libgfortran3-armel-cross - Runtime library for GNU Fortran applications
libgfortran3-armhf-cross - Runtime library for GNU Fortran applications
libgfortran3-mips-cross - Runtime library for GNU Fortran applications
libgfortran3-mips64el-cross - Runtime library for GNU Fortran applications
libgfortran3-mipsel-cross - Runtime library for GNU Fortran applications
libgfortran3-ppc64el-cross - Runtime library for GNU Fortran applications
libgfortran3-s390x-cross - Runtime library for GNU Fortran applications
libn32gfortran-6-dev-mips-cross - Runtime library for GNU Fortran applications (n32 development files)
libn32gfortran-6-dev-mips64el-cross - Runtime library for GNU Fortran applications (n32 development files)
libn32gfortran-6-dev-mipsel-cross - Runtime library for GNU Fortran applications (n32 development files)
libn32gfortran3-mips-cross - Runtime library for GNU Fortran applications (n32)
libn32gfortran3-mips64el-cross - Runtime library for GNU Fortran applications (n32)
libn32gfortran3-mipsel-cross - Runtime library for GNU Fortran applications (n32)
gfortran-6-alpha-linux-gnu - GNU Fortran compiler
gfortran-6-hppa-linux-gnu - GNU Fortran compiler
gfortran-6-m68k-linux-gnu - GNU Fortran compiler
gfortran-6-mips64-linux-gnuabi64 - GNU Fortran compiler
gfortran-6-multilib-mips64-linux-gnuabi64 - GNU Fortran compiler (multilib support)
gfortran-6-multilib-powerpc-linux-gnu - GNU Fortran compiler (multilib support)
gfortran-6-multilib-powerpc64-linux-gnu - GNU Fortran compiler (multilib support)
gfortran-6-multilib-sparc64-linux-gnu - GNU Fortran compiler (multilib support)
gfortran-6-powerpc-linux-gnu - GNU Fortran compiler
gfortran-6-powerpc-linux-gnuspe - GNU Fortran compiler
gfortran-6-powerpc64-linux-gnu - GNU Fortran compiler
gfortran-6-sh4-linux-gnu - GNU Fortran compiler
gfortran-6-sparc64-linux-gnu - GNU Fortran compiler
lib32gfortran-6-dev-mips64-cross - Runtime library for GNU Fortran applications (32bit development files)
lib32gfortran-6-dev-ppc64-cross - Runtime library for GNU Fortran applications (32bit development files)
lib32gfortran-6-dev-sparc64-cross - Runtime library for GNU Fortran applications (32bit development files)
lib32gfortran3-mips64-cross - Runtime library for GNU Fortran applications (32bit)
lib32gfortran3-ppc64-cross - Runtime library for GNU Fortran applications (32bit)
lib32gfortran3-sparc64-cross - Runtime library for GNU Fortran applications (32bit)
lib64gfortran-6-dev-powerpc-cross - Runtime library for GNU Fortran applications (64bit development files)
lib64gfortran3-powerpc-cross - Runtime library for GNU Fortran applications (64bit)
libgfortran-6-dev-alpha-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-hppa-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-m68k-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-mips64-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-powerpc-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-powerpcspe-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-ppc64-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-sh4-cross - Runtime library for GNU Fortran applications (development files)
libgfortran-6-dev-sparc64-cross - Runtime library for GNU Fortran applications (development files)
libgfortran3-alpha-cross - Runtime library for GNU Fortran applications
libgfortran3-hppa-cross - Runtime library for GNU Fortran applications
libgfortran3-m68k-cross - Runtime library for GNU Fortran applications
libgfortran3-mips64-cross - Runtime library for GNU Fortran applications
libgfortran3-powerpc-cross - Runtime library for GNU Fortran applications
libgfortran3-powerpcspe-cross - Runtime library for GNU Fortran applications
libgfortran3-ppc64-cross - Runtime library for GNU Fortran applications
libgfortran3-sh4-cross - Runtime library for GNU Fortran applications
libgfortran3-sparc64-cross - Runtime library for GNU Fortran applications
libn32gfortran-6-dev-mips64-cross - Runtime library for GNU Fortran applications (n32 development files)
libn32gfortran3-mips64-cross - Runtime library for GNU Fortran applications (n32)
gfortran - GNU Fortran 95 compiler
gfortran-aarch64-linux-gnu - GNU Fortran 95 compiler for the arm64 architecture
gfortran-arm-linux-gnueabi - GNU Fortran 95 compiler for the armel architecture
gfortran-arm-linux-gnueabihf - GNU Fortran 95 compiler for the armhf architecture
gfortran-mips-linux-gnu - GNU Fortran 95 compiler for the mips architecture
gfortran-mips64el-linux-gnuabi64 - GNU Fortran 95 compiler for the mips64el architecture
gfortran-mipsel-linux-gnu - GNU Fortran 95 compiler for the mipsel architecture
gfortran-multilib - GNU Fortran 95 compiler (multilib files)
gfortran-multilib-mips-linux-gnu - GNU Fortran 95 compiler for the mips architecture
gfortran-multilib-mips64el-linux-gnuabi64 - GNU Fortran 95 compiler for the mips64el architecture
gfortran-multilib-mipsel-linux-gnu - GNU Fortran 95 compiler for the mipsel architecture
gfortran-multilib-s390x-linux-gnu - GNU Fortran 95 compiler for the s390x architecture
gfortran-powerpc64le-linux-gnu - GNU Fortran 95 compiler for the ppc64el architecture
gfortran-s390x-linux-gnu - GNU Fortran 95 compiler for the s390x architecture
gfortran-alpha-linux-gnu - GNU Fortran 95 compiler for the alpha architecture
gfortran-hppa-linux-gnu - GNU Fortran 95 compiler for the hppa architecture
gfortran-m68k-linux-gnu - GNU Fortran 95 compiler for the m68k architecture
gfortran-mips64-linux-gnuabi64 - GNU Fortran 95 compiler for the mips64 architecture
gfortran-multilib-mips64-linux-gnuabi64 - GNU Fortran 95 compiler for the mips64 architecture
gfortran-multilib-powerpc-linux-gnu - GNU Fortran 95 compiler for the powerpc architecture
gfortran-multilib-powerpc64-linux-gnu - GNU Fortran 95 compiler for the ppc64 architecture
gfortran-multilib-sparc64-linux-gnu - GNU Fortran 95 compiler for the sparc64 architecture
gfortran-powerpc-linux-gnu - GNU Fortran 95 compiler for the powerpc architecture
gfortran-powerpc-linux-gnuspe - GNU Fortran 95 compiler for the powerpcspe architecture
gfortran-powerpc64-linux-gnu - GNU Fortran 95 compiler for the ppc64 architecture
gfortran-sh4-linux-gnu - GNU Fortran 95 compiler for the sh4 architecture
gfortran-sparc64-linux-gnu - GNU Fortran 95 compiler for the sparc64 architecture
gfortran-mingw-w64 - GNU Fortran compiler for MinGW-w64
gfortran-mingw-w64-i686 - GNU Fortran compiler for MinGW-w64 targeting Win32
gfortran-mingw-w64-x86-64 - GNU Fortran compiler for MinGW-w64 targeting Win64
libgeant321-2-gfortran - [Physics] Library for GEANT 3.21
libcojets2-gfortran - [Physics] COJETS p-p and pbar-p interaction Monte Carlo library
libeurodec1-gfortran - [Physics] Monte Carlo library for quark and heavy lepton decays
libherwig59-2-gfortran - [Physics] Monte Carlo event generator simulating hadronic events
libisajet758-3-gfortran - [Physics] Monte Carlo generator for proton / electron reactions
libpdflib804-2-gfortran - [Physics] Comprehensive library of parton density functions
libphotos202-1-gfortran - [Physics] Monte Carlo simulation of photon radiation in decays
libphtools2-gfortran - [Physics] General purpose Monte Carlo routines
libcoarrays-dev - Co-Array Fortran libraries for gfortran - development files
libcoarrays0d - Co-Array Fortran libraries for gfortran
libpawlib-lesstif3-gfortran - CERNLIB PAW library (Lesstif-dependent part)
libpawlib2-gfortran - CERNLIB PAW library - portion without Lesstif dependencies
lib64gfortran-6-dev - Runtime library for GNU Fortran applications (64bit development files)
lib64gfortran3 - Runtime library for GNU Fortran applications (64bit)
lib64gfortran3-dbg - Runtime library for GNU Fortran applications (64bit debug symbols)
root@fb722a23170c:/opt/spec_cpu2006# apt-get install gfortran
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following additional packages will be installed:
  gfortran-6 libgfortran-6-dev libgfortran3
Suggested packages:
  gfortran-multilib gfortran-doc gfortran-6-multilib gfortran-6-doc libgfortran3-dbg libcoarrays-dev
The following NEW packages will be installed:
  gfortran gfortran-6 libgfortran-6-dev libgfortran3
0 upgraded, 4 newly installed, 0 to remove and 0 not upgraded.
Need to get 7481 kB of archives.
After this operation, 28.3 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://deb.debian.org/debian stretch/main amd64 libgfortran3 amd64 6.3.0-18+deb9u1 [265 kB]
Get:2 http://deb.debian.org/debian stretch/main amd64 libgfortran-6-dev amd64 6.3.0-18+deb9u1 [299 kB]
Get:3 http://deb.debian.org/debian stretch/main amd64 gfortran-6 amd64 6.3.0-18+deb9u1 [6916 kB]
Get:4 http://deb.debian.org/debian stretch/main amd64 gfortran amd64 4:6.3.0-4 [1356 B]
Fetched 7481 kB in 2s (2570 kB/s)  
debconf: delaying package configuration, since apt-utils is not installed
Selecting previously unselected package libgfortran3:amd64.
(Reading database ... 51707 files and directories currently installed.)
Preparing to unpack .../libgfortran3_6.3.0-18+deb9u1_amd64.deb ...
Unpacking libgfortran3:amd64 (6.3.0-18+deb9u1) ...
Selecting previously unselected package libgfortran-6-dev:amd64.
Preparing to unpack .../libgfortran-6-dev_6.3.0-18+deb9u1_amd64.deb ...
Unpacking libgfortran-6-dev:amd64 (6.3.0-18+deb9u1) ...
Selecting previously unselected package gfortran-6.
Preparing to unpack .../gfortran-6_6.3.0-18+deb9u1_amd64.deb ...
Unpacking gfortran-6 (6.3.0-18+deb9u1) ...
Selecting previously unselected package gfortran.
Preparing to unpack .../gfortran_4%3a6.3.0-4_amd64.deb ...
Unpacking gfortran (4:6.3.0-4) ...
Setting up libgfortran3:amd64 (6.3.0-18+deb9u1) ...
Setting up libgfortran-6-dev:amd64 (6.3.0-18+deb9u1) ...
Processing triggers for libc-bin (2.24-11+deb9u4) ...
Setting up gfortran-6 (6.3.0-18+deb9u1) ...
Setting up gfortran (4:6.3.0-4) ...
update-alternatives: using /usr/bin/gfortran to provide /usr/bin/f95 (f95) in auto mode
update-alternatives: using /usr/bin/gfortran to provide /usr/bin/f77 (f77) in auto mode
```

#### verificando localização do gfortran
```
root@fb722a23170c:/opt/spec_cpu2006# find / -iname gfortran
/usr/share/lintian/overrides/gfortran
/usr/share/doc/gfortran
/usr/bin/gfortran
```

#### arquivo de configuração
```bash
root@fb722a23170c:/opt/spec_cpu2006# vim /opt/spec_cpu2006/config/linux64-amd64-gcc63.cfg 
```
dentro do arquivo insira o conteúdo abaixo
```bash
# This is a sample config file. It was tested with:
#
#      Compiler name/version:       [gcc, 6.3]
#      Operating system version:    [DEBIAN 9 for AMD64]
#      Hardware:                    [Opteron]
#
# If your platform uses different versions, different
# hardware or operates in a different mode (for
# example, 32- vs. 64-bit mode), there is the possibiliy
# that this configuration file may not work as-is.
#
# Note that issues with compilation should be directed
# to the compiler vendor. Information about SPEC techncial
# support can be found in the techsupport document in the
# Docs directory of your benchmark installation.
#
# Also note that this is a sample configuration. It
# is expected to work for the environment in which
# it was tested; it is not guaranteed that this is
# the config file that will provide the best performance.
#
# Note that you might find a more recent config file for
# your platform with the posted results at
# www.spec.org/cpu2006
####################################################################
# linux64-amd64-gcc63.cfg
# FSF's gcc 6.3 config file for
# AMD64 64-bit (64 bit binaries on 64 bit host)
# Config file for CPU2006 int and fp Base only
#####################################################################

ignore_errors = yes
tune          = base
ext           = amd64-m64-gcc63-nn
output_format = asc, pdf, Screen
reportable    = 1
teeout        = yes
teerunout     = yes
hw_avail = Dec-9999
license_num = 9999
test_sponsor = Turbo Computers
prepared_by = 
tester      =
test_date = Dec-9999

default=default=default=default:
#####################################################################
#
# Compiler selection
#
#####################################################################
CC           = /usr/bin/gcc
CXX          = /usr/bin/g++
FC           = /usr/bin/gfortran

## HW config
hw_model     = Tyan Thunder KKQS Pro (S4882)
hw_cpu_name  = AMD Opteron 256
hw_cpu_char  =
hw_cpu_mhz   = 3000
hw_fpu       = Integrated
hw_nchips = 1
hw_ncores = 1
hw_ncoresperchip= 1
hw_nthreadspercore = 1
hw_ncpuorder = 1 chip
hw_pcache    = 64 KB I + 64 KB D on chip per chip
hw_scache    = 1 MB I+D on chip per chip
hw_tcache    = None
hw_ocache    = None
hw_memory = 2 GB (2 x 1GB DDR333 CL2.5)
hw_disk   = SATA
hw_vendor = Tyan

## SW config
sw_os        = DEBIAN 9 (for AMD64)
sw_file      = ext3
sw_state     = runlevel 3
sw_compiler  = gcc , g++ & gfortran 4.2.0325 (for AMD64)
sw_avail = Dec-9999
sw_other = None
sw_auto_parallel = No
sw_base_ptrsize = 64-bit
sw_peak_ptrsize = Not Applicable


#####################################################################
# Optimization
#####################################################################
## Base is low opt
default=base=default=default:
COPTIMIZE     = -O2
CXXOPTIMIZE  = -O2 
FOPTIMIZE    = -O2

notes0100= C base flags: $[COPTIMIZE]
notes0110= C++ base flags: $[CXXOPTIMIZE]
notes0120= Fortran base flags: $[FOPTIMIZE]

#####################################################################
# 32/64 bit Portability Flags - all
#####################################################################

default=base=default=default:
notes25= PORTABILITY=-DSPEC_CPU_LP64 is applied to all benchmarks in base.
PORTABILITY = -DSPEC_CPU_LP64


#####################################################################
# Portability Flags - INT
#####################################################################

400.perlbench=default=default=default:
notes35    = 400.perlbench: -DSPEC_CPU_LINUX_X64
CPORTABILITY= -DSPEC_CPU_LINUX_X64

462.libquantum=default=default=default:
notes60= 462.libquantum: -DSPEC_CPU_LINUX
CPORTABILITY= -DSPEC_CPU_LINUX

483.xalancbmk=default=default=default:
CXXPORTABILITY= -DSPEC_CPU_LINUX


#####################################################################
# Portability Flags - FP
#####################################################################
481.wrf=default=default=default:
wrf_data_header_size = 8
CPORTABILITY = -DSPEC_CPU_CASE_FLAG -DSPEC_CPU_LINUX
#####################################################################

__MD5__
401.bzip2=base=amd64-m64-gcc63-nn=default:
# Last updated Thu Mar 21 04:12:07 2019
optmd5=dedf19223fd7af9943543aa7a23db222
baggage=
compile_options=\
@eNqdUE0LgkAQvfsrhr2bINFBVNDVZGt1l9RDXQQXCTu44cf/b7VCM7r0mMOwO2/ee4NZzC0whq41\
yroxrkKArkqCvPe1bLqNBD1IeYgLzHPVJkHo5xEo6MyEJ+aBgvLdFt6wOzm0onI1bAHGDlqqoOmR\
+QfGMwd9SCJNOVLL9tSLUvX3LY80NpJ5RmJyCdUIMycSZ6fM8wkl2XnJm1whjZLkuIr6SvEjgC3L\
WyX6zoUZs80xAA3Wqf4xNi1i+XiHxREeNNBtiQ==
exemd5=e7a3409aba510b4774e0332a12377e41

root@9942631ed030:/nvmsim# 
```

#### passo 7 - testes de execução
```bash
root@fb722a23170c:/opt/spec_cpu2006# runspec --config=linux64-amd64-gcc63.cfg -action=build --tune=base bzip2             
runspec v4662 - Copyright 1999-2006 Standard Performance Evaluation Corporation
Using 'linux-suse101-AMD64' tools
Reading MANIFEST... 17910 files
Loading runspec modules...............
Locating benchmarks...found 31 benchmarks in 12 benchsets.
Locating output formats: ASCII, config, CSV, flags, HTML, mail, PDF, PostScript, raw, Screen, Submission Check
Reading config file '/opt/spec_cpu2006/config/linux64-amd64-gcc63.cfg'
Loading "http://www.spec.org/auto/cpu2006/current_version" for version check: OK

NOTICE: There is a newer version of the suite available from SPEC.
  Version 1.200 was released on Wed Aug 17 01:10:33 2011

                       ----------------------------------
                       The run will continue in 5 seconds
                       ----------------------------------
Benchmarks selected: 401.bzip2
Compiling Binaries
  Building 401.bzip2 ref base amd64-m64-gcc63-nn default: (build_base_amd64-m64-gcc63-nn.0000)
specmake clean 2> make.clean.err | tee make.clean.out
rm -rf bzip2 bzip2.exe *.o  *.fppized.f*
find . \( -name \*.o -o -name '*.fppized.f*' \) -print | xargs rm -rf
rm -rf core 
specmake build 2> make.err | tee make.out
/usr/bin/gcc -c -o spec.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         spec.c
/usr/bin/gcc -c -o blocksort.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         blocksort.c
/usr/bin/gcc -c -o bzip2.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         bzip2.c
In file included from bzip2.c:155:0:
bzip2.c: In function 'compressStream':
bzlib.h:80:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
    #  define SPEC_NULLCAST (int)
                            ^
bzip2.c:484:32: note: in expansion of macro 'SPEC_NULLCAST'
       outputHandleJustInCase = SPEC_NULLCAST NULL;
                                ^~~~~~~~~~~~~
bzip2.c:487:27: warning: assignment makes integer from pointer without a cast [-Wint-conversion]
    outputHandleJustInCase = NULL;
                           ^
In file included from bzip2.c:155:0:
bzip2.c: In function 'uncompressStream':
bzlib.h:80:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
    #  define SPEC_NULLCAST (int)
                            ^
bzip2.c:611:32: note: in expansion of macro 'SPEC_NULLCAST'
       outputHandleJustInCase = SPEC_NULLCAST NULL;
                                ^~~~~~~~~~~~~
bzip2.c:614:27: warning: assignment makes integer from pointer without a cast [-Wint-conversion]
    outputHandleJustInCase = NULL;
                           ^
/usr/bin/gcc -c -o bzlib.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         bzlib.c
In file included from bzlib_private.h:73:0,
                 from bzlib.c:77:
bzlib.c: In function 'BZ2_bzWriteOpen':
bzlib.h:80:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
    #  define SPEC_NULLCAST (int)
                            ^
bzlib.c:996:13: note: in expansion of macro 'SPEC_NULLCAST'
    if (f == SPEC_NULLCAST NULL ||
             ^~~~~~~~~~~~~
bzlib.c: In function 'BZ2_bzReadOpen':
bzlib.h:80:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
    #  define SPEC_NULLCAST (int)
                            ^
bzlib.c:1172:13: note: in expansion of macro 'SPEC_NULLCAST'
    if (f == SPEC_NULLCAST NULL ||
             ^~~~~~~~~~~~~
bzlib.c: In function 'bzopen_or_bzdopen':
bzlib.h:80:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
    #  define SPEC_NULLCAST (int)
                            ^
bzlib.c:1512:12: note: in expansion of macro 'SPEC_NULLCAST'
       fp = SPEC_NULLCAST NULL;
            ^~~~~~~~~~~~~
bzlib.h:80:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
    #  define SPEC_NULLCAST (int)
                            ^
bzlib.c:1521:14: note: in expansion of macro 'SPEC_NULLCAST'
    if (fp == SPEC_NULLCAST NULL) return NULL;
              ^~~~~~~~~~~~~
/usr/bin/gcc -c -o compress.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         compress.c
/usr/bin/gcc -c -o crctable.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         crctable.c
/usr/bin/gcc -c -o decompress.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         decompress.c
/usr/bin/gcc -c -o huffman.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         huffman.c
/usr/bin/gcc -c -o randtable.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         randtable.c
/usr/bin/gcc   -O2  -DSPEC_CPU_LP64        spec.o blocksort.o bzip2.o bzlib.o compress.o crctable.o decompress.o huffman.o randtable.o                     -o bzip2
specmake options 2> options.err | tee options.out
COMP: /usr/bin/gcc -c -o options.o -DSPEC_CPU -DNDEBUG    -O2       -DSPEC_CPU_LP64         <source>
C: CC="/usr/bin/gcc"
C: COBJOPT="-c -o options"
P: CPUFLAGS="-DSPEC_CPU -DNDEBUG"
P: BENCH_FLAGS=""
P: BENCH_CFLAGS=""
O: OPTIMIZE=""
O: COPTIMIZE="-O2"
P: PORTABILITY="-DSPEC_CPU_LP64"
P: CPORTABILITY=""
O: EXTRA_CFLAGS=""
O: EXTRA_OPTIMIZE=""
O: EXTRA_COPTIMIZE=""
P: EXTRA_PORTABILITY=""
P: EXTRA_CPORTABILITY=""
LINK: /usr/bin/gcc   -O2  -DSPEC_CPU_LP64        <objects>           -o options
C: LD="/usr/bin/gcc"
O: COPTIMIZE="-O2"
P: PORTABILITY="-DSPEC_CPU_LP64"
C: LDOUT="-o options"

Build successes: 401.bzip2(base)

Build Complete

The log for this run is in /opt/spec_cpu2006/result/CPU2006.001.log

runspec finished at Thu Mar 21 15:14:33 2019; 17 total seconds elapsed
root@fb722a23170c:/opt/spec_cpu2006#
```

#### passo 8 - testes de execução
```bash
root@fb722a23170c:/opt/spec_cpu2006# runspec --config=linux64-amd64-gcc63.cfg --size=test --tune=base --noreportable --iterations=1 bzip2
runspec v4662 - Copyright 1999-2006 Standard Performance Evaluation Corporation
Using 'linux-suse101-AMD64' tools
Reading MANIFEST... 17910 files
Loading runspec modules...............
Locating benchmarks...found 31 benchmarks in 12 benchsets.
Locating output formats: ASCII, config, CSV, flags, HTML, mail, PDF, PostScript, raw, Screen, Submission Check
Reading config file '/opt/spec_cpu2006/config/linux64-amd64-gcc63.cfg'
Benchmarks selected: 401.bzip2
Compiling Binaries
  Up to date 401.bzip2 test base amd64-m64-gcc63-nn default


Parsing Flags
  Looking at 401.bzip2 base amd64-m64-gcc63-nn default: done
Flag Parsing Complete

Setting Up Run Directories
  Setting up 401.bzip2 test base amd64-m64-gcc63-nn default: created (run_base_test_amd64-m64-gcc63-nn.0000)
Running Benchmarks
  Running 401.bzip2 test base amd64-m64-gcc63-nn default
/opt/spec_cpu2006/bin/specinvoke -d /opt/spec_cpu2006/benchspec/CPU2006/401.bzip2/run/run_base_test_amd64-m64-gcc63-nn.0000 -e speccmds.err -o speccmds.stdout -f speccmds.cmd -C
/opt/spec_cpu2006/bin/specinvoke -E -d /opt/spec_cpu2006/benchspec/CPU2006/401.bzip2/run/run_base_test_amd64-m64-gcc63-nn.0000 -c 1 -e compare.err -o compare.stdout -f compare.cmd
Success: 1x401.bzip2
Producing Reports
mach: default
  ext: amd64-m64-gcc63-nn
    size: test
      set: int
        format: raw -> /opt/spec_cpu2006/result/CINT2006.002.test.rsf
        format: flags -> /opt/spec_cpu2006/result/CINT2006.002.test.flags.html
        format: ASCII -> /opt/spec_cpu2006/result/CINT2006.002.test.txt
        format: PDF -> /opt/spec_cpu2006/result/CINT2006.002.test.pdf
        format: Screen -> 

                                  Estimated                       Estimated
                Base     Base       Base        Peak     Peak       Peak
Benchmarks      Ref.   Run Time     Ratio       Ref.   Run Time     Ratio
-------------- ------  ---------  ---------    ------  ---------  ---------
400.perlbench                               NR                                 
401.bzip2          --       4.87         -- S                                  
403.gcc                                     NR                                 
429.mcf                                     NR                                 
445.gobmk                                   NR                                 
456.hmmer                                   NR                                 
458.sjeng                                   NR                                 
462.libquantum                              NR                                 
464.h264ref                                 NR                                 
471.omnetpp                                 NR                                 
473.astar                                   NR                                 
483.xalancbmk                               NR                                 
 Est. SPECint_base2006                   --
 Est. SPECint2006                                                   Not Run

      set: fp

The log for this run is in /opt/spec_cpu2006/result/CPU2006.002.log

runspec finished at Thu Mar 21 15:20:52 2019; 20 total seconds elapsed
root@fb722a23170c:/opt/spec_cpu2006# 

```

#### passo 9 - testes de execução
```bash
root@fb722a23170c:/opt/spec_cpu2006# runspec --config=linux64-amd64-gcc63.cfg --size=ref --noreportable --iterations=1 bzip2
runspec v4662 - Copyright 1999-2006 Standard Performance Evaluation Corporation
Using 'linux-suse101-AMD64' tools
Reading MANIFEST... 17910 files
Loading runspec modules...............
Locating benchmarks...found 31 benchmarks in 12 benchsets.
Locating output formats: ASCII, config, CSV, flags, HTML, mail, PDF, PostScript, raw, Screen, Submission Check
Reading config file '/opt/spec_cpu2006/config/linux64-amd64-gcc63.cfg'
Benchmarks selected: 401.bzip2
Compiling Binaries
  Up to date 401.bzip2 ref base amd64-m64-gcc63-nn default


Parsing Flags
  Looking at 401.bzip2 base amd64-m64-gcc63-nn default: done
Flag Parsing Complete

Setting Up Run Directories
  Setting up 401.bzip2 ref base amd64-m64-gcc63-nn default: created (run_base_ref_amd64-m64-gcc63-nn.0000)
Running Benchmarks
  Running 401.bzip2 ref base amd64-m64-gcc63-nn default
/opt/spec_cpu2006/bin/specinvoke -d /opt/spec_cpu2006/benchspec/CPU2006/401.bzip2/run/run_base_ref_amd64-m64-gcc63-nn.0000 -e speccmds.err -o speccmds.stdout -f speccmds.cmd -C
/opt/spec_cpu2006/bin/specinvoke -E -d /opt/spec_cpu2006/benchspec/CPU2006/401.bzip2/run/run_base_ref_amd64-m64-gcc63-nn.0000 -c 1 -e compare.err -o compare.stdout -f compare.cmd
Success: 1x401.bzip2
Producing Reports
mach: default
  ext: amd64-m64-gcc63-nn
    size: ref
      set: int
        format: raw -> /opt/spec_cpu2006/result/CINT2006.003.rsf
        format: flags -> /opt/spec_cpu2006/result/CINT2006.003.flags.html
        format: ASCII -> /opt/spec_cpu2006/result/CINT2006.003.txt
        format: PDF -> /opt/spec_cpu2006/result/CINT2006.003.pdf
        format: Screen -> 

                                  Estimated                       Estimated
                Base     Base       Base        Peak     Peak       Peak
Benchmarks      Ref.   Run Time     Ratio       Ref.   Run Time     Ratio
-------------- ------  ---------  ---------    ------  ---------  ---------
400.perlbench                               NR                                 
401.bzip2        9650        466       20.7 *                                  
403.gcc                                     NR                                 
429.mcf                                     NR                                 
445.gobmk                                   NR                                 
456.hmmer                                   NR                                 
458.sjeng                                   NR                                 
462.libquantum                              NR                                 
464.h264ref                                 NR                                 
471.omnetpp                                 NR                                 
473.astar                                   NR                                 
483.xalancbmk                               NR                                 
 Est. SPECint_base2006                   --
 Est. SPECint2006                                                   Not Run

      set: fp

The log for this run is in /opt/spec_cpu2006/result/CPU2006.003.log

runspec finished at Thu Mar 21 15:30:45 2019; 481 total seconds elapsed
root@fb722a23170c:/opt/spec_cpu2006# 

```