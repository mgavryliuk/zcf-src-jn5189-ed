#!/usr/env/bin bash

set -ex

TOOLCHAIN_DOWNLOADED_FILE="toolchain/.downloaded"
if [ -f "${TOOLCHAIN_DOWNLOADED_FILE}" ]; then
    exit 0
fi

TOOLCHAIN_DOWNLOAD_URL="https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"
TOOLCHAIN_DOWNLOAD_PATH="/tmp/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"
TOOLCHAIN_UNPACK_TARGET_DIR="${WORKSPACE}/toolchain"

echo "Downloading BA2 toolchain..."
wget -O "${TOOLCHAIN_DOWNLOAD_PATH}" "${TOOLCHAIN_DOWNLOAD_URL}"

echo "Extracting toolchain..."
tar xvf "${TOOLCHAIN_DOWNLOAD_PATH}" -C "${TOOLCHAIN_UNPACK_TARGET_DIR}" --strip-components=1

touch "${TOOLCHAIN_DOWNLOADED_FILE}"

echo "Cleaning up archive..."
rm "${TOOLCHAIN_DOWNLOAD_PATH}"
