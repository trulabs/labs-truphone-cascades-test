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
* Java library for BDD/TDD

The static library provides a tcp server that the host connects
to and drives the client.

You can, if you wish, use a standard TCP connection
to communicate with the server (i.e. netcat).

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
like easyB or JBehave.

## License

BSD 3-Clause / new / simplified (see LICENSE)

## Latest Changes
#### v1.1.1
* TBD

## Prerequisites
- Qt4 (sdk) & make
- BlackBerry NDK 10.1.0.1020+
- QtCreator (2.8.x+) optional: for editing only

## ToDo

- [ ] Look at integrating the new Automatic Input Control (_sys_inject_events)
* Without the above:
- [ ] Add lots of other support (hopefully AIC will mean we don't have to)

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

The test-cascades-cli will use the qmake/gcc/etc from the host (x86).
The test-cascades-lib will use the qmake/gcc/etc from the NDK.
The test-cascades-java will be built using Gradle.

As these are Qt projects you can use the qmake on the CLI assuming that
you have sourced the bbndk-env.sh file to setup the associated
development environment variables.

## Target Library for BlackBerry

The library is a plain old static library that you need to imbed into your project and call from your main() function.

However, with recent versions, extensions are supported.

### Core

The Core is the main UI testing framework, it'll let you manipulate the UI and control it remotely.

### XMPP

This is the first extension and doesn't have to be included, it's optional. It lets you create XMPP clients so that you can automate the testing of messaging clients.

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
    
    // if you want to test XMPP stuff too
    XmppHarness * const xmppHarness = new XmppHarness();
    if (xmppHarness)
    {
        const bool xmppHarnessOk = xmppHarness->installHarness();
        Q_ASSERT(xmppHarnessOk);
        Q_UNUSED(xmppHarnessOk);
    }

A number of commands can work on objectNames so you can make the script
shorter by using them, otherwise it'll use a path to the object. Use the
record command to work out and discover how paths work.

## Running on the Device

To connect to the server running on the phone (i.e. 169.254.0.x) you need
to ensure development mode is enabled.

## Commands

The list of available commands will change as the BlackBerry 10 NDK
adds more functionality and we add more so it's best to simply
execute the help command (or read the code).

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

'call' lets you call other scripts so that you can create a hierarchy or
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
    tap down keyNumCall
    qml app.logout

## Script Recording

    test-cascades-cli 192.168.70.130 15000 script --record

Once you've exited or logged out you'll find a file named 'script' which you can play back with:

    test-cascades-cli 192.168.70.130 15000 script

## Extensions

From version 1.1.0 onwards the the main functionality has been renamed 'core'. New functionality will
be available via 'extensions'.

### XMPP Extension

The XMPP extension can be used to help automate XMPP messaging clients.

It supports the following commands:

    xmppChatMarker   # displayed/read
    xmppChatState    # composing/paused/active
    xmppConnect      # connect to a server
    xmppCorrect      # edit/delete the last message sent
    xmppDebug        # show the xml received and sent in stream
    xmppDisconnect   # disconnect a resource
    xmppHelp         # overview of the extension
    xmppMessage      # send a message to a client
    xmppPresence     # online/offline/dnd etc & status message
    xmppPrint        # print out the xml for the last message (tx or rx)
    xmppReachability # telephone/address information
    xmppTest         # test variables of last received message

## Correctness & Style
* checked with cppcheck
* style checked with vera++ & modified Google's cpplint.py

## Documentation

Documentation is provided by doxygen, use make doc to generate
the html documentation under ./doc directory.

## Limitations
* I'd like to use libscreen/bps events to capture/reproduce proper touch events but at the moment it doesn't seem to work properly.

## Older versions
#### v1.1.0
* Change the project structure to support extensions
* Add an XMPP extension (requires QXmpp - provided)
* Add List action command to run Context Menu Actions
* Fixed the Spy command for signals with parameters
* SystemDialog can be used on SystemPrompts - but the text is lost :(

#### v1.0.19
* test-cascades-lib uses qDebug to print what's running.
This can be used to correlate the activity of the CLI
and the library - especially when doing post-mortem analysis.
* Add a Quit command to terminate the application
* Bug fixes for the List command

#### v1.0.18
* CLI prints out which file is being read
* Flush event loop before/after commands
* Remove duplicated code in List command
* Fixes

#### v1.0.17
* Move the code for finding QObjects into common place
* Fix the CLI popping commands from the end of a file

#### v1.0.16.1
* 'test' command will accept 'Application' an an object

#### v1.0.16
* Add 'failure-ok' setting to the CLI so you can do the following:

        test myObject text valueMustBeTrue
        action magicActionThatMightHaveEffect
        cli-setting failure-ok true
        test myObject text valueMustBeFalse
        cli-setting failure-ok false

#### v1.0.15
* Fix the CLI file stack in release builds

#### v1.0.14
* Fix the CLI file stack (calling scripts from scripts)

#### v1.0.12
* Console now flushes
* Commands don't just search the current scene()
* sysdialog now can do 'Button'

#### v1.0.11
* Fix the Touch command

#### v1.0.10
* Added support for List hold & release (multiselect)
* Added support for SystemDialogs (sysdialog command)

#### v1.0.9
* The CLI now supports retries. You can enable and disable them as the script runs and configure intervals and maximums
* Added 'settings' to the CLI such as CLI retry
* Remove legacy code (Buffer class)
* Fixes

#### v1.0.8
* Fix release builds (debug ones were fine)

#### v1.0.7
* Added SegmentControl support (segment command) to control them, either by index or by their text value
* Added List tap (i.e. click on) support
* Added CLI 'call' support so you can create snippets and call scripts from other scripts
* Fixes

#### v1.0.6
* More java unit tests, bug fixes

#### v1.0.5
* Added the Page command to check which page your on. This works with the objectName of the page.
* Added (initial) list support to check values and queries QVariantMaps

#### v1.0.4
* C++ include paths fixed - no need to double include

#### v1.0.3
* Java unit tests

#### v1.0.2
* Makefile support for devices and mingw/windows help

#### v1.0.1
* First FOSS release

#### v1.0.0
* Internal release
