ifndef MAKE
	MAKE=make
endif

$(info make binary = $(MAKE))

all:
	@echo 'You need to build in two steps:'
	@echo '1: Run make all-host (make tools for the target)'
	@echo '2: Run make install-host (install the host tools)'
	@echo '3: ** source bbndk-env.sh ** (pulls in the target qmake,make,gcc)'
	@echo '4: Run make all-target (build the lib)'
	@echo '5: Run make doc (optional - needs doxygen)'
	@echo
	@echo 'The CLI won't build if you've already sourced (or run) bbndk-env'

all-host: build-cli build-java

all-target: build-lib

install-host: install-cli install-java

check:
	cppcheck --enable=all --inline-suppr --xml -I test-cascades-lib test-cascades-lib/src/ 2> cppcheck.lib.xml
	@cat cppcheck.lib.xml
	cppcheck --enable=all --inline-suppr --xml -I test-cascades-cli test-cascades-cli/src/ 2> cppcheck.cli.xml
	@cat cppcheck.cli.xml
	vera++ test-cascades-lib/include/*.h test-cascades-lib/src/*.cpp 2>@1 1> vera.lib
	@cat vera.lib
	vera++ test-cascades-cli/include/*.h test-cascades-cli/src/*.cpp 2>@1 1> vera.cli
	@cat vera.cli
	cpplint.py --output=xml --root=test-cascades-lib/include test-cascades-lib/include/*.h test-cascades-lib/src/*.cpp 2>&1 | tee cpplint.lib.xml
	cpplint.py --output=xml --root=test-cascades-cli/include test-cascades-cli/include/*.h test-cascades-cli/src/*.cpp 2>&1 | tee cpplint.cli.xml

clean: clean-lib clean-cli clean-java clean-doc

clean-cli:
	rm -rf test-cascades-cli/bin

clean-lib:
	rm -rf test-cascades-lib/lib

clean-java:
	make -C test-cascades-java clean

clean-doc:
	rm -rf doc

doc: clean-doc
	doxygen Doxyfile

build-cli:
	mkdir -p test-cascades-cli/bin/Release
	(cd test-cascades-cli/bin/Release; qmake ../../test-cascades-cli.pro -r CONFIG+=release QMAKE_CXXFLAGS+=-Wall QMAKE_CXXFLAGS+=-Wextra)
	$(MAKE) -C test-cascades-cli/bin/Release

build-lib:
	mkdir -p test-cascades-lib/lib/Simulator-Debug
	(cd test-cascades-lib/lib/Simulator-Debug; qmake ../../test-cascades-lib.pro -r -spec blackberry-x86-qcc CONFIG+=debug QMAKE_CXXFLAGS+=-Wall QMAKE_CXXFLAGS+=-Wextra)
	$(MAKE) -C test-cascades-lib/lib/Simulator-Debug
	mkdir -p test-cascades-lib/lib/Device-Debug
	(cd test-cascades-lib/lib/Device-Debug; qmake ../../test-cascades-lib.pro -r -spec unsupported/blackberry-armv7le-qcc CONFIG+=debug QMAKE_CXXFLAGS+=-Wall QMAKE_CXXFLAGS+=-Wextra)
	$(MAKE) -C test-cascades-lib/lib/Device-Debug
	mkdir -p test-cascades-lib/lib/Device-Release
	(cd test-cascades-lib/lib/Device-Release; qmake ../../test-cascades-lib.pro -r -spec unsupported/blackberry-armv7le-qcc CONFIG+=release QMAKE_CXXFLAGS+=-Wall QMAKE_CXXFLAGS+=-Wextra)
	$(MAKE) -C test-cascades-lib/lib/Device-Release

build-java:
	make -C test-cascades-java

uninstall-cli:
	rm /usr/bin/test-cascades-cli

install-cli:
	@echo
	@echo "############################################"
	@echo "# Installing CLI to /usr/bin"
	@echo "############################################"
	@echo "* Installing binary"
	@cp test-cascades-cli/bin/Release/test-cascades-cli /usr/bin

install-java:
	make -C test-cascades-java install
