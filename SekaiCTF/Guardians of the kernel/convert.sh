#!/bin/bash

musl-gcc interact.c -o interact -static
gzip interact
base64 interact.gz
