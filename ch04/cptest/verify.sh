#!/bin/bash

SRC=$1
DEST=$2
ERROR=0

echo "Testing $SRC -> $DEST"

# 1. 檢查邏輯大小是否完全相同
SIZE_SRC=$(stat -c %s "$SRC")
SIZE_DEST=$(stat -c %s "$DEST")

if [ "$SIZE_SRC" -eq "$SIZE_DEST" ]; then
    echo "[OK] Size matches: $SIZE_SRC"
else
    echo "[FAIL] Size mismatch! Src: $SIZE_SRC, Dest: $SIZE_DEST"
    ERROR=1
fi

# 2. 檢查內容 MD5 (確保 read/write 沒讀歪)
# 注意：md5sum 會讀取空洞並將其視為 0，所以兩者必須相同
MD5_SRC=$(md5sum "$SRC" | awk '{print $1}')
MD5_DEST=$(md5sum "$DEST" | awk '{print $1}')

if [ "$MD5_SRC" == "$MD5_DEST" ]; then
    echo "[OK] MD5 matches: $MD5_SRC"
else
    echo "[FAIL] Content mismatch!"
    ERROR=1
fi

# 3. 檢查實體區塊佔用 (驗證是否有成功創造空洞)
BLOCKS_SRC=$(stat -c %b "$SRC")
BLOCKS_DEST=$(stat -c %b "$DEST")

echo "Physical Blocks - Src: $BLOCKS_SRC, Dest: $BLOCKS_DEST"
if [ "$BLOCKS_DEST" -le "$BLOCKS_SRC" ]; then
    echo "[OK] Sparse property preserved."
else
    echo "[WARN] Destination is larger than source! Sparse blocks might be lost."
    ERROR=1
fi
echo "---------------------------------------"

exit $ERROR
