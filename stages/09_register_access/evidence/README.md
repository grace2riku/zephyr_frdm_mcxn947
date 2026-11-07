# 段階9：Codexの事前検証

実施日：2026-11-01（JST）。本人の実習と区別する。
ボードへの書き込み・UART操作・実機GDB接続は行っていない。
以下は実行結果の要約であり、実機UARTの原文ログではない。
ビルド・QEMU検証時点ではRM本文は未照合だった。その後、同日に本人提供PDFとの照合を完了した。
[RM照合記録](../REGISTER_REFERENCE.md)参照。実装の変更は不要で、以下のソフトウェア検証結果を継続して使用する。

## 採用環境と外部OSS

- 開始時の学習リポジトリ：変更なしmain `1d28591`。
  `codex/stage9-register-access`を作成した。
- Zephyr HEAD `dccb09599635bdff17633fa7e9dab014b91dce90`、変更なし。
- NXP HAL HEAD `7554bc0f6f6c284972eba7b2c17b16fbb9cc0ad0`。
- NT-Shell公式0.3.1を段階8から独立コピー。ライセンス表示を保持。
- `python stages/09_register_access/scripts/verify_vendor.py --archive .tmp/ntshell-v0.3.1.tar.gz`：
  `PASS: 16 NT-Shell files match the official archive and checksums`、終了コード0。

## CPU0向けビルド

リポジトリのルートで実行。本人用の外部workspaceのbuildとは別の保存先。

```powershell
. .\stages\01_hello_world\scripts\env.ps1
west build -b frdm_mcxn947/mcxn947/cpu0 stages/09_register_access `
    -d .tmp/build/09_register_access_codex -- `
    -DUSER_CACHE_DIR="$learningRepo/.tmp/cache"
```

最初のサンドボックス内の実行はDTCのMSYS初期化でアクセス拒否となった。
同じコマンドを制限外で再実行し、166/166リンク完了、終了コード0。
Zephyr 4.4.2、SDK 1.0.1、GCC 14.3.0、Python 3.12.10、west 1.5.0、
CMake 3.31.8、DTC 1.6.1を表示で確認した。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 34,436 B | 2,097,152 B | 1.64% |
| SRAM | 8,744 B | 524,288 B | 1.67% |

RAM割当領域320 KiBを分母にすると2.67%。SRAMX・IDT_LISTは0 B。
段階8の記録に対してFlash +236 B、RAM -2,248 B。機能追加とLEDスレッド削除を含む差分。
実行時最大スタック／ヒープ、ハードウェア動作を表す値ではない。

### 生成物の照合

- CMakeCache：段階9アプリ・prj.conf、CPU0、採用Zephyr、SDKコンパイラを確認。
- `.config`：SERIAL／UART_INTERRUPT_DRIVEN／UART_MCUX_LPUART／GPIO／ARM_SECURE_FIRMWARE有効。
- `.config`：CONSOLE／CONSOLE_SUBSYS／SHELL／PRINTK／LOG／LED／INPUT無効。
  main優先度6、mainスタック4096 B。
- 生成Devicetreeヘッダー：GPIO0 `0x50096000`、赤色LED pin 10、flags 1（Active Low）。
- ソースのBUILD_ASSERT：GPIO0・pin・極性、GPIO0_BASE、GPIO_Typeの6レジスタのoffsetと32 bit型に一致。
- `nm`：`_flash_used=0x8684`、`_image_ram_size=0x2228`。
  `memory_test_buffer`は`0x30000670`、0x80 B。LEDのstack／entryシンボルなし。
- `objdump`：register_access_readは単一の32 bit `ldr`、register_access_writeは単一の32 bit `str`。
  ベースは0x50096000。writeにはread-modify-writeなし。
- `runners.yaml`：LinkServer、MCXN947:FRDM-MCXN947、cm33_core0、Flash 0x10000000。

## QEMUのコマンド検証

```powershell
west build -b qemu_cortex_m3 stages/09_register_access/tests/commands `
    -d .tmp/build/09_register_commands_codex -- `
    -DUSER_CACHE_DIR="$learningRepo/.tmp/cache"
$env:Path = 'C:\Program Files\Git\mingw64\bin;' + $env:Path
west build -d .tmp/build/09_register_commands_codex -t run
# 成功表示後にCtrl+A、XでQEMUを終了する。
```

初回は同じDTC制限で失敗。制限外で再実行し、140/140リンク完了、ビルド終了コード0。
`TESTSUITE stage9_commands succeeded`、`pass = 9, fail = 0, skip = 0`、
`PROJECT EXECUTION SUCCESSFUL`を確認した。Ctrl+A、Xで正常終了、run終了コード0。
GitのDLLパス追加は実行プロセスだけの設定。

本物のcommands.c、register_commands.c、memory_commands.c、上流ntopt.cをリンク。
UART・MMIOをテスト用に置換し、許可外MMIOの呼出しが0回であることと、
セット／クリア／反転のモデルの変化・他ビットの保持を検査する。
QEMUはMCXN947のGPIOモデルではないため、実機レジスタの動作確認とは扱わない。

| テスト | 結果・検証対象 |
|---|---|
| test_info_help_and_empty_do_not_touch_mmio | PASS：表示・空行だけではMMIOしない |
| test_read_allowlist | PASS：許可3レジスタの読み出しと表示 |
| test_write_side_effects_preserve_other_pins | PASS：単一alias書き込み・PDOR検証・他ビット保持 |
| test_zero_is_noop_and_repeated_set_clear_are_idempotent | PASS：0は不変、繰り返しset／clearは同じ状態 |
| test_denied_addresses_directions_and_bits_never_access_mmio | PASS：別名・不整列・ISFR・RO書込・alias読出・他bit等17件でMMIOなし |
| test_bad_syntax_and_numbers_never_access_mmio | PASS：引数・幅・符号・不正桁・桁あふれ・旧LEDコマンド等30件でMMIOなし |
| test_fault_and_full_line_block_write_and_recover | PASS：受信異常／63 Bで書込なし、62 Bで正常復帰 |
| test_failed_latch_check_has_no_success_reply | PASS：ラッチ不一致にERROR、OKなし |
| test_ram_remains_separate_from_register_access | PASS：RAM操作を継続し、mem／regが互いの領域へ入らない |

## 本人の後続結果と学習範囲の見直し

以上のビルド・テストはCodexの事前検証として保持する。
その後の本人のビルド・書き込み・起動・レジスタ操作・不正入力の確認は[user-evidence](../user-evidence/README.md)に別の実施者として記録した。
2026-11-07に本人がZephyrの設定・デバイス・ドライバAPI・スレッド／キューを今後の学習の軸として明示した。
GDB照合は本人の依頼で今回の必須項目から除外する。実施済みには変更しない。
レジスタ資料・テストは補助として保持し、Cの全行理解をZephyr学習の進行条件にしない。
本人のREADME確認は2026-11-08に完了し、コミット・プッシュ・PR作成を依頼された。
その後、2026-11-08に本人からPRのマージを明示的に依頼され、PR #27をmainへマージした（a198abd）。
続いて本人の明示的な依頼でイシュー #10をcompletedとしてクローズした。段階9は完了。追加ビルド・実機操作は行っていない。
