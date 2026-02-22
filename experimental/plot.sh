#!/bin/bash
# Activate virtual environment and run plotter

cd "$(dirname "$0")"
source venv/bin/activate
python3 plotter.py
