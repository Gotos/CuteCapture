#!/bin/bash

if [ -z "$CC_VERSION" ]
then
      printf 'dev-version'
else
      printf '%s' "$CC_VERSION"
fi