# 段階8：本人の実習記録

## 環境確認（2026-10-31受領、JST）

本人がPowerShellで実行したコマンドと表示結果をチャットで受領した。

| 項目 | 本人の表示結果 |
|---|---|
| ZEPHYR_BASE | `C:\Users\grace\zephyrproject-mcxn947\zephyr` |
| west | `West version: v1.5.0` |
| Zephyr HEAD | `dccb09599635bdff17633fa7e9dab014b91dce90` |
| 学習リポジトリのブランチ | `codex/stage8-memory-ram` |

すべて採用値と一致。環境スクリプトのドットソース、ZEPHYR_BASEへの移動、
`$memoryBuild = Join-Path $zephyrWorkspace 'build\08_memory_ram'`も提供コマンドに含まれる。
受領日を記録し、本人の操作した正確な日時や他ツールの版の実測とは区別する。

## main.cの確認（2026-10-31受領、JST）

`main.c`の表示手順と、追加した`memory_commands_init()`・`memory_commands_info()`、
初期化・範囲表示から受信開始・シェル実行への流れを案内した。
本人から「確認完了しました」との報告を受領し、main.cの確認完了として記録した。
本人による再説明や実行時の観察結果とは区別する。

## commands.cの確認（2026-10-31受領、JST）

受信異常・行長の検査、`ntopt_parse()`による単語分割、`dispatch()`から
`memory_commands_execute()`への振り分けと、`memory_commands_usage()`によるヘルプ追加を案内した。
本人から「確認完了しました」との報告を受領し、commands.cの確認完了として記録した。
本人による再説明や実行時の観察結果とは区別する。

## memory_commands.cのRAM確保・初期化・範囲表示（2026-10-31受領、JST）

128 Bの`memory_test_buffer`、同じ領域をバイト単位で参照する`ram_bytes()`、
`memory_commands_init()`の`00..7f`初期化、`memory_commands_info()`の終端を含まない範囲表示を案内した。
本人から「確認完了しました」との報告を受領し、この部分の確認完了として記録した。
同ファイル全体の確認や、本人による再説明・実機動作の確認とは区別する。

## memory_commands.cの数値解析・アクセス幅・範囲検査（2026-10-31受領、JST）

`parse_number()`の整数変換と計算前の上限検査、`parse_width()`のbitからバイト数への変換、
`checked_offset()`の範囲・アラインメント検査と先頭からのoffset計算を案内した。
本人から「確認完了しました」との報告を受領し、この3関数の確認完了として記録した。
本人による再説明や、不正入力の実機での拒否確認とは区別する。

## memory_commands.cの転送・表示・コマンド実行処理（2026-10-31受領、JST）

`read_value()`・`write_value()`・`dump_bytes()`・`memory_commands_execute()`を案内した。
16／32 bitのmemcpy転送、リトルエンディアンのバイト順、検査後のアクセス、書き込み後の読み戻し表示を説明した。
本人から「確認完了しました」との報告を受領し、この4関数の確認完了として記録した。
本人による再説明や、実機の読み書き・ダンプの一致確認とは区別する。

## CMake・設定表示と外部OSS照合（2026-10-31受領、JST）

本人から`Get-Content`によるCMakeLists.txtとprj.confの全文表示を受領した。
`src/memory_commands.c`を含むソース一覧、ヘッダ検索先、UART割り込み・GPIO有効、
標準Console／Shell・PRINTK・LOG・BOOT_BANNER等の無効化、main優先度6・スタック4096 Bが想定と一致した。

本人が実行した`python "$learningRepo\stages\08_memory_ram\scripts\verify_vendor.py"`の出力：

```text
PASS: 16 NT-Shell files match checksums
```

直後の`$LASTEXITCODE`は`0`。保存済みチェックサムとの一致を確認した。
今回の本人による公式配布アーカイブとの照合、ライセンス本文・UPSTREAM本文の表示確認とは区別する。
Kconfigの既定値・selectに関する質問と回答は[QA-046・QA-047](../../../docs/qa.md)に記録した。

## CPU0向けビルド（2026-10-31受領、JST）

本人からビルド末尾の表示と、直後の`$LASTEXITCODE`の結果を受領した。

- Zephyr 4.4.2、build v4.4.2。
- `[165/165] Linking C executable zephyr\zephyr.elf`。
- 生成先：`C:/Users/grace/zephyrproject-mcxn947/build/08_memory_ram/zephyr/zephyr.elf`。
- ボード：`frdm_mcxn947/mcxn947/cpu0`。
- Flash 34,200 B／2 MiB（1.63%）、RAM 10,992 B／320 KiB（3.35%）。
- SRAMX 0 B、IDT_LIST 0 B。
- `$LASTEXITCODE`：`0`。

本人のビルド成功を確認。Codexの別出力先の事前ビルドとは区別する。
マイコン全SRAM 512 KiBを分母にするとRAMは2.10%。ビルド時使用量であり実行時最大使用量は未測定。

Codexは本人用CMakeCache・.config・mapを読み取り、対象アプリ・CPU0・設定と使用量の一致を確認した。
このCodexによる照合を、本人による生成設定の表示確認としては扱わない。

## 生成.configの表示確認（2026-10-31受領、JST）

本人が`Select-String`で表示した段階8の生成.configの結果を受領した。

| 項目 | 本人の表示結果 |
|---|---|
| MAIN_STACK_SIZE | `4096` |
| SERIAL・UART_INTERRUPT_DRIVEN・UART_MCUX_LPUART・GPIO・LITTLE_ENDIAN | すべて`y` |
| LOG・CONSOLE・CONSOLE_SUBSYS・PRINTK・SHELL・BOOT_BANNER | すべて`# CONFIG_… is not set` |

本人用`build/08_memory_ram/zephyr/.config`の表示で、案内した設定すべての一致を確認した。
Codexによる先行の生成物読み取り照合と、今回の本人の表示結果を区別する。

## 今回の接続先の列挙（2026-10-31受領、JST）

本人から次のコマンドの表示結果を受領した。

- `Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name`：COM5、MCU-Link VCom Port (COM5)。
- `LinkServer probes --no-boot`：プローブ1番、MCU-LINK FRDM-MCXN947 (r0E7)、CMSIS-DAP V3.128、シリアルHN2APUBRDCEPT。

今回の書き込み対象のMCU-LinkとUART接続先を確認した。前段階の記録の流用とは区別する。

## Flash書き込み（2026-10-31受領、JST）

本人から成功表示・終了コード0の報告と、次の末尾抜粋を受領した。

```text
Pb: (100) Finished writing Flash successfully.
Nt: Loaded 0x8598 bytes in 311ms (about 109kB/s)
Nt: Reset target (system)
Nc: Starting execution using system reset with a stall address
Nc: Retask read watchpoint 1 at 0x50000040 to use for boot ROM stall
```

直後の`$LASTEXITCODE`は`0`。Flash書き込み成功を確認した。
ロード量0x8598は34,200 Bで本人のビルド結果と一致する。
提供されたのは末尾抜粋であり、全ログ・消去セクタ数・補助ブート処理の有無は未確認。

## UART起動・端末設定・初期LED（2026-10-31受領、JST）

本人から次の起動表示を受領した。

```text
Stage 8: NT-Shell RAM commands
NT-Shell package 0.3.1; local echo OFF; type help.
LED starts OFF; blink default toggle interval=500 ms.
RAM: [0x30000730, 0x300007b0) 128 bytes; end exclusive
read/write: 8/16/32 bit, aligned; dump: 1..64 bytes.
mcxn947>
```

起動表示は期待値と一致。RAM範囲は128 Bで、有効なバイトアドレスの最終位置は0x300007af。
本人は「Tera Termの設定を合わせ、赤色LEDが消灯していることも確認しました」と報告した。
案内した設定はCOM5／115200／8N1／フロー制御なし／ローカルエコーOFF／送信CR+LF・受信CR。
設定とLEDは本人の確認報告に基づき、設定画面や目視映像自体の提供とは区別する。

## help・範囲表示・初期パターンの読み出し（2026-10-31受領、JST）

本人からhelp、mem info、dump、3幅のreadの実行UARTを受領した。
helpにはLEDとRAMのコマンド・引数仕様が表示され、mem infoは起動時と同じ範囲だった。

| 本人の入力 | 提供出力で確認した内容 |
|---|---|
| `mem info` | `[0x30000730, 0x300007b0)`、128 B、終端を含まない |
| `mem dump 0x30000730 16` | `00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f` |
| `mem read 0x30000730 8` | `R8 0x30000730 = 0x00` |
| `mem read 0x30000730 16` | `R16 0x30000730 = 0x0100` |
| `mem read 0x30000730 32` | `R32 0x30000730 = 0x03020100` |

初期パターンと、同じ位置を8／16／32 bitで読んだ値がすべて期待値と一致した。

## 各幅の書き込み・読み戻し・dumpとLED継続（2026-10-31受領、JST）

本人からled blink 500、各幅のwrite／read、最後のdumpの実行UARTを受領した。

| 入力 | 提供UARTの結果 |
|---|---|
| `led blink 500` | `OK: led blink` |
| `mem write 0x30000730 32 0x12345678` | `W32 0x30000730 = 0x12345678` |
| `mem read 0x30000730 32` | `R32 0x30000730 = 0x12345678` |
| `mem write 0x30000734 16 0xabcd` | `W16 0x30000734 = 0xabcd` |
| `mem read 0x30000734 16` | `R16 0x30000734 = 0xabcd` |
| `mem write 0x30000736 8 255` | `W8 0x30000736 = 0xff` |
| `mem read 0x30000736 8` | `R8 0x30000736 = 0xff` |
| `mem dump 0x30000730 8` | `0x30000730: 78 56 34 12 cd ab ff 07` |

すべて期待値に一致。本人から入力待ち中・操作中もLED点滅が続いたとの報告を受領した。
約10秒の無入力待機を案内したが正確な経過時間は未取得。目視での継続確認であり、周期・遅延・個別dump処理時間中の測定は未実施。
現時点のLED設定はblink 500、先頭8 Bは既知パターン78 56 34 12 cd ab ff 07。

## 各幅の最終有効位置・最大値・末尾dump（2026-10-31受領、JST）

本人から各操作のUARTを受領し、すべて期待値に一致すると確認した。

| 操作した位置と幅 | 書き込み値 | write／readの表示値 |
|---|---|---|
| 0x300007af、8 bit | 255 | 0xff |
| 0x300007ae、16 bit | 65535 | 0xffff |
| 0x300007ac、32 bit | 0xffffffff | 0xffffffff |

`mem dump 0x300007af 1`は`0x300007af: ff`、
`mem dump 0x300007ac 4`は`0x300007ac: ff ff ff ff`。
各幅のアクセスが終端直前まで収まり、最大値が受理されることを実機で確認した。
後の広い幅の書き込みが先の狭い幅の位置を含むため、末尾4 Bの最終内容はff ff ff ff。

## 範囲外・終端超過・不整列の拒否と復帰（2026-10-31受領、JST）

本人から次の9件のUARTを受領した。

| 入力 | 提供UARTのERROR内容 |
|---|---|
| `mem read 0x300007b0 8` | outside test RAM range |
| `mem write 0x300007b0 8 0` | outside test RAM range |
| `mem dump 0x300007b0 1` | outside test RAM range |
| `mem read 0x3000072f 8` | outside test RAM range |
| `mem dump 0x300007af 2` | outside test RAM range |
| `mem dump 0x30000771 64` | outside test RAM range |
| `mem read 0x30000731 16` | unaligned address |
| `mem write 0x30000732 32 0` | unaligned address |
| `mem read 0xffffffff 32` | outside test RAM range |

各拒否後にプロンプトへ復帰していた。
前後の`mem dump 0x30000730 16`は両方とも次の内容だった。

```text
0x30000730: 78 56 34 12 cd ab ff 07 08 09 0a 0b 0c 0d 0e 0f
```

最後の`mem read 0x30000730 32`も`R32 0x30000730 = 0x12345678`で、正常な応答を確認した。
不変を照合したのは先頭16 Bであり、RAM全体の不変の実測ではない。
今回の拒否試験中のLEDについての追加報告は未受領（前段の通常操作中の点滅継続は確認済み）。

## ダンプ長上限・不正引数11件と復帰（2026-10-31受領、JST）

本人の`mem dump 0x30000730 64`のUART：

```text
0x30000730: 78 56 34 12 cd ab ff 07 08 09 0a 0b 0c 0d 0e 0f
0x30000740: 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
0x30000750: 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
0x30000760: 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
```

64 Bが4行で表示された。
続く以下の11件すべてで`ERROR: invalid memory command or argument`、Memory usageの全項目、プロンプトへの復帰を提供UARTで確認した。

- dump長：`0`と`65`。
- read幅：`64`。
- writeの値：8 bitで`256`、16 bitで`65536`、32 bitで`4294967296`。
- 8 bit writeの符号・表記：`-1`、`+1`、`0x`、`0xgg`。
- 余分な引数：`mem read 0x30000730 32 extra`。

最後の先頭16 B dumpは`78 56 34 12 cd ab ff 07 08 09 0a 0b 0c 0d 0e 0f`のままで、
readも`R32 0x30000730 = 0x12345678`。正常コマンドの応答と比較範囲内の不変を確認した。
今回の不正引数試験中のLEDについての追加報告は未受領（通常操作中の継続は確認済み）。

## RESET後の再初期化・LED消灯（2026-10-31受領、JST）

本人からRESET後の起動UART、mem infoと次の実行結果を受領した。

- 起動表示・mem infoともRAM範囲は`[0x30000730, 0x300007b0)`、128 B。
- `mem dump 0x30000730 16`：`00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f`。
- `mem read 0x30000730 32`：`R32 0x30000730 = 0x03020100`。
- `mem dump 0x300007ac 4`：`0x300007ac: 7c 7d 7e 7f`。
- 本人の目視報告：「LEDは消灯しています。」

前の試験で書き換えた先頭と末尾の両方が初期パターンへ戻り、LEDも消灯したことを確認した。
全128 Bを再度dumpした結果ではなく、提供された先頭16 Bと末尾4 B等の照合に基づく。

## GDB接続・ELFロード・ページ送り（2026-10-31受領、JST）

本人の`west debug -d $memoryBuild -r linkserver --probe HN2APUBRDCEPT`のログを受領した。

- rebuildは`ninja: no work to do.`。
- GNU gdb (Zephyr SDK 1.0.1) 16.2、LinkServer 1.4.85。
- プローブHN2APUBRDCEPT、対象MCXN947:FRDM-MCXN947、core cm33_core0へ接続。
- シンボル元は本人用`build/08_memory_ram/zephyr/zephyr.elf`。
- `Start address 0x10002370, load size 34200`の後、(gdb)へ到達。
- 本人の`set pagination off`入力も表示された。

本人が(gdb)表示まで2回Enterを押したと報告した。
ログには`Type <RET> for more, q to quit, c to continue without paging`が2回あり、出力のページ送りである。
補助プローブブートのFailed to boot probes／UnicodeDecodeErrorの後にプローブ選択・接続は成功している。
初回resetには`No image address available for soft reset`が表示されたが、その後のELFロードは完了した。
FW更新は候補案内のみで、更新実行の表示はない。

## GDBロード後のリセット・実行再開（2026-10-31受領、JST）

本人から「monitor resetでリセット成功の表示を確認し、continueを実行しました。
Tera Termにも段階8の起動表示が出ました」との報告を受領した。
ロード後のリセット・実行再開を本人の確認報告に基づき確認した。
この操作のGDB応答の原文自体は未受領。再起動UARTは次の確認時に受領した。

## GDB照合用の既知パターン再書き込み（2026-10-31受領、JST）

本人から再起動UART、mem info、次のwrite／read／dumpの実行UARTを受領した。

- 再起動表示は段階8、範囲は`[0x30000730, 0x300007b0)`、128 B。
- `mem write 0x30000730 32 0x12345678`：W32の値0x12345678。
- `mem write 0x30000734 16 0xabcd`：W16の値0xabcd。
- `mem write 0x30000736 8 255`：W8の値0xff。
- 各位置を同じ幅でreadし、R32=0x12345678・R16=0xabcd・R8=0xff。
- `mem dump 0x30000730 8`：`0x30000730: 78 56 34 12 cd ab ff 07`。

GDB接続・リセット後の同じ実行で、照合用の既知パターンを再設定できたことを確認した。

## GDBでのCPU停止・RAM照合（2026-10-31受領、JST）

本人のCtrl+C操作後のGDB出力を受領した。

```text
Program received signal SIGINT, Interrupt.
arch_cpu_idle () at C:/Users/grace/zephyrproject-mcxn947/zephyr/arch/arm/core/cortex_m/cpu_idle.c:104
104             __enable_irq();
```

| 本人のGDBコマンド | 提供出力 |
|---|---|
| `print/x &memory_test_buffer` | `$1 = 0x30000730` |
| `x/8bx &memory_test_buffer` | `0x78 0x56 0x34 0x12 0xcd 0xab 0xff 0x07` |
| `x/2hx &memory_test_buffer` | `0x5678 0x1234` |
| `x/1wx &memory_test_buffer` | `0x12345678` |

すべて期待値と一致し、直前の同じ実行でのシェルの範囲・書き込み・read・dumpと照合できた。
SIGINTは案内したCtrl+Cによる停止。停止位置はその時点のidle処理であり、異常終了の報告ではない。

## GDB終了後の無応答（2026-10-31受領、JST）

本人から、GDB終了後に`led off`と`mem read 0x30000730 32`へ応答しないとの報告と、RESETしてよいかとの質問を受領した。
この報告時点ではGDBのmonitor・detach・quitの各応答と終了コードは未受領だった。切断後の自動再開は成功扱いにせず、原因は未確定とする。
GDBでの既知パターン照合はすでに実機で確認済みで、この終了後の無応答とは区別する。
RESETによる復旧確認を案内した。RESET後の先頭32 bitの期待値は0x03020100、LEDは消灯。
復旧結果と最終状態は未受領。

## GDB終了ログの追加確認（2026-10-31受領、JST）

本人から次の終了ログを受領した。

```text
(gdb) monitor ondisconnect cont
(gdb) detach
Detaching from program: C:\Users\grace\zephyrproject-mcxn947\build\08_memory_ram\zephyr\zephyr.elf, Remote target
Ending remote debugging.
[Inferior 1 (Remote target) detached]
(gdb) INFO: Disconnected from core cm33_core0
Gdbserver on port 3333 has closed
quit
```

その後PowerShellへ戻り、本人の`$LASTEXITCODE`は`0`。
monitor入力に明示的なエラーはなく、detach・GDB server終了・GDB終了は提供ログで確認できた。
この終了コードはCPUの実行継続・UART応答を証明しない。無応答の原因はまだ未確定。
RESET後の復旧は次の提供結果で確認した。

## GDB終了後のRESETによる復旧・最終状態（2026-10-31受領、JST）

本人から応答が戻り赤色LEDが消灯しているとの報告と、RESET後のUARTを受領した。
今回の貼付はNT-Shell package行から始まり、RAM範囲は同じ`[0x30000730, 0x300007b0)`だった。

- `help`：LEDとRAMの使用方法の全項目を表示。
- `led off`：`OK: led off`。
- `mem read 0x30000730 32`：`R32 0x30000730 = 0x03020100`。
- 赤色LED：本人が消灯を確認したと報告。

RESETでシェル応答が復旧し、先頭値は起動時の初期値へ戻った。最終状態はLED消灯。
GDBは終了コード0で終了済み。GDBでの既知パターン照合は成功しているが、切断直後の自動再開・UART応答は成功扱いにしない。
無応答の原因や再現性は未確定で、終了後のRESET復旧を含む今回の結果として記録する。

## READMEの本人確認・PR作成依頼（2026-11-01受領、JST）

本人から「READMEを確認しました。実際の実習内容と一致しています。PRを作成してください」との報告を受領した。
READMEの本人確認を完了として記録した。
依頼に基づきCodexが実装と記録をコミット2ae5e69としてプッシュし、2026-11-01に[PR #26](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/26)を作成した。
PR作成時点では本人のPR確認・マージ・クローズ承認は未受領だった。後続の承認と完了は以下に記録する。

## PR確認・マージ・イシュー完了（2026-11-01受領・実施、JST）

本人から「PR #26の内容を確認しました。問題ありません。マージして、イシュー #9をクローズしてください」との報告・依頼を受領した。
CodexがPRのOPEN・マージ可能状態と、本人が確認したheadから変更がないことを確認した。

- 確認済みhead：`6c6918654bc72b12e56b6db4bab0a94ab94b5bac`。
- マージ方法：merge。expected headを指定して実行し、merged=trueを確認。
- マージコミット：`d7b4b8eafdbed3d970a7ce173b26bf7010c5d4db`。
- イシュー #9：作業内容4件・完了条件4件の計8チェック項目を完了に更新し、closed／completedを確認。
- ローカルmain：origin/mainへfast-forwardで同期。

段階8の本人実習・README・PR確認と、承認に基づくマージ・イシュー完了を記録した。
最終実機状態は前節のLED消灯・RESET後のRAM初期パターン・GDB終了済みのまま。今回、ボードへの追加操作は行っていない。

Codexによる事前検証は[別記録](../evidence/README.md)とする。
