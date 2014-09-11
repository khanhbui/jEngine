## jEngine - Make help

help: # help
	@echo "------------------------------------------------------"
	@grep "^##" $(MAKEFILE_LIST) | grep -v grep
	@echo ""
	@grep "^[0-9a-zA-Z\-]*: #" $(MAKEFILE_LIST) | grep -v grep
	@echo "------------------------------------------------------"

ifndef target
target:="bla bla..."
endif

new: # new proj
	@echo $(target)

build-tool: # build atlas generator tool
	cd tool && javac AtlasGenerator.java

atlas: # create atlas
	cd tool && ./run.sh

scale: # scale images upto 2 times
	cd tool && ruby scale_images.rb

jni-build: # build
	cd android/jni && ndk-build

jni-clean: # clean
	cd android/jni && ndk-build clean

keystore: # create keystore
	keytool -genkey -v -keyalg RSA -keystore android/builds/key.keystore -alias swipeblocks -validity 10000

sign: # sign
	jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore android/builds/key.keystore android/builds/release-unsigned.apk swipeblocks

verify: # verify
	jarsigner -verify -verbose -certs android/builds/release-unsigned.apk

zalign: # zalign
	zipalign -f -v 4 android/builds/release-unsigned.apk android/builds/swipeblocks.apk

release: # release
	make sign
	make verify
	make zalign

connect: # connect adb over wifi
	adb tcpip 5555
	adb connect 192.168.0.101:5555
