![Logo](icon.jpg)
# ProtonELV
(aka “Proton expendable launch vehicle” with an emphasis on *expendable*)

A companion homebrew for Nintendo Switch that might prove useful if you are into https://github.com/XorTroll/uLaunch.

It can swap stock Home menu (qlaunch) with uLaunch and apply changes via rebooting to payload, backing up everything useful (e.g. a Home menu theme) you might have had in your relevant “contents” folders in the process. 

## Installation and setup 
* Download uLaunch’s release https://github.com/XorTroll/uLaunch/releases archive, place /ulaunch from the archive to the root of your SD card, create a folder named “ProtonELV” (without quotation marks, duh) inside /ulaunch. Place the contents of /atmosphere/contents (pun not intended; four folders) from the uLaunch’s archive into /ulaunch/ProtonELV.
* Uninstall a custom Home menu theme if you have one and reboot. I found no way to swap folders which are MITM’ed without rebooting.
* Download ProtonELV.nro https://github.com/octopuserectus/ProtonELV/releases and delicately place it into /switch or wherever you want.
* Start humming “L’Internationale”.
* Run ProtonELV via Homebrew Menu (applet or title redirection mode, whatever rocks your spaceship).

## Controls
Controls are explained via on-screen prompts. 

## Planned features (aka “would never implement those”)
Online install and update, install nigthly builds, not suck in general. 

## Credits
Let’s be honest, it’s just a few extra lines of code inside https://github.com/Atmosphere-NX/Atmosphere/tree/master/troposphere/reboot_to_payload.
