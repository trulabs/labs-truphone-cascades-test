all:
	@echo 'You need to build in two steps:'
	@echo '1: Run make all-host (make tools for the target)'
	@echo '2: Run make install-host (install the host tools)'
	@echo '3: source bbndk-env.sh (pulls in the target qmake,make,gcc)'
	@echo '4: Run make all-target (build the lib)'
	@echo '5: Run make install-lib (install the lib into bbndk)'

all-host: build-cli build-java

all-target: build-lib

install-host: install-cli install-java

check:
	cppcheck --enable=all --inline-suppr --xml -I test-cascades-lib test-cascades-lib/src/ 2> cppcheck.lib.xml
	@cat cppcheck.lib.xml
	cppcheck --enable=all --inline-suppr --xml -I test-cascades-cli test-cascades-cli/src/ 2> cppcheck.cli.xml
	@cat cppcheck.cli.xml
	vera++ -p default test-cascades-lib/include/*.h test-cascades-lib/src/*.cpp -s -x vera.lib.xml
	@cat vera.lib.xml
	vera++ -p default test-cascades-cli/include/*.h test-cascades-cli/src/*.cpp -s -x vera.cli.xml
	@cat vera.cli.xml
	cpplint.py --output=xml --root=test-cascades-lib/include test-cascades-lib/include/*.h test-cascades-lib/src/*.cpp 2>&1 | tee cpplint.lib.xml
	cpplint.py --output=xml --root=test-cascades-cli/include test-cascades-cli/include/*.h test-cascades-cli/src/*.cpp 2>&1 | tee cpplint.cli.xml

clean: clean-lib clean-cli clean-java

clean-cli:
	@rm -rf test-cascades-cli/bin/Release

clean-lib:
	@rm -rf test-cascades-lib/lib/Simulator-Debug

clean-java:
	make -C test-cascades-java clean

doc:
	@rm -rf doc
	@doxygen Doxyfile

build-cli:
	mkdir -p test-cascades-cli/bin/Release
	(cd test-cascades-cli/bin/Release; qmake ../../test-cascades-cli.pro -r CONFIG+=release QMAKE_CXXFLAGS+=-Wall QMAKE_CXXFLAGS+=-Wextra QMAKE_CXXFLAGS+=-DBUFFER_ON_HOST)
	(cd test-cascades-cli/bin/Release; make)

build-lib:
	mkdir -p test-cascades-lib/lib/Simulator-Debug
	(cd test-cascades-lib/lib/Simulator-Debug; qmake ../../test-cascades-lib.pro -r -spec blackberry-x86-qcc CONFIG+=debug QMAKE_CXXFLAGS+=-Wall QMAKE_CXXFLAGS+=-Wextra)
	(cd test-cascades-lib/lib/Simulator-Debug; make)

build-java:
	make -C test-cascades-java

uninstall-lib:
	rm -rf ${QNX_TARGET}/usr/include/test-cascades-lib
	rm -rf ${QNX_TARGET}/x86/lib/test-cascades-lib

uninstall-cli:
	rm /usr/bin/test-cascades-cli

install-cli:
	@echo
	@echo "############################################"
	@echo "# Installing CLI to /usr/bin"
	@echo "############################################"
	@echo "* Installing binary"
	@cp test-cascades-cli/bin/Release/test-cascades-cli /usr/bin

install-lib:
	@echo
	@echo "############################################"
	@echo "# Installing LIB to ${QNX_TARGET}"
	@echo "############################################"
	@echo "* Installing headers"
	@mkdir ${QNX_TARGET}/usr/include/test-cascades-lib
	@cp -R test-cascades-lib/include ${QNX_TARGET}/usr/include/test-cascades-lib
	@echo "* Installing library"
	@mkdir ${QNX_TARGET}/x86/lib/test-cascades-lib/
	@mkdir ${QNX_TARGET}/x86/lib/test-cascades-lib/Simulator-Debug/
	@cp test-cascades-lib/lib/Simulator-Debug/libtest-cascades-lib.a ${QNX_TARGET}/x86/lib/test-cascades-lib/Simulator-Debug/

install-java:
	make -C test-cascades-java install
