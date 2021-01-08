#!/bin/bash
shopt -s nullglob

for file in $1/*
do
  convert $file -compress none $file
done
shopt -u nullglob #revert nullglob back to it's normal default state
