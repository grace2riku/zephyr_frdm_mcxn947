# 段階8：NT-Shellで専用RAMを読み書き・ダンプする

対象：[イシュー #9](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/9)。前提：[段階7](../07_ntshell/README.md)。
開始日：2026-10-31（JST）。作業ブランチ：`codex/stage8-memory-ram`。
成果物：[PR #26](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/26)（2026-11-01作成、本人のPR確認待ち）。

## 目的と現在の状態

アドレス、アクセス幅、アラインメント、範囲チェック、数値解析、バイト順を学ぶ。
段階7のNT-Shell・UART・LED機能を引き継ぎ、専用RAMへのmem read・mem write・mem dumpを追加した。
開始時は変更なしmain、3e17fb2。Codexがイシュー #9のOPENを確認し、作業ブランチを作成した。

2026-10-31（JST）、本人の環境・ソース・設定・外部OSSのチェックサム照合、CPU0向けビルド・書き込み、
全幅のRAM読み書き・ダンプ、末尾境界・最大値、範囲外／不整列／不正引数の拒否と正常復帰、RESETでの初期化、GDBとの照合まで確認した。
入力待ち・通常RAM操作中のLED点滅継続も本人の目視報告で確認済み。
ビルド使用量はFlash 34,200 B、RAM 10,992 B。本人の実習と[Codexの事前検証](evidence/README.md)は区別する。

GDBの切断・終了は終了コード0だったが、その直後のUARTは無応答となった。
本人のRESETでhelp・LED操作・RAM読み出しが復旧した。原因は未確定で、自動再開の成功とは扱わない。
最終状態はLED消灯、専用RAMはRESET後の初期パターン（先頭R32=0x03020100）、GDB終了済み。

| イシュー #9の完了条件 | 現在の根拠 |
|---|---|
| 専用RAMのwrite・read・dumpが一致する | 3幅の既知値とバイト列をUART・GDBで確認 |
| 範囲外・不整列・不正引数を拒否する | 範囲外等7件、不整列2件、不正引数11件と正常復帰を提供UARTで確認 |
| 実装幅と表示形式を明記する | 下記の仕様とhelpに8／16／32 bit・表示形式・ダンプ長を記載 |
| バージョン・手順・期待動作・結果・未確認事項をREADMEへ残す | 本READMEと証跡に記録済み。2026-11-01に本人が実際の実習内容と一致すると確認 |

本人がREADMEを確認し、実際の実習内容と一致すると報告してPR作成を依頼した（2026-11-01受領）。
Codexが実装と記録をコミット2ae5e69としてプッシュし、[PR #26](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/26)を作成した（2026-11-01）。
**本人のPR確認、マージとイシューのクローズは未実施。**
イシュー #9はOPEN。本人の明示的な承認後にマージとクローズを行う。
詳細な実行結果は[user-evidence/README.md](user-evidence/README.md)に保存した。

## 学習に使用したAIモデル

| 利用期間（JST） | AIツール | AIモデル（モデルID） | AIモデルバージョン | 推論設定 | 確認根拠 |
|---|---|---|---|---|---|
| 2026-10-31〜 | OpenAI Codex（デスクトップ） | 未確認 | 未確認 | 未確認 | 本段階の保存済み設定は未照合。過去段階のモデル情報を流用しない |

[記録方針](../../docs/ai-models.md)に従う。

## 使用環境

| 項目 | 採用値／今回の確認範囲 |
|---|---|
| ボード | FRDM-MCXN947、`frdm_mcxn947/mcxn947/cpu0` |
| Zephyr | 4.4.2、`dccb09599635bdff17633fa7e9dab014b91dce90`。CodexがローカルHEADと変更なしを確認 |
| SDK／Python／west／CMake／DTC | 1.0.1／3.12.10／1.5.0／3.31.8／1.6.1を継続。本人のwest 1.5.0は今回の表示で確認済み。他ツールの今回の本人による実測は未実施 |
| NT-Shell | 公式配布0.3.1。取得元・SHA・ライセンスは[UPSTREAM.md](UPSTREAM.md) |
| UART／USB | J17のMCU-Linkを継続する方針。115200、8N1、フロー制御なし、ローカルエコーOFF、送信CR+LF・受信CR |
| 接続先 | 本人の今回の列挙でCOM5／MCU-Link VCom Port、MCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTを確認（2026-10-31受領） |

## コマンドとRAMの仕様

```text
help
led on
led off
led blink [100..2000]
mem info
mem read <0xADDRESS> <8|16|32>
mem write <0xADDRESS> <8|16|32> <VALUE>
mem dump <0xADDRESS> <LENGTH:1..64>
```

- 専用RAMは`memory_test_buffer`の128 B。4 B境界に置いた`uint32_t[32]`。
- 起動時にバイト列`00 01 02 ... 7f`で初期化する。書き込み値はRESETで初期パターンに戻る。
- 起動表示と`mem info`に`[先頭, 終端)`を表示する。終端アドレスそのものは範囲外。
- アドレスは`0x`または`0X`付き16進数。値・ダンプ長は10進数または同じ接頭辞の16進数。
- 幅は文字どおり`8`・`16`・`32`（bit）。転送量は1・2・4 B。アドレスはそれぞれ1・2・4 Bの倍数。
- 値は符号なし。8 bitは0..255、16 bitは0..65535、32 bitは0..4294967295。超過を切り捨てず拒否する。
- read／writeは1値を扱い、`R8`／`W16`／`R32`等、アドレス、2／4／8桁の16進値を表示する。
- dumpは1..64 B、1行16 B、各バイトを2桁の16進数で表示。バイト境界なら開始でき、末尾の短い行も表示する。
- 専用RAM以外、終端をまたぐアクセス、幅に合わないアドレス、不正な桁・符号・小数・余分な引数を拒否する。
- 数値の乗算前に桁あふれを調べ、アドレスは整数のまま検査する。`address + length`での桁あふれを避け、範囲確認後だけバッファ内の位置へアクセスする。

CPU0はリトルエンディアン。先頭へ32 bitの`0x12345678`を書くと、dumpでは`78 56 34 12`となる。
8／16／32 bitは論理的な転送幅であり、単一のCPU命令を保証する仕様ではない。
16／32 bitは`memcpy`でオブジェクト表現を転送し、型の別名参照による未定義動作を避ける。
mainスレッドだけがRAMを操作し、割り込みやLEDスレッドは変更しないため、`volatile`や排他ロックは不要。
レジスタ操作は[段階9](../../docs/roadmap.md)で扱う。

入力行は段階7と同じ最大62 B。63 Bの満杯行は解析前に拒否する。
UARTエラー／受信キュー満杯を検出した行も実行せず、取消し後に再入力する。
これらの実機経路の確認と、コマンドの自動テストは区別する。

## ソースを読む順番

1. `src/main.c`：UART／LED初期化→RAM初期化→NT-Shell初期化→範囲表示→受信開始→シェル実行。
2. `src/commands.c`：受信異常・行長検査→`ntopt_parse`→`dispatch`→`mem`かLEDへ振り分け。
3. `src/memory_commands.c`：数値・幅解析→範囲・アラインメント検査→転送／表示。
4. `CMakeLists.txt`と`prj.conf`：新ソースの組み込み、UART割り込み・GPIO有効、標準Shell等の受信競合を無効化。
5. `src/serial_port.c`、`src/led_control.c`：段階7から継続する受信とLED処理。

## 本人の操作手順

一度に全部を実行せず、各手順の結果を確認して次へ進む。以下の出力例は期待値であり実測ログではない。

### 1. 環境とブランチの確認（本人確認済み）

新しいPowerShellで実行する。このPowerShellだけへPATH・変数を設定する。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
Set-Location $env:ZEPHYR_BASE
$memoryBuild = Join-Path $zephyrWorkspace 'build\08_memory_ram'
$env:ZEPHYR_BASE
west --version
git rev-parse HEAD
git -C $learningRepo branch --show-current
```

期待値：`C:\Users\grace\zephyrproject-mcxn947\zephyr`、west 1.5.0、上記Zephyr SHA、`codex/stage8-memory-ram`。

本人から4項目の表示結果を受領し、すべて一致すると確認した（2026-10-31受領）。
環境スクリプトの読み込みと`$memoryBuild`の代入も提供されたコマンドに含まれる。
本人によるビルド・実機動作や、SDK・Python等の個別の版確認とは区別する。

### 2. ソースと外部OSSの確認

上記の順番でソースを読み、初期パターン、境界と拒否条件を確認する。

```powershell
Get-Content "$learningRepo\stages\08_memory_ram\src\main.c"
Get-Content "$learningRepo\stages\08_memory_ram\src\commands.c"
Get-Content "$learningRepo\stages\08_memory_ram\src\memory_commands.c"
Get-Content "$learningRepo\stages\08_memory_ram\CMakeLists.txt"
Get-Content "$learningRepo\stages\08_memory_ram\prj.conf"
python "$learningRepo\stages\08_memory_ram\scripts\verify_vendor.py"
$LASTEXITCODE
```

期待値：`PASS: 16 NT-Shell files match checksums`と終了コード0。

本人からCMakeLists.txt・prj.confの全文表示を受領し、追加ソース・ヘッダ検索先・UART／GPIO・競合機能の無効化・main設定が想定どおりと確認した（2026-10-31受領）。
本人の`verify_vendor.py`は`PASS: 16 NT-Shell files match checksums`、終了コード0。
これは保存済みチェックサムとの照合であり、今回の本人による公式アーカイブ照合・ライセンス本文確認とは区別する。

Console・Shell・PRINTK・LOG等がすべて既定でyという意味ではない。
採用版ではCONSOLE・UART_CONSOLEはCPU0のボードdefconfigがyを指定し、PRINTK・BOOT_BANNERはKconfigのdefault y。
一方、SHELL・LOG・CONSOLE_SUBSYSは通常の既定値がnで、今回は使わない意図を明記している。
BOOT_BANNERはPRINTKをselectするため、PRINTKと一緒に無効化する。
明示的なnも他機能のselectを強制的に覆す指定ではなく、最終状態は生成した.configで確認する。
採用ローカル定義と[公式の設定マージ説明](https://docs.zephyrproject.org/latest/build/kconfig/setting.html#the-initial-configuration)を参照。

### 3. ビルドと生成設定の確認（本人確認済み）

段階8専用の本人用出力先を作り直す。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\08_memory_ram" -d $memoryBuild
$LASTEXITCODE
Select-String -Path "$memoryBuild\zephyr\.config" `
    -Pattern 'CONFIG_(SERIAL|UART_INTERRUPT_DRIVEN|UART_MCUX_LPUART|GPIO|CONSOLE|CONSOLE_SUBSYS|SHELL|PRINTK|LOG|BOOT_BANNER|MAIN_STACK_SIZE|LITTLE_ENDIAN)(=| )'
```

リンク完了・終了コード0・使用量を記録する。UART割り込み・GPIO・LITTLE_ENDIAN有効、
console／shell／printk／log無効、mainスタック4096 Bを確認する。

本人から165/165リンク完了・終了コード0と、本人用`build/08_memory_ram`・CPU0への生成表示を受領した（2026-10-31）。
Flash 34,200 B、RAM 10,992 B、SRAMX・IDT_LISTは0 B。CPU0向けRAM領域320 KiBに対して3.35%。
マイコン全体のFlash 2 MiBに対して1.63%、SRAM 512 KiBに対して2.10%。静的なビルド時使用量であり、実行時最大使用量とは区別する。
Codexは本人用CMakeCacheの段階8アプリ・prj.conf・CPU0・採用Zephyr・SDKコンパイラ、.configの上記設定を読み取り照合した。
mapの`_flash_used=0x8598`、`_image_ram_size=0x2af0`も一致。
本人から生成.configの表示結果を受領し、MAIN_STACK_SIZE=4096、SERIAL・UART_INTERRUPT_DRIVEN・UART_MCUX_LPUART・GPIO・LITTLE_ENDIAN=y、
LOG・CONSOLE・CONSOLE_SUBSYS・SHELL・PRINTK・BOOT_BANNERの無効化がすべて想定と一致した（2026-10-31受領）。

### 4. 接続確認・書き込み

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
LinkServer probes --no-boot
```

本人からCOM5／MCU-Link VCom Portと、プローブ1番のMCU-LINK FRDM-MCXN947 (r0E7)、
CMSIS-DAP V3.128、シリアルHN2APUBRDCEPTの表示を受領した（2026-10-31）。
今回の列挙で確認したシリアル番号を指定する書き込み手順は以下。

```powershell
west -v flash -d $memoryBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

`Finished writing Flash successfully.`と終了コード0を確認する。
LinkServerの補助ブート処理の既知のUnicodeDecodeErrorは、後続の書き込み結果と分けて確認する。

本人から`Pb: (100) Finished writing Flash successfully.`、`Loaded 0x8598 bytes in 311ms`、
対象リセット・実行開始の抜粋と終了コード0を受領した（2026-10-31）。
ロード量34,200 Bは本人のビルド結果と一致。Flash書き込み成功を確認した。
今回の提供範囲は末尾の抜粋であり、全ログ・消去セクタ数・補助ブート処理の有無の確認とは区別する。
UARTの起動表示と動作確認は以下の本人結果で確認済み。

Tera Termを今回のCOMと上記設定へ合わせ、RESETを押して離す。

```text
Stage 8: NT-Shell RAM commands
NT-Shell package 0.3.1; local echo OFF; type help.
LED starts OFF; blink default toggle interval=500 ms.
RAM: [0x........, 0x........) 128 bytes; end exclusive
read/write: 8/16/32 bit, aligned; dump: 1..64 bytes.
mcxn947>
```

本人から上記と一致する起動UART全文を受領した（2026-10-31）。
実際のRAM表示は`RAM: [0x30000730, 0x300007b0) 128 bytes; end exclusive`。
有効なバイトアドレスは`0x30000730`から`0x300007af`までで、終端そのものは含まない。
本人がTera Termの案内した設定を合わせ、赤色LEDが消灯していることも確認したと報告した。
UART文字列の提供と本人の目視・設定確認報告に基づくもので、設定画面自体は未受領。

### 5. 初期パターンと各幅の読み書き

`help`→`mem info`を実行し、表示された先頭を手元でAと呼ぶ。
**Aは説明用の記号であり、シェルの変数ではない。実際の0x付きアドレスへ置き換える。**
アドレスはビルドで変わり得るため、過去の例を流用しない。

| 入力（Aは置換） | 期待値 |
|---|---|
| `mem dump A 16` | `00 01 02 ... 0f` |
| `mem read A 8` | `0x00` |
| `mem read A 16` | `0x0100` |
| `mem read A 32` | `0x03020100` |
| `mem write A 32 0x12345678`→`mem read A 32` | 両方`0x12345678` |
| `mem dump A 4` | `78 56 34 12` |
| A+4へ`16 0xabcd`を書き、同じ幅で読む | `0xabcd` |
| A+6へ`8 255`を書き、同じ幅で読む | `0xff` |
| `mem dump A 8` | `78 56 34 12 cd ab ff 07` |

`A+4`等も計算後の0x付きアドレスに置き換える。シェルは加算式を解釈しない。
途中で`led blink 500`を実行し、入力待ちとdump中に点滅が続くか確認する。

本人からhelpのLED／RAM使用方法、mem infoの同じ範囲表示、先頭16 Bの`00 01 ... 0f`を受領した（2026-10-31）。
同じ先頭0x30000730のreadはR8=`0x00`、R16=`0x0100`、R32=`0x03020100`で、初期値とリトルエンディアンの期待値に一致した。
続いて本人から32 bitの0x12345678、16 bitの0xabcd、8 bitの255のwrite／readと、
先頭8 Bのdump=`78 56 34 12 cd ab ff 07`を受領し、すべて期待値に一致した（2026-10-31）。
本人はled blink 500の後、入力待ち中・操作中も点滅が続いたと報告した。目視の継続確認であり、厳密な周期や個々のdump処理時間中の測定とは区別する。
各幅の最終有効位置・範囲外／不整列の拒否確認とRESETでの再初期化は次節、GDB照合は手順7に記録。

### 6. 境界・不正入力

各拒否後にプロンプトへ戻り、有効な`mem read`が動くことも確認する。

本人から0x300007afで8 bitの255、0x300007aeで16 bitの65535、0x300007acで32 bitの0xffffffffをwrite／readしたUARTを受領し、
各幅の最終有効位置・最大値の受理を確認した（2026-10-31）。
末尾0x300007afの1 B dumpは`ff`、0x300007acの4 B dumpは`ff ff ff ff`で一致した。
続いて本人のUARTで、終端0x300007b0のread／write／dump、先頭直前0x3000072fのread、
0x300007afから2 Bと0x30000771から64 Bの終端超過dump、0xffffffffの32 bit readの7件を範囲外として拒否したことを確認した（2026-10-31受領）。
0x30000731の16 bit readと0x30000732の32 bit writeも不整列として拒否した。
試験前後の先頭16 Bは同じ`78 56 34 12 cd ab ff 07 08 09 0a 0b 0c 0d 0e 0f`で、最後の32 bit readも0x12345678。
拒否後のプロンプト復帰と有効コマンドの応答を確認した。比較した範囲は先頭16 Bであり、RAM全体の不変の実測とは区別する。
この拒否試験中のLEDの追加報告は未受領。入力待ち・通常RAM操作中の点滅継続は前節で本人確認済み。
本人の`mem dump 0x30000730 64`は0x30000730・740・750・760から始まる4行で64 Bを表示した（2026-10-31受領）。
dump長0／65、幅64、8／16／32 bitの上限超過値256／65536／4294967296、符号-1／+1、不完全な0x・不正桁0xgg、余分な引数の11件は、
すべて`ERROR: invalid memory command or argument`とMemory usageを表示し、プロンプトへ復帰した。
最後の先頭16 B dumpも同じ既知パターンで、readは0x12345678。拒否後の正常動作を確認した。
この不正引数試験中のLEDの追加報告は未受領。RESETでの再初期化は以下の本人結果で確認済み。

| 操作 | 期待値 |
|---|---|
| A+127を8 bit、A+126を16 bit、A+124を32 bitで読み書き | 最後の有効な位置として受理 |
| A+128のread／write／dump、A-1のread | `outside test RAM range` |
| A+127から2 B、A+65から64 Bをdump | 終端超過として拒否 |
| A+1の16／32 bit、A+2の32 bit読み書き | `unaligned address` |
| `mem dump A 1`／`mem dump A 64` | 受理。64 Bは4行 |
| `mem dump A 0`／`65`、幅64、値256を8 bitでwrite | 不正引数として拒否 |
| 値-1／+1／0x／0xgg／4294967296、余分な引数 | 不正引数として拒否 |
| `mem read 0xffffffff 32` | 範囲外として拒否し、停止しない |

拒否前後で`mem dump A 16`を比較し、内容が変わらないことを確認する。
RESET後は初期パターンへ戻ることも確認する。

本人からRESET後の起動UARTとmem infoの同じ範囲表示、先頭16 Bの00..0f、
先頭32 bit read=0x03020100、末尾0x300007acから4 Bの7c 7d 7e 7fを受領した（2026-10-31）。
前に書き換えた先頭と末尾が初期パターンへ戻ったことを確認した。本人はLED消灯も報告した。

### 7. GDBとの照合

段階1と同じLinkServerのGDBを使う。**`west debug`はELFをロードしリセットするため、接続前に書いたパターンは作り直す。**
今回のプローブ番号を確認し、同じビルドのELFで接続する。

```powershell
west debug -d $memoryBuild -r linkserver --probe HN2APUBRDCEPT
```

本人の接続ログで、ninjaのno work、SDK 1.0.1のGDB 16.2、プローブHN2APUBRDCEPT・cm33_core0への接続、
本人用build/08_memory_ram/zephyr/zephyr.elfのシンボル読み込みとload size 34200、(gdb)到達を確認した（2026-10-31受領）。
2回のEnterは`Type <RET> for more`のページ送り待機への応答で、異常ではない。
本人が`set pagination off`を入力した表示も受領した。
補助プローブブートのUnicodeDecodeErrorと、ロード前の初期resetの`No image address available for soft reset`は表示されたが、その後に接続・ELFロードは完了している。
FW更新は候補案内であり、更新実行の表示ではない。
その後、本人からmonitor resetでリセット成功を確認し、continueを実行してTera Termにも段階8の起動表示が出たとの報告を受領した（2026-10-31）。
ロード後のリセットと再開は本人の確認報告に基づき確認済み。今回のリセット応答の原文自体は未受領。再起動UARTは後続の既知パターン確認時に受領した。

GDBで以下を実行する。

```text
set pagination off
monitor reset
continue
```

Tera Termで起動表示と`mem info`を確認し、手順5の3つの既知値を書き直し、read／dump結果を残す。
本人から再起動UARTと同じRAM範囲、32 bit=0x12345678・16 bit=0xabcd・8 bit=0xffのwrite／read、
先頭8 Bのdump=`78 56 34 12 cd ab ff 07`を受領し、GDBで照合する既知パターンが用意できたことを確認した（2026-10-31）。

GDB端末でCtrl+Cを押してCPUを止める。停止中はシェルとLEDも止まる。

```text
print/x &memory_test_buffer
x/8bx &memory_test_buffer
x/2hx &memory_test_buffer
x/1wx &memory_test_buffer
```

先頭アドレスが`mem info`と一致し、バイト列は`78 56 34 12 cd ab ff 07`、
16 bitの先頭2値は`0x5678 0x1234`、32 bitの先頭値は`0x12345678`となることを照合する。
本人のCtrl+CでSIGINTとなり、arch_cpu_idleのcpu_idle.c:104で停止した表示を受領した（2026-10-31）。
GDBのアドレスは0x30000730、x/8bxは78 56 34 12 cd ab ff 07、x/2hxは0x5678 0x1234、x/1wxは0x12345678で、すべて期待値と一致した。
これでシェルの書き込み・read・dumpと、同じ実行状態のGDBメモリ照合を本人の実機で確認した。

CPU停止中のGDBで、段階1と同じく`monitor ondisconnect cont`→`detach`→`quit`を実行して終了する。
最初の指定は切断時にCPUを再開させるためのLinkServer設定。
PowerShellで終了コードを確認し、Tera Termで`led off`と`mem read 0x30000730 32`を実行する。
プロンプト応答・0x12345678の保持とLED消灯を確認する。

本人からGDB終了後にled offとmem readへ応答しないとの報告を受領した（2026-10-31）。
今回のGDB切断時の自動再開は成功と扱わない。
その後、本人からmonitor ondisconnect cont入力、detachによるEnding remote debugging・Inferior detached・Disconnected from core cm33_core0・Gdbserver closed、
quit後のPowerShell復帰と終了コード0を受領した（2026-10-31）。切断・終了は成功だが、CPU再開・UART応答の成功とは区別する。
本人へボードのRESETによる復旧確認を案内した。RESET後は専用RAMが再初期化され、先頭32 bitの期待値は0x03020100となる。
GDBで既知パターンが一致した確認結果と、この終了後の無応答を区別する。
その後、本人からRESET後のRAM範囲表示、helpの全項目、led offのOK、先頭read=0x03020100と赤色LED消灯の報告を受領した（2026-10-31）。
RESETによるUART応答の復旧と、最終状態のLED消灯・RAM再初期化を確認した。GDB終了直後の無応答の原因は未確定のまま残す。

## 確認結果と未確認事項

| 項目 | 状態 |
|---|---|
| Codexの外部OSS照合 | 16ファイルと公式原本・SHA一致、終了コード0 |
| CodexのCPU0向けビルド・QEMUテスト | [事前検証記録](evidence/README.md)参照 |
| 本人の環境 | ZEPHYR_BASE・west 1.5.0・採用Zephyr SHA・作業ブランチの一致を本人の表示で確認（2026-10-31受領） |
| 本人のソース・設定 | main.c・commands.c・memory_commands.cの案内した各処理の確認完了報告と、CMakeLists.txt・prj.confの想定どおりの表示を受領（2026-10-31） |
| 本人の外部OSS照合 | 16ファイルの保存済みチェックサムとの一致・終了コード0を確認（2026-10-31受領）。今回の本人による公式アーカイブ照合・ライセンス本文確認は未受領 |
| 本人のビルド | 165/165リンク完了・終了コード0、CPU0・本人用build/08_memory_ram。Flash 34,200 B、RAM 10,992 B、SRAMX・IDT_LIST 0 B（2026-10-31受領） |
| 本人用生成物のCodex照合 | 対象アプリ・設定・CPU0・Zephyr・SDK、生成.configとmapの使用量を読み取り確認 |
| 本人の生成設定表示 | UART関連・GPIO・LITTLE_ENDIAN=y、Console／Shell／PRINTK／LOG／BOOT_BANNER無効、mainスタック4096 Bを本人の表示で確認（2026-10-31受領） |
| 本人の接続先 | 今回の本人の列挙でCOM5／MCU-Link VCom Portと、FRDM-MCXN947／CMSIS-DAP V3.128／HN2APUBRDCEPTの一致を確認（2026-10-31受領） |
| 本人の書き込み | Flash成功表示・終了コード0、ロード量0x8598（34,200 B）、対象リセット・実行開始の末尾表示を確認（2026-10-31受領） |
| 本人のUART起動・初期LED・端末設定 | 起動UART全文とプロンプト・RAM範囲[0x30000730, 0x300007b0)の一致、案内したTera Term設定と赤色LED消灯の確認報告を受領（2026-10-31）。設定画面自体は未受領 |
| 本人のhelp・mem info・初期パターンと各幅のread | helpと範囲一致、先頭16 Bの00..0f、R8=0x00・R16=0x0100・R32=0x03020100を提供UARTで確認（2026-10-31受領） |
| 本人の全幅のwrite・読み戻し・dump | 32 bit=0x12345678、16 bit=0xabcd、8 bit=0xffのwrite／readと、dump=78 56 34 12 cd ab ff 07の一致を提供UARTで確認（2026-10-31受領） |
| 本人のRESETでの再初期化 | 同じ範囲、先頭16 B=00..0f、先頭R32=0x03020100、末尾4 B=7c 7d 7e 7fを提供UARTで確認し、LED消灯の報告も受領（2026-10-31） |
| 本人の最終有効位置と最大値 | 8 bit=0x300007afで0xff、16 bit=0x300007aeで0xffff、32 bit=0x300007acで0xffffffffのwrite／readと、末尾1 B・4 Bのdump一致を提供UARTで確認（2026-10-31受領） |
| 本人の範囲外・アラインメントの拒否と復帰 | 範囲外／終端超過7件・不整列2件を期待するERRORで拒否。前後の先頭16 B一致と正常read=0x12345678を提供UARTで確認（2026-10-31受領） |
| 本人の不正引数・数値境界・ダンプ長上限 | 64 Bの4行dump、長さ・幅・各幅の上限超過値・符号・不正16進表記・余分な引数11件の拒否と使用方法表示、最後の先頭16 B不変と正常readを提供UARTで確認（2026-10-31受領） |
| 本人のGDB接続・ELFロード・再開 | GDB 16.2、HN2APUBRDCEPT・cm33_core0、段階8 ELFのload size 34200と(gdb)到達を確認。ロード後のmonitor reset成功・continue・段階8起動も本人報告で確認（2026-10-31受領） |
| 本人のGDBによる既知パターンとの一致 | Ctrl+Cで停止し、アドレス0x30000730、8 B列78 56 34 12 cd ab ff 07、16 bit値0x5678 0x1234、32 bit値0x12345678を提供GDB出力で確認（2026-10-31受領） |
| 本人のGDB終了と切断後の応答・LED | detach・server終了・quit後の終了コード0を確認。切断直後のUARTは無応答だったが、RESET後にhelp・led off・read=0x03020100とLED消灯を確認（2026-10-31受領）。無応答の原因は未確定 |
| 本人の入力待ち・操作中のLED継続 | led blink 500後、入力待ち中・RAM操作中も点滅が継続したとの本人の目視報告を受領（2026-10-31）。周期・処理時間中の厳密な測定は未実施 |
| UART異常・キュー満杯・行長境界の実機経路 | 未確認 |
| 実行時最大スタック／ヒープ・厳密な周期 | 未測定 |
| 最終状態 | 段階8の同じビルド、GDB終了済み・終了コード0。RESET後のシェル応答とLED消灯、先頭R32=0x03020100。端末は送信CR+LF・受信CR（設定変更の報告なし） |
| 本人のREADME確認 | 実際の実習内容と一致するとの確認報告とPR作成依頼を受領（2026-11-01） |
| PR作成・本人のPR確認／マージ／イシュー完了 | PR #26を2026-11-01に作成。本人のPR確認・マージ・クローズは未実施。#9はOPEN |

本人の出力や観察は[user-evidence](user-evidence/README.md)へ区別して残す。
