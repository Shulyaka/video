video
=====

Video recognition of landing pads for my quadrocopter project.
This is a computer vision project with Seeeduino Mega and Video Experimenter shield by Nootropic Design (http://www.nootropicdesign.com/ve/).
The task is to recognize landing pads with a (X) shape on board of my quadrocopter and to send the corresponding commands to flight controller that can be found on https://github.com/Shulyaka/quadro .
The code requires additional wires as described on this page: http://nootropicdesign.com/projectlab/2011/07/13/ve-on-the-seeeduino-mega/  (however the brown wire should be soldered to pin 18, not 21 as SCL pin is left for I2C communication with sonars)
and customized TVOut library (http://www.nootropicdesign.com/ve/downloads/TVout-VE.zip)
