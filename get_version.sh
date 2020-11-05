#!/bin/bash

if [ -z ${GITHUB_REF} ]; then
      printf 'dev-version'
else
  REF_END="${GITHUB_REF#refs/}"
  REF_MID="${REF_END%%/*}"
  if [ "$REF_MID" == "tags" ]; then
    printf "%s" "${GITHUB_REF#refs/tags/}"
  else
    printf "dev-%s" "${GITHUB_SHA}"
  fi
fi
