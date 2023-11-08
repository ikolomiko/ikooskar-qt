#!/usr/bin/env bash

SECONDS=0
cd "$(dirname "$0")/../build"

flatpak-builder flatpak-build/ ../flatpak-manifest.json --force-clean
flatpak build-export ikooskar-flatpak flatpak-build
flatpak build-bundle ikooskar-flatpak ikoOSKAR.flatpak tr.com.ikooskar.ikoOSKAR

echo "Took $SECONDS seconds"
