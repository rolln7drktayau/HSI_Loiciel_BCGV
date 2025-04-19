#!/bin/bash

#Execution of the driver (on the first terminal)
gnome-terminal -- bash -c driver/bin/driver

#Execution of the application (on the second terminal)
gnome-terminal -- bash -c app/bin/app