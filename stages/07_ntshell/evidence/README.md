# 段階7：Codexの事前検証

実施日：2026-10-30（JST）。本人の実習と区別する。
実機の接続先列挙・UART操作・書き込みは行っていない。

## 外部OSSと採用環境

- 開始時の学習リポジトリ：変更なしmain、`90b2eae`。Codexが`codex/stage7-ntshell`を作成した。
- 採用ZephyrのローカルHEAD：`dccb09599635bdff17633fa7e9dab014b91dce90`。開始時の`git status --short`は出力なし。
- NT-Shell公式配布パッケージ0.3.1を取得。[固定情報](../UPSTREAM.md)とSHA256SUMSを保存した。
- `python stages/07_ntshell/scripts/verify_vendor.py --archive .tmp/ntshell-v0.3.1.tar.gz`：終了コード0、16ファイルすべて公式アーカイブ・ハッシュと一致。

## CPU0向けビルド

```powershell
. .\stages\01_hello_world\scripts\env.ps1
west build -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\07_ntshell" `
    -d "$learningRepo\.tmp\build\07_ntshell_codex" -- `
    -U BUILD_VERSION -U CMAKE_CACHEFILE_DIR `
    -DUSER_CACHE_DIR="$learningRepo/.tmp/cache"
```

164/164のリンク完了、終了コード0。Zephyr 4.4.2、SDK 1.0.1、GCC 14.3.0、
Python 3.12.10、west 1.5.0、CMake 3.31.8、DTC 1.6.1をビルド表示で確認した。
出力は本人用とは別のリポジトリ内`.tmp/build/07_ntshell_codex`。

最初の制限実行はDTC呼出し後に進まなくなり中断した。
その試行だけで指定したBUILD_VERSION・CMAKE_CACHEFILE_DIRを解除し、キャッシュを作業領域へ指定して通常権限で再実行したところ成功した。
中断をアプリのコンパイルエラーや本人の環境不良とは扱わない。

### 生成設定とDevicetree

- CMakeCache：段階7アプリ、CPU0ボード、採用Zephyr・SDKのコンパイラとvenvのPythonを確認。
- `.config`：SERIAL、UART_INTERRUPT_DRIVEN、UART_MCUX_LPUART、GPIOは有効。
- `.config`：CONSOLE、CONSOLE_SUBSYS、SHELL、PRINTK、LOG、BOOT_BANNER、UART_ASYNC_APIは無効。
- main優先度6、スタック4096 B。LED優先度5、要求スタック2048 Bはソースで確認。
- 生成Devicetree：`zephyr,console`はflexcomm4_lpuart4、nxp,lpuart、115200、okay。
- `led0`はred_led、GPIO0のピン10、Active Low。
- map：`_flash_used=0x7a90`、`_image_ram_size=0x2a70`。

| 対象 | ビルド時使用量 | マイコン全体 | 全体に対する使用率 |
|---|---:|---:|---:|
| 内蔵Flash | 31,376 B | 2,097,152 B | 1.50% |
| 内蔵SRAM | 10,864 B | 524,288 B | 2.07% |

RAM 10,864 B＋SRAMX 0 B。IDT_LISTも0 Bで、重複加算しない。
CPU0に割り当てた320 KiBに対するRAM使用率は3.32%。
実行時最大スタック・ヒープ、割り込み遅延、周期は未測定。

## QEMUによるコマンド解析テスト

本物の`commands.c`と上流`ntopt.c`を使い、UART出力・LED操作・受信欠落フラグをテスト用に置き換えた。
テスト用のconsole／printk設定とQEMUのメモリ使用量は、実機アプリとは別である。

```powershell
west build -b qemu_cortex_m3 `
    "$learningRepo\stages\07_ntshell\tests\commands" `
    -d "$learningRepo\.tmp\build\07_ntshell_commands_codex" -- `
    -DUSER_CACHE_DIR="$learningRepo/.tmp/cache"

# このPowerShellプロセスだけへ、既存GitのGCCランタイムDLLの場所を追加。
$env:Path = 'C:\Program Files\Git\mingw64\bin;' + $env:Path
west build -d "$learningRepo\.tmp\build\07_ntshell_commands_codex" -t run
```

138/138のテスト用リンク完了、ビルド終了コード0。
QEMUは`TESTSUITE stage7_commands succeeded`、**pass=7、fail=0、skip=0**、
`PROJECT EXECUTION SUCCESSFUL`を表示した。[実行ログ](command-tests.log)を保存した。

確認した内容：

1. helpと空行／空白。
2. on・off・blink、既定500 ms、100／2000 msの境界。
3. 未知コマンド、余分な引数、負数・単位付き・範囲外・巨大整数でLED依頼が発生しない。
4. 62 Bは受理、63 Bの満杯行は有効な先頭があっても拒否。多数の引数。
5. 受信欠落フラグがある間はLEDコマンドを実行しない。
6. GPIO失敗時にOKを表示しない。
7. 点滅中のGPIO失敗を次のコマンド時に通知する。

最初のQEMU起動は`0xc0000135`で失敗した。PE依存を調べ、SDKのlibjpeg／libpixmanが必要とする
`libgcc_s_seh-1.dll`を既存Git配下に見つけ、実行プロセスのPATHだけへ追加した。
追加インストール・SDK変更・PC全体のPATH変更は行っていない。

テスト後にQEMUが待機したため、成功表示を確認後、専用qemu.pidとプロセス名を照合して終了した。
そのためログ末尾のrunターゲットには強制終了由来のFAILEDがある。
**テスト7件の成功とrunラッパーの終了コードは区別し、run全体が終了コード0だったとは扱わない。**
手元の対話端末から実行する場合、成功表示後はQEMU案内のCtrl+A、Xで終了できる。

## 残る確認

- NT-Shell本体の編集・履歴・表示、UARTのISR／FIFO／改行の経路とLEDスレッドは、このコマンド単体テストの対象外。
- UARTエラー・キュー満杯時のCAN／Ctrl+Cによる取消しと再入力は、ソースを上流VT遷移表と照合した。実機では未確認。
- 本人のビルド・書き込みと、全LEDコマンド、操作待ち／入力途中の点滅継続、編集キー、不正入力の実機確認が必要。
- イシュー#8の学習完了や本人の理解を、自動テストの成功だけでは完了にしない。
