# 段階8：Codexの事前検証

実施日：2026-10-31（JST）。本人の実習と区別する。
ボードへの接続・書き込み・UART操作・実機GDB操作は行っていない。
以下はツールの実行結果から要点を記録したものであり、実機UARTの原文ログではない。

## 採用環境と外部OSS

- 開始時の学習リポジトリ：変更なしmain、`3e17fb2`。`codex/stage8-memory-ram`を作成。
- Zephyr HEAD：`dccb09599635bdff17633fa7e9dab014b91dce90`、変更なし。
- NT-Shell 0.3.1を段階7から独立コピー。[固定情報](../UPSTREAM.md)を記録。
- `python stages/08_memory_ram/scripts/verify_vendor.py --archive .tmp/ntshell-v0.3.1.tar.gz`：
  `PASS: 16 NT-Shell files match the official archive and checksums`、終了コード0。

## CPU0向けビルド

学習リポジトリのルートで実行。本人用の外部workspace内のbuildとは別の出力先を使った。

```powershell
. .\stages\01_hello_world\scripts\env.ps1
west build -b frdm_mcxn947/mcxn947/cpu0 stages/08_memory_ram `
    -d .tmp/build/08_memory_ram_codex -- `
    -DUSER_CACHE_DIR="$learningRepo/.tmp/cache"
```

165/165リンク完了、終了コード0。Zephyr 4.4.2、SDK 1.0.1、GCC 14.3.0、
Python 3.12.10、west 1.5.0、CMake 3.31.8、DTC 1.6.1をビルド表示で確認。
生成設定中に出力が途切れたためプロセスを読み取り確認したが、停止・再試行はせず、そのまま正常完了した。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 34,200 B | 2,097,152 B | 1.63% |
| SRAM | 10,992 B | 524,288 B | 2.10% |

RAMの割り当て領域320 KiBを分母にすると3.35%。SRAMX・IDT_LISTは0 B。
段階7の同じ構成の記録に対してFlashは2,824 B増、RAMは128 B増。
ビルド時の静的使用量であり、実行時最大スタック／ヒープは未測定。

### 生成物の照合

- CMakeCacheのアプリ・prj.conf・CPU0・Zephyrパス・SDKコンパイラが意図したものと一致。
- `.config`：SERIAL・UART_INTERRUPT_DRIVEN・UART_MCUX_LPUART・GPIO・LITTLE_ENDIAN有効。
- `.config`：CONSOLE・CONSOLE_SUBSYS・SHELL・PRINTK・LOG無効、main優先度6・スタック4096 B。
- `nm`：`_flash_used=0x8598`、`_image_ram_size=0x2af0`。
- `nm`：`memory_test_buffer`は`0x30000730`、大きさ`0x80`、BSS、4 B境界。
- SDKのGDBを**ELFだけ**へバッチ起動し、`info address memory_test_buffer`で静的領域`0x30000730`、
  `ptype memory_test_buffer`で`unsigned int [32]`を確認。実機メモリ内容の確認とは異なる。
- `runners.yaml`：LinkServer、MCXN947:FRDM-MCXN947、cm33_core0。

このアドレスはCodexの今回のビルド値であり、本人のシェル入力へ固定値として流用しない。

## QEMUのコマンドテスト

```powershell
west build -b qemu_cortex_m3 stages/08_memory_ram/tests/commands `
    -d .tmp/build/08_memory_ram_commands_codex -- `
    -DUSER_CACHE_DIR="$learningRepo/.tmp/cache"
$env:Path = 'C:\Program Files\Git\mingw64\bin;' + $env:Path
west build -d .tmp/build/08_memory_ram_commands_codex -t run
# 成功表示後にCtrl+A、XでQEMUを終了する。
```

139/139リンク完了、ビルド終了コード0。
QEMUで`TESTSUITE stage8_commands succeeded`、`pass = 15, fail = 0, skip = 0`、
`PROJECT EXECUTION SUCCESSFUL`を確認。Ctrl+A、Xで正常終了し、runも終了コード0。
PATH追加は既存GitのGCCランタイムDLLを利用するための実行プロセスだけの設定。

本物の`commands.c`・`memory_commands.c`・上流`ntopt.c`をリンクし、UART出力・LED操作・
受信異常フラグをテスト用に置換。表示だけでなく、専用RAMのバイト列を直接読んで照合した。
テスト用のconsole設定・使用量は実機アプリと区別する。

| テスト | 結果・対象 |
|---|---|
| test_ram_pattern_and_info | PASS：128 B初期パターン・範囲表示・全幅の初期値 |
| test_write_widths_and_dump | PASS：全幅の書き込みとread、実バイト列とdumpの一致 |
| test_last_valid_access_and_max_values | PASS：各幅の最終有効位置、最大値とゼロ |
| test_bad_ranges_never_access_memory | PASS：0、先頭未満、終端、UINTPTR_MAX付近、終端超過の拒否、RAM不変 |
| test_unaligned_access_rejected | PASS：16／32 bit不整列の拒否、RAM不変、非整列の1 B dump受理 |
| test_bad_numbers_widths_and_lengths | PASS：数値の桁あふれ・符号・不正桁・不正幅・長さ・余分な引数の拒否、RAM不変 |
| test_dump_maximum_and_partial_row | PASS：64 B上限と部分行、終端の直前までの表示 |
| test_fault_and_long_line_block_ram_write | PASS：受信異常・63 B満杯行で書き込み抑止、拒否後の正常復帰 |
| test_help_and_empty | PASS：help、空行・空白 |
| test_valid_led_commands | PASS：LEDコマンドの既存解析 |
| test_invalid_input_never_changes_led | PASS：不正なLEDコマンドで操作なし |
| test_length_boundary_and_many_arguments | PASS：62／63 B境界と多数引数 |
| test_rx_loss_blocks_execution | PASS：受信欠落時のLED操作抑止 |
| test_gpio_error_has_no_success_reply | PASS：GPIO失敗時に成功表示なし |
| test_blink_error_reported_before_next_command | PASS：非同期GPIOエラーの通知 |

## 事前検証時点の残る確認

以下はCodexの事前検証を終えた時点の一覧。後続の本人の実機結果は[段階8の記録](../README.md#確認結果と未確認事項)と[user-evidence](../user-evidence/README.md)を参照する。

- 本人の環境・ソース確認、ビルド・書き込み・接続先の再確認。
- 実機の全幅のread／write／dump、境界・不正入力の拒否とその後の復帰。
- 同じELF・実行時の既知パターンを使った本人のGDB照合。
- 入力待ち・dump中の実機LED継続。UART・編集・エラー復帰の実機経路。
- 本人のREADME・PR確認、明示的なマージ・クローズ承認。イシュー #9はOPEN。
