# -----------------------------------
# -- top level targets

VERSION = 1.0.0

all: tool-check test-cascades-java

clean: clean-test-cascades-java

install:
	test -f dist/test-cascades-java.jar
	cp -v dist/test-cascades-java.jar /usr/share/java/test-cascades-java-$(VERSION).jar
	chmod 644 /usr/share/java/test-cascades-java-$(VERSION).jar
	test -f /usr/share/java/test-cascades-java-$(VERSION).jar
	ln -f -v /usr/share/java/test-cascades-java-$(VERSION).jar /usr/share/java/test-cascades-java.jar
	test -f /usr/share/java/test-cascades-java.jar

# -----------------------------------
# -- target to check required 
# -- tools are installed

REQUIRED_TOOLS = javac java gradle

tool-check:
	@for cmd in $(REQUIRED_TOOLS); do \
		echo Checking for $$cmd ; \
		which $$cmd > /dev/null; \
		if [ $$? -eq 1 ] ; then \
			echo $$cmd is required; \
			exit 1; \
		fi; \
	done

# -----------------------------------
# -- client library
test-cascades-java:
	@gradle build
	
clean-test-cascades-java:
	@gradle clean
