# labs-truphone-cascades-test

Framework that provides introspective automation API for
BlackBerry 10 devices (including simulators) over a network
connection.

## Overview
This framework offers the ability to test bb10 applications
on a device or simulator.

It comes in three parts:
* static-library linked into the instrumented application
* console application on the host
* Java library for bdd/tdd

The static library provides a tcp server that the host connects
to and drives the client.

You can, if you wish, use a standard TCP connection
to communute with the server (i.e. netcat).

Once you connect to the device/simulator it will respond
to every command as a primitive form of flow control.
If the command is successful it will respond 'OK' or
otherwise will respond with an error message.

On login you will get a single 'OK' to let you know that the
connection was made and is ready to accept commands. You can
then use 'help' or 'help <cmd>' where <cmd> is a command to
get more information.

In short you can simulate clicks on cascade controls,
enter text and test for Qt properties (such as text values
and whether a control is enabled or not).

You can also manipulate the native address book.

The provided CLI application can be used to run scripts
and it will generate an XML file with the results. The
scripts are simply the commands you would type but with
added built-in flow-control and XML output.

The Java library can be integrated into BDD/TDD environments
like easyB or JBehave

## License

BSD 3-Clause / new / simplified (see LICENSE)

## Version History
### v1.0.14
* Fix the CLI file stack (calling scripts from scripts)

### v1.0.12
* Console now flushes
* Commands don't just search the current scene()
* sysdialog now can do 'Button'

### v1.0.11
* Fix the Touch command

### v1.0.10
* Added support for List hold & release (multiselect)
* Added support for SystemDialogs (sysdialog command)

### v1.0.9
* The CLI now supports retries. You can enable and disable them as the script runs and configure intervals and maximums
* Added 'settings' to the CLI such as CLI retry
* Remove legacy code (Buffer class)
* Fixes

### v1.0.8
* Fix release builds (debug ones were fine)

### v1.0.7
* Added SegmentControl support (segment command) to control them, either by index or by their text value
* Added List tap (i.e. click on) support
* Added CLI 'call' support so you can create snippets and call scripts from other scripts
* Fixes

### v1.0.6
* More java unit tests, bug fixes

### v1.0.5
* Added the Page command to check which page your on. This works with the objectName of the page.
* Added (initial) list support to check values and queries QVariantMaps

### v1.0.4
* C++ include paths fixed - no need to double include

### v1.0.3
* Java unit tests

### v1.0.2
* Makefile support for devices and mingw/windows help

### v1.0.1
* First FOSS release

### v1.0.0
* Internal release

## Prerequisites
- Qt4 (sdk) & make
- BlackBerry NDK 10.1.0.1020+
- QtCreator (2.8.x+) optional: for editing only

## ToDo

* Look at integrating the new Automatic Input Control (_sys_inject_events)
* Without the above:
* Add lots of other support (hopefully AID will mean we don't have to)

## Building
There are three projects
* test-cascades-lib - the library file to embed
* test-cascades-cli - the CLI application
* test-cascades-java - the Java API library

Or you can just use make

        1. make					# Get some info up
        2. make all-host			# Build the CLI & Java library
        3. source <bbndk-path>/bbndk-env.sh	# Pull in the BlackBerry NDK tools
        4. make all-target			# Build it for the targets
        5. sudo -E make install-cli		# Install CLI into /usr/bin (linux only)

### Building on Windows (mingw)
On windows you may need (depending if you have other make systems present) to
tell the Makefile which make binary to use, for example, I needed to do the following:

        1. mingw32-make MAKE=mingw32-make all-host	# Build on Windows
        2. c:\bbndk\bbndk-env.bat 			# (for example)
        3. make all-target				# Build it for the target

You should only need to do that for the host - once you've sourced bbdnk
then you won't need to.

### Building cont.

Other options are

        make doc			# Generate API documentation
        make clean			# Clean the build
        sudo -E make uninstall-lib	# Uninstall the library

If you want to edit & build the projects:

Both are built with QtCreator 3+ which must be configured
to use the installed BlackBerry 10 NDK for the device or simulator.

the test-cascades-cli will use the qmake/gcc/etc from the host (x86)
the test-cascades-lib will use the qmake/gcc/etc from the ndk
the test-cascades-java will be built using Gradle

As these are Qt projects you can use the qmake on the CLI assuming that
you have sourced the bbndk-env.sh file to setup the associated
development environment variables

## Java Library

The java library uses gradle for dependancy management.

    gradle build	# Build the library file
    gradle test		# Run the unit tests
    gradle clean 	# Clean everything out
    gradle eclipse 	# Generate an eclipse project

## Project Integration

In your project file (.pro) add the following:

    ...
    simulator { # or device
        CONFIG(release, debug|release) {
            DESTDIR = o
        }
        CONFIG(debug, debug|release) {
            DESTDIR = o-g
            INCLUDEPATH += <path/to/lib>/test-cascades-lib
            # Simulator-Debug can be replaced with Device-Debug or Device-Release as needed
            LIBS += -ltest-cascades-lib -L<path/to/lib>/test-cascades-lib/Simulator-Debug
        }
    }
    ...


In your main C++ entry file you need to add the following:

    #include <CascadesHarness.h>
    using truphone::test::cascades::CascadesHarness;

Somewhere you need to add (before the blocking app.exec()) the following:

    // app is the Application instance
    CascadesHarness * harness = new CascadesHarness(&app);
    harness->startHarness(15000); // 15000 is the tcp port number to listen on

A number of commands can work on objectNames so you can make the script
shorter by using them, otherwise it'll use a path to the object. Use the
record command to work out and discover how paths work.

## Running on the Device

To connect to the server running on the phone (i.e. 169.254.0.x) you need
to ensure development mode is enabled.

## Commands

The list of available commands will change as the BlackBerry 10 NDK
adds more functionality and we add more so it's best to simply
execute the help command (or read the code)

Here is a list:

* action
* click
* contacts
* dropdown
* help
* exit (close the connection)
* key
* list (select, scroll, check, tap)
* longClick
* page
* pop
* qml
* record
* segment (SegmentControl)
* sleep
* spy
* stop (recording)
* systemdialog
* tab
* tap (up/down/move/cancel)
* test
* text
* toast
* toggle
* touch (screenx, screeny, winx, winy, localx, localy, target, <receiver>)

## test-cascades-cli

The CLI is a single binary which takes 3 parameters
* the ip of the device/simulator
* the port of the device/simulator (i.e. 15000)
* the script file to execute
* (optional: --record, record what occurs on screen to the script file)

The XML output file will be the same as the input file but suffixed
with .xml.

The CLI can be configured to perform a limited number of retries if commands
or tests fail. This is particularly handy if you're dealing with networks
that may perform unreliably and where you can't rely on sleep commands.

The CLI supports two additional commands, call cli-setting.

'call' lets you call other scripts so that you can create a hierarcy or
common set of scripts to build up larger functional tests from smaller,
more manageable, scripts.

### call example

    call login.txt
    call checkFirstPage.txt
    call gotoTab1.txt
        ...
        tab 1
        call tab1Checks1.txt
            ...
            test object text hello
            ...
        call tab1Checks2.txt
        call tab1Checks3.txt
        ...
    call doSomeStuff.txt
    call checkSettings.txt
    call doLogout.txt

If the call command is sent to the device, it will return an error.

### cli-setting

    # Enable retries
    cli-setting retry 1
    # Set retry period to every second
    cli-setting retry-interval 1000
    # Give the application 60 seconds to retry
    cli-setting retry-max-intervals 60

You can remove settings and leave them at their default value by not
specifying a value, i.e.

    # Disable retries
    cli-setting retry

If the cli-setting command is sent to the device, it will return an error.

### Retries

If we execute a script where a test fails we'll see the following:

    CC "# test that we're on the correct page"
    << page thePage
    >> ERROR: Couldn't find a page
    RT page thePage
    >> ERROR: Couldn't find a page
    RT page thePage
    >> ERROR: Couldn't find a page
    RT page thePage
    >> ERROR: Couldn't find a page
    RT page thePage
    >> OK

### Record mode

With the optional record mode all events that occur are streamed back
to the CLI and recorded to the script file. You can then modify the
script file with your own changes/tests etc and re-execute it
for a test-driven environment.

You can also press CTRL+SHIFT (simulator only) and click on an object and the recorder
will generate test commands for all the object's properties (such
as the text value of a textfield).

## Example Script

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

## Script Recording

    test-cascades-cli 192.168.70.130 15000 script --record

Once you've exited or logged out you'll find a file named 'script' which you can play back with:

    test-cascades-cli 192.168.70.130 15000 script

## Correctness & Style
* checked with cppcheck
* style checked with vera++ & modified Google's cpplint.py

## Documentation

Documentation is provided by doxygen, use make doc to generate
the html documentation under ./doc directory.

## Limitations
* I'd like to use libscreen/bps events to capture/reproduce proper touch events but at the moment it doesn't seem to work properly.
