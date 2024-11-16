#!/bin/bash
python3 -m venv env
source env/bin/activate
python3 -m pip install matplotlib
python3 -m pip install numpy
python3 script.py
