#!/bin/bash

# Start the D-Bus daemon for JACK session management
eval $(dbus-launch --sh-syntax)

# Start JACK in the background
jackd -d alsa &

# Allow a moment for JACK to initialize
sleep 2

# Run the application
exec "$@"
