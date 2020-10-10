#!/bin/bash

WORKSPACE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Global params
QT_GIT_URL="git://code.qt.io/qt/qt5.git"
QT_VER="5.15.0"

PI_USER="dietpi"
PI_IP_ADDR="pifotoframe.local"

SRC_DIR="qt5-src"
GIT_DIR="qt5-git"
BLD_DIR="qt5-build"

CROSS_COMPILE_PATH="/opt/cross-pi-gcc/bin/"

# Funcrion
function rsync2pi {
	printf "rsync %s\n" $1
	rsync -az $PI_USER@$PI_IP_ADDR:$1 $2
}

function configureQT {
	mkdir -p $SRC_DIR/$BLD_DIR && cd $SRC_DIR/$BLD_DIR
	$WORKSPACE/$SRC_DIR/$GIT_DIR/qtbase/configure -release -opengl es2 -device linux-rasp-pi-g++ -device-option CROSS_COMPILE=$CROSS_COMPILE_PATH/arm-linux-gnueabihf- -sysroot $WORKSPACE/sysroot -opensource -confirm-license -rpath -skip qtwayland -skip qtlocation -skip qtscript -skip qtandroidextras -make libs -prefix /usr/local/qt5pi -extprefix $WORKSPACE/$SRC_DIR/qt5pi -hostprefix $WORKSPACE/$SRC_DIR/qt5 -no-use-gold-linker -v -no-gbm -nomake tests -nomake examples
	cd $WORKSPACE
}

function getQT {
if [ ! -d $SRC_DIR ] ; 
	then  
		printf "Cloning Qt%s\n" $QT_VER
		mkdir $SRC_DIR && cd $SRC_DIR
		git clone $QT_GIT_URL -b $QT_VER $GIT_DIR
		cd $GIT_DIR
		perl init-repository
		cp -R qtbase/mkspecs/linux-arm-gnueabi-g++ qtbase/mkspecs/linux-arm-gnueabihf-g++
		sed -i -e 's/arm-linux-gnueabi-/arm-linux-gnueabihf-/g' qtbase/mkspecs/linux-arm-gnueabihf-g++/qmake.conf
		cd $WORKSPACE
	else 
		git -C $SRC_DIR/$GIT_DIR clean -dfx
		printf "Qt5%s already clonned. Clean up previous build\n" $QT_VER
			
fi
}

function mkSysroot {
printf "Creating sysroot directories\n"
mkdir -p sysroot sysroot/usr sysroot/opt &

rsync2pi /lib sysroot &
rsync2pi /usr/include sysroot/usr &
rsync2pi /usr/lib sysroot/usr &
rsync2pi /opt/vc sysroot/opt 

wait 
printf "generating sysroot relative link\n"
./sysroot-relativelinks.py sysroot
}

function makeInstallQT {
cd $WORKSPACE/$SRC_DIR/$BLD_DIR
make -j3 && make install
printf "exit code = %u\n" $?
}

function doAll {
	getQt
	mkSysroot
	configureQT
	makeInstallQT
}
function main() {
	usage="build_qt [-h] [-g] [-c] [-m] [-a]-- shell script to cross compile QT using the toolchaine at $CROSS_COMPILE_PATH\n
		\n
		where:\n
    			\t-h  show this help text\n
    			\t-g  get QT src from $QT_GIT_URL if repo has already been cloned clean up\n
    			\t-s  make sysroot and sync from $PI_USER:$PI_IP_ADDR\n
    			\t-c  configure QT before compiling\n
    			\t-m  make and install QT\n
    			\t-a  do all the above steps\n"
    
	local OPTIND
	while getopts ":hgcscma" opt; do
  		case ${opt} in
    			h ) # process option h
      				echo -e $usage;;
    			g ) # process option t
      				getQT;;
      			s ) # process option t
      				mkSysroot;;
      			c ) # process option t
      				configureQT;;
      			m ) # process option t
      				makeInstallQT;;
      			a ) # process option t
      				doAll;;
    			\? ) 
      				echo -e $usage;;
  		esac
	done
}
main "$@"
