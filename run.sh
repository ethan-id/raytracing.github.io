#!/bin/bash

cmake --build build

rm image.ppm

build/myWeekend > image.ppm

open image.ppm

