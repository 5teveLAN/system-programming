#!/bin/bash
# 使用 Tab 4 spaces

echo "Generating test files..."

# 1. 純空洞檔案 (1MB)
truncate -s 1M 1_pure_hole.img

# 2. 結尾空洞檔案 (100K 資料 + 900K 洞)
dd if=/dev/urandom of=2_tail_hole.img bs=1K count=100
truncate -s 1M 2_tail_hole.img

# 3. 開頭空洞檔案 (500K 洞 + 500K 資料)
dd if=/dev/urandom of=3_head_hole.img bs=1K count=500 seek=500

# 4. 碎片化檔案 (資料-洞-資料-洞)
# 第一段資料 (0-4K)
dd if=/dev/zero bs=4K count=1 of=4_fragmented.img
# 跳過中間，在 12K 的位置寫入 4K
dd if=/dev/zero bs=4K count=1 seek=3 of=4_fragmented.img
# 再跳過，在 28K 的位置寫入 4K
dd if=/dev/zero bs=4K count=1 seek=7 of=4_fragmented.img
# 最後把檔案撐到 64K
truncate -s 64K 4_fragmented.img

echo "Done! Use 'ls -ls' to see the difference between size and blocks."
ls -ls *.img
