#!/bin/bash

# Ensure unlimited memlock before starting JACK
ulimit -l unlimited

echo "YOOOOOOOOOOO"

# Start JACK in the background
# jackd -d alsa &
jackd -d alsa -d hw:1,0 &

# Allow a moment for JACK to initialize
sleep 2

# Start a shell if no arguments are given or forward commands
exec "$@"
