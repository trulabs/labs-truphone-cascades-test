labs-truphone-cascades-test
===========================

overview
========
this framework offers the ability to test bb10 applications
on a device or simulator.

it comes in three parts:
* static-library linked into the instrumented application
* console application on the host
* java library for bdd/tdd

the static library provides a tcp server that the host connects
to and drives the client

you can, if you wish, use a standard tcp connection
to communute with the server (i.e. netcat)

once you connect to the device/simulator it will respond
to every command as a primitive form of flow control.
if the command is successful it will respond 'OK' or
otherwise will respond with an error message.

on login you will get a single 'OK' to let you know that the
connection was made and is ready to accept commands. you can
then use 'help' or 'help <cmd>' where <cmd> is a command to
get more information.

in short you can simulate clicks on bb cascade controls,
enter text and test for qt properties (such as text values
and whether a control is enabled or not)

you can also manipulate the native address book.

the provided cli application can be used to run scripts
and it will generate an xml file with the results. the
scripts are simply the commands you would type but with
added built-in flow-control and xml output.

the java library can be integrated into bdd/tdd environments
like easyB or jbehave

license
=======

BSD 3-Clause / new / simplified (see LICENSE)

prerequisites
=============
- qt4 (sdk) & make
- blackberry ndk 10.1.0.1020+
- qt creator (2.8.x+) optional: for editing only

todo
====

* look at integrating the new Automatic Input Control (_sys_inject_events)
* without the above:
* add list support
* add lots of other support (hopefully AID will mean we don't have to)

building
========
there are two projects
* test-cascades-lib - the library file to embed
* test-cascades-cli - the cli application

or you can just use make

	1. make					# get some info up
	2. make all-host			# build the cli
	3. source <bbndk-path>/bbndk-env.sh	# pull in the blackberry tools
	4. make all-target			# build it for the target
	5. sudo -E make install-cli		# install cli into /usr/bin (linux only)

building on windows (mingw)
===========================
on windows you may need (depending if you have other make systems present) to
tell the makefile which make binary to use, for example, i needed to do the following

	1. mingw32-make MAKE=mingw32-make all-host	# build on windows
	2. c:\bbndk\bbndk-env.bat 			# (for example)
	3. make all-target				# build it for the target

you should only need to do that for the host - once you've sourced bbdnk
then you won't need to

building cont.
==============

other options are
	
	make doc			# generate API documentation
	make clean			# clean the build
	sudo -E make uninstall-lib	# uninstall the library

if you want to edit & build the projects:

both are built with qt-creator 2.7.0+ which must be configured
to use the installed bb10 ndk for the device or simulator

the test-cascades-lib will use the qmake/gcc/etc from the ndk
the test-cascades-cli will sue the qmake/gcc/etc from the host (x86)

as these are qt projects you can use the qmake on the cli assuming that
you have sourced the bbndk-env.sh file to setup the associated
development environment variables

java library
============

the java library uses gradle for dependancy management

    gradle build # build the library file
	gradle test # run the unit tests (-D properties are needed to define the server address)
	gradle clean # clean everything out
	gradle eclipse # generate an eclipse project

integration
===========

at the top you need to add the following:

        #include <include/CascadesHarness.h>
        using truphone::test::cascades::CascadesHarness;

somewhere in your main.cpp you need to add (before the blocking app.exec())
the following:

	CascadesHarness * harness = new CascadesHarness(&app);
	harness->startHarness(15000); // 15000 is the tcp port number to listen on

in your project file add the following:

	...
	simulator {
        	CONFIG(release, debug|release) {
                	DESTDIR = o
	        }
	        CONFIG(debug, debug|release) {
	                DESTDIR = o-g
	                INCLUDEPATH += ${QNX_TARGET}/usr/include/test-cascades-lib
        	        LIBS += -ltest-cascades-lib -L${QNX_TARGET}/x86/lib/test-cascades-lib/Simulator-Debug
	        }
	}
	...

the click/touch commands can work on objectNames so you can make the script
shorter by using them, otherwise it'll use a path to the object

commands
========

the list of available commands will change as the bb10 ndk
adds more functionality and we add more so it's best to simply
execute the help command (or read the code)

here is a short list

* contacts
* click
* longClick
* tap (up/down/move/cancel)
* tab
* sleep
* text
* test
* touch (screenx, screeny, winx, winy, localx, localy, target)
* record
* key
* touch
* action
* qml
* toggle
* spy

test-cascades-cli
=================

the cli is a single binary which takes 3 parameters
* the ip of the device/simulator
* the port of the device/simulator (i.e. 15000)
* the script file to execute
* (optional: --record, record what occurs on screen to the script file)

the xml output file will be the same as the input file but suffixed
with .xml

with the optional record mode all events that occur are streamed back
to the cli and recorded to the script file. you can then modify the
script file with your own changes/tests etc and re-execute it
for a test-driven environment

you can also press CTRL+SHIFT and click on an object and the recorder
will generate test commands for all the object's properties (such
as the text value of a textfield)

example script
==============

	text createOrLoginUserName myUsername
	text createOrLoginPassword noPassword
	test createOrLoginUserName text myUsername
	test createOrLoginPassword text noPassword
	click createOrLoginLogin
	sleep 5000
	tab Contacts
	click keyNum5
	click keyNum0
	click keyNum4
	click keyNumBackspace
	click keyNum5
	test keyNumToDial text 505
	touch down keyNumCall
	qml app.logout

script recording
================

	test-cascades-cli 192.168.70.130 15000 script --record

Once you've exited or logged out you'll find a file named 'script' which you can play back with

	test-cascades-cli 192.168.70.130 15000 script

correctness & style
===================
* checked with cppcheck
* style checked with vera++ & cpplint.py

documentation
=============

documentation is provided by doxygen, use make doc to generate
the html documentation under ./doc

limitations
===========
* i'd like to use libscreen/bps events to capture/reproduce proper touch events but at the moment it doesn't seem to work properly
