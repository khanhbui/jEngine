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

jni-build:
	cd android/jni && ndk-build

jni-clean:
	cd android/jni && ndk-build clean

