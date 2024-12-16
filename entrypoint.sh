#!/bin/bash

# Ensure unlimited memlock before starting JACK
ulimit -l unlimited

# Start JACK in the background
jackd -d alsa &

# Allow a moment for JACK to initialize
sleep 2

# Start a shell if no arguments are given or forward commands
exec "$@"
