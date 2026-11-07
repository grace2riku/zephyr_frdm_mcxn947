# 段階9：ZephyrのGPIOドライバと周辺レジスタの役割を確認する

対象：[イシュー #10](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/10)。前提：[段階8](../08_memory_ram/README.md)。
開始日：2026-11-01（JST）。作業ブランチ：`codex/stage9-register-access`。

## 目的と現在の状態

ZephyrのDevicetree・Kconfig・GPIO APIがMCXN947の端子にどう結び付くか、ドライバとアプリの担当を確認する。
実施済みの直接レジスタ操作は、その関係を知るための補助教材として残す。

**2026-11-07、本人の依頼によりGDBによる読み出し値の照合は省略し、作業内容・完了条件から外した。**
Zephyr学習を中心とし、GDB・レジスタの詳細調査や数値解析の全行理解を必須にはしない。
この方針は段階9だけでなく、後続段階にも適用する。[継続方針](../../AGENTS.md)を参照。

| この段階で見るZephyr機能 | 使用する目的 | 今回のコードでの対応 |
|---|---|---|
| Devicetree | ボードのどの端子を使うか指定する | DT_ALIAS(led0)・GPIO_DT_SPEC_GETでGPIO0／pin 10／Active Lowを参照 |
| GPIO API | ドライバの準備を確認して端子を設定する | gpio_is_ready_dt・gpio_pin_configure_dtで起動時に消灯する出力へ設定 |
| Kconfig | 使用する機能・ドライバをビルドへ含める | GPIO・UART割り込みを有効化。標準Shell等との同じUARTの競合を避ける |
| UART割り込みとキュー・main | 受信とコマンド処理の担当を分ける | ISRで受信バイトをk_msgqへ渡し、mainでNT-Shellとコマンドを処理 |

通常のZephyrアプリの周辺機能操作は、対応するドライバAPIの利用を基本とする。
今回のMMIOは、GPIO APIが扱う回路との関係を観察する補助として実施した。
初期設定後はmainだけが出力を変更し、GPIO APIや別スレッドから同時に書き換えない構成を確認する。
Codexが開始時の変更なしmain `1d28591`とイシュー #10のOPENを確認し、作業ブランチと初期実装を作成した。
本人のNT-Shellファイル照合とCPU0向けビルドは2026-11-07に成功を確認した。
本人のFlash書き込みも成功表示・終了コード0を確認した。
起動表示・mcxn947>・赤色LED消灯・help正常応答は本人の確認報告を受領済み。
reg infoとPDOR／PDDR／PDIRの初期読み出し、PCOR 2回／PSOR 1回の書込・PDOR readを提供UARTで確認済み。
LEDの点灯→点灯維持→消灯も本人が目視確認したと報告した。
PTORの点灯→消灯と、消灯状態でのPSOR／PCOR／PTORへの値0による状態維持も確認済み。
不正入力9件のERRORを提供UARTで確認し、消灯維持と最後の正常read・helpの応答も本人報告で確認済み。GDB照合は本人の指示で省略した。
旧条件のGDB照合は除外する。2026-11-08に本人がREADMEの6項目と実際の実習の一致を確認し、コミット・プッシュ・PR作成を依頼した。
イシューはOPENを維持し、PRの本人確認・マージ・クローズは未実施として残す。

**2026-11-01、本人提供のリファレンスマニュアル本文とのCodexによる照合を完了した。**
PDF実物はMCX N Reference Manual `MCXNP184M150F70RM`、Rev. 7, 2025-10-10。
第73章の対象レジスタと第4章のメモリマップを確認し、初期実装の変更は不要だった。
ハードウェアは8／16／32 bitに対応し、本コマンドが32 bitに限定すること、
PSOR／PCOR／PTORは読み出し0となること、未接続ピンのPDOR値は未定義となることを説明へ補足した。
資料のSHA-256、節・ページ番号、W1Cと読み出し副作用の具体例は[RM照合記録](REGISTER_REFERENCE.md)を参照する。
本人のマニュアル読解・実習完了とは区別する。本人の環境4項目は提供表示で一致を確認済み。
2026-11-07、本人からMMIO・許可表の検査の意味が分かり、register_access.h/.cも確認できたとの報告を受領した。
register_commands.cの説明後、本人から未理解だが先へ進めたいとの依頼を受領した（2026-11-07）。
理解確認を保留し、commands.c・main.c・CMakeLists.txt・prj.confの役割を短く説明した。
その後、本人のファイル照合PASS・終了コード0と、166/166リンク完了・ビルド終了コード0を確認した。
Flash 34,436 B・RAM 8,744 B。本人の今回の列挙でCOM5とプローブHN2APUBRDCEPTも確認済み。
このプローブを指定する手順に対する本人の書き込み成功報告と、Tera Termでの起動・初期LED消灯・helpの確認報告を受領した。
reg infoの許可表と、PDOR=0x00000400／PDDR=0x00000400／PDIR=0x00000449、LED消灯維持を確認した。
PCOR／PSORの書き込みとPDORの0→0→0x400、LED点灯→点灯維持→消灯を確認した。
PTORの2回の反転と消灯状態での3操作先への値0も、UARTと本人のLED目視報告で確認した。
不正入力9件の拒否・プロンプト復帰・消灯維持・正常readとhelpの応答を確認した。GDB照合は本人の依頼で省略し、実習記録の確認へ進む。
ソース理解の完了とビルド成功は別に扱う。

Codexの事前検証は[evidence/README.md](evidence/README.md)、本人の記録は[user-evidence/README.md](user-evidence/README.md)に分ける。

## 学習に使用したAIモデル

| 利用期間（JST） | AIツール | AIモデル（モデルID） | AIモデルバージョン | 推論設定 | 確認根拠 |
|---|---|---|---|---|---|
| 2026-11-01〜 | OpenAI Codex（デスクトップ） | 未確認 | 未確認 | 未確認 | 本段階の保存済み設定は未照合。過去段階の情報を流用しない |

[記録方針](../../docs/ai-models.md)に従う。

## 使用環境・配線

- FRDM-MCXN947、`frdm_mcxn947/mcxn947/cpu0`。CPU1は起動しない。
- Zephyr 4.4.2、`dccb09599635bdff17633fa7e9dab014b91dce90`。CodexがHEADと変更なしを確認。
- NXP HAL：`7554bc0f6f6c284972eba7b2c17b16fbb9cc0ad0`。
- SDK 1.0.1、Python 3.12.10、west 1.5.0、CMake 3.31.8、DTC 1.6.1を継続する。
- NT-Shell公式配布0.3.1を段階8から独立コピー。[UPSTREAM.md](UPSTREAM.md)参照。
- J17のMCU-Link USBを使用。UART 115200、8N1、フロー制御なし、ローカルエコーOFF、送信CR+LF・受信CR。
- 内蔵赤色LED（P0_10、Active Low）を使用する。GPIO0 bit 10はArduino D9とも共有するので、外部回路は接続しない。
- COM・プローブは本段階の本人による再列挙でCOM5／MCU-Link VCom Port、HN2APUBRDCEPT／FRDM-MCXN947と確認した（2026-11-07受領）。

## レジスタの許可表

アドレスは本構成のSecure側GPIO0 `0x50096000`＋オフセット。
RM §73.7.1.1の基本アドレス0x40096000と、第4章Table 16のSecure窓を照合した。
ソースは固定値を直接ポインタ化せず、Devicetreeの`DT_REG_ADDR`を使う。
`GPIO0_BASE`との一致・LED端子・極性・各オフセットを`BUILD_ASSERT`で検査する。
起動表示／`reg info`の値と本人用の生成物を再照合する。

| レジスタ | オフセット | この構成のアドレス | RMとヘッダーで確認した意味 | Shellで許可する操作 |
|---|---:|---|---|---|
| PDOR | 0x40 | 0x50096040 | RW、出力ラッチ | readのみ |
| PSOR | 0x44 | 0x50096044 | 1で対応するPDORビットをセット、0で不変 | writeのみ、bit 10だけ |
| PCOR | 0x48 | 0x50096048 | 1で対応するPDORビットをクリア、0で不変 | writeのみ、bit 10だけ |
| PTOR | 0x4c | 0x5009604c | 1で対応するPDORビットを反転、0で不変 | writeのみ、bit 10だけ |
| PDIR | 0x50 | 0x50096050 | RO、入力値 | readのみ |
| PDDR | 0x54 | 0x50096054 | RW、方向。1で出力 | readのみ |

本コマンドのアクセスはすべて32 bit、4 B境界。GPIOハードウェアは8／16／32 bitアクセスに対応する（RM §73.7、p.3529）。
任意のアドレス範囲を開放せず、表のアドレス・方向と完全一致した操作のみを許可する。
書き込み値は`0`か`0x400`（10進数1024）。他ビットを含む値はマスクして続行せず拒否する。
Non-secure側の別名、ISFR、PORT設定、UART、クロック、Flashなどはこのコマンドでアクセスできない。

### RAMとMMIO、RO／RW／W1Cと副作用

- 段階8のRAMは保存した値を同じ場所から読む。レジスタは周辺回路への窓で、同じ検証方法を一律には使えない。
- `sys_read32`／`sys_write32`でMMIOを扱う。採用ARM実装のvolatile 32 bitアクセスを利用する。
  volatileは排他制御や複合操作の不可分性を保証しない。
- PDORはハードウェア上RWでも、Shellでは書き込み禁止。ポート全体への書き込みで他端子を変えないため。
- PSOR／PCOR／PTORは読み戻して書き込み値と比較しない。更新先のPDOR bit 10とLEDを確認する。
  RMの各図では読み出し値は0で、書いた値を保存して返すレジスタではない。
  read-modify-writeをせず、指定したビットを単一の32 bit storeで送る。
- PCORの「1を書いて出力ラッチを0にする」働きと、割り込みフラグのW1Cは用途が異なる。
  GPIO ISFR[0/1]はRM §73.7.1.20（pp.3549–3550）でも1でフラグをクリアする。これに`reg |= mask`をすると、読み出した他の1まで書き戻して消し得る。
  レベル条件が残る場合の再セットやDMA完了による自動クリアもあり、常に書込直後に0となるわけではない。
  本段階ではISFRを許可表へ入れず、割り込みフラグの実機クリア試験は行わない。
- 読むとFIFOが進む・フラグが消えるレジスタは、単なる観察でも状態を変える。UARTの受信データなどへ範囲ダンプやGDBの自動更新を行わない。
  許可したPDOR／PDIR／PDDRには、個別の図・フィールド記述にreadで状態を消す作用は定義されていない。
  LPUARTのDATAと、FIFOを取り除かず読むDATAROの違いも[RM照合記録](REGISTER_REFERENCE.md)に記録した。どちらもShellでは操作しない。
- PDIRは端子入力、PDORは出力ラッチ。端子状態・入力バッファ・外部接続に依存するため、ポート全体が一致するとは決めつけない。
- 未接続ピンのPDOR読み値はRMでも未定義。全bitの固定値比較をせず、使用するbit 10をマスクして確認する。

### 所有者とドライバの競合

起動時だけ`gpio_pin_configure_dt(GPIO_OUTPUT_INACTIVE)`でP0_10をGPIO出力・High（消灯）へ設定する。
採用`gpio_mcux.c`は出力ラッチの設定後に方向を切り替え、PORTのGPIO muxと入力バッファも設定する。
設定後のP0_10への書き込みはmainのNT-Shellだけが担当する。
RMのハードウェアリセット値と起動後の値は異なる。アプリが初期設定するため、UART起動時にはPDOR／PDDRのbit 10は1になる。
段階8の`led_control.c`とLEDスレッドは取り込まず、`led on/off/blink`コマンドも用意しない。
GPIOドライバ自体は初期設定のため有効。`CONFIG_LED_GPIO`／`CONFIG_INPUT_GPIO_KEYS`が有効な構成はビルド時に拒否する。
UARTのISRは受信キューだけを操作し、P0_10を変更しない。CPU1・デバッガからの書き込みも同時に行わない。

## コマンド

```text
help
reg info
reg read <0xADDRESS> 32
reg write <0xADDRESS> 32 <VALUE>
mem info
mem read <0xADDRESS> <8|16|32>
mem write <0xADDRESS> <8|16|32> <VALUE>
mem dump <0xADDRESS> <LENGTH:1..64>
```

アドレスは`0x`／`0X`付き16進数。値は符号なしの10進数か`0x`／`0X`付き16進数。
32 bitを超える数値、符号、不正桁、余分な引数、幅が文字どおり`32`以外のレジスタ操作は拒否する。
レジスタのdumpは実装しない。RAMの仕様は[段階8](../08_memory_ram/README.md#コマンドとramの仕様)を継承し、
`mem`ではレジスタを操作できず、`reg`では専用RAMを操作できない。
62 Bの入力上限・63 B満杯行の拒否・UART欠落時のコマンド抑止は継続する。
UARTエラー時はまずEnterを1回押し、その後コマンドを入力し直す。

書き込み前後のPDORを読み、bit 10が操作に応じた値になった場合だけ`OK`を返す。
この`OK`は出力ラッチの確認であり、LEDの発光は本人の目視で別に確認する。

## ソースを読む順番

1. `register_access.h/.c`：端子とアドレスの定義、ビルド時照合、初期設定、32 bit MMIO。
2. `register_commands.c`：許可表、数値・幅・権限・ビットの検査、操作後のPDOR確認。
3. `commands.c`：受信異常と行長を検査してから`reg`／`mem`へ振り分け。
4. `main.c`：UART→GPIO設定→RAM→NT-Shell→情報表示→受信開始。
5. `CMakeLists.txt`／`prj.conf`：段階8のLEDスレッドをリンクしないこと、UART受信競合の無効化。

## 本人の操作手順

一度に全部を進めず、結果を確認して次へ進む。以下は予定手順と期待値であり、実測記録ではない。

### 1. 環境確認（本人確認済み）

本人提供のRMと許可表の照合はCodexが完了した。新しいPowerShellで以下を実行し、表示を確認する。
環境スクリプトの設定はそのPowerShellだけに適用される。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
Set-Location $env:ZEPHYR_BASE
$registerBuild = Join-Path $zephyrWorkspace 'build\09_register_access'
$env:ZEPHYR_BASE
west --version
git rev-parse HEAD
git -C $learningRepo branch --show-current
```

期待値：ZEPHYR_BASE=`C:\Users\grace\zephyrproject-mcxn947\zephyr`、west 1.5.0、
Zephyr SHA=`dccb09599635bdff17633fa7e9dab014b91dce90`、ブランチ=`codex/stage9-register-access`。

本人から上記4項目のPowerShell表示を受領し、すべて期待値と一致することを確認した（2026-11-01受領）。
これは本人の環境確認であり、ソース・ビルド・実機の確認とは区別する。操作の正確な時刻は未取得。

### 2. ソース・固定情報の確認と本人のビルド

上の読む順番でコードと設定を案内し、外部OSS照合とビルドへ進む。
最初のregister_access.h/.cのオフセット・bit mask、BUILD_ASSERT、GPIO初期化、32 bit MMIOを説明し、
本人からMMIOと許可表の理解・両ファイルのコード確認完了の報告を受領した（2026-11-07）。
続いてregister_commands.cの許可表、表示関数、数値解析、引数・幅・操作方向・値の検査、
書込前後のPDOR bit 10の比較を説明した。本人は「理解はできていませんが、先にすすめてください」と報告した（2026-11-07受領）。
同ファイルは理解・確認完了として扱わず、本人の依頼に基づき先へ進める。
commands.cの受信異常・行長検査／単語分割／振り分け、main.cの初期化・コールバック登録・受信開始、
CMakeLists.txtのソース組込みとprj.confのUART・GPIO・受信競合機能の設定を短く説明し、以下の実行を案内する。
これらの本人の理解・確認完了報告は未受領。外部OSS照合・ビルドの結果は以下のとおり受領した。

```powershell
python "$learningRepo\stages\09_register_access\scripts\verify_vendor.py"
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\09_register_access" -d $registerBuild
$LASTEXITCODE
```

実行場所はZEPHYR_BASE。`-p always`は段階9専用の出力先を作り直す。
リンク完了・終了コード0・Flash／RAM使用量、CMakeCacheのアプリ・CPU0・採用Zephyr・SDKを確認する。
生成.configでUART割り込み・GPIO・Secure firmware有効、console／shell／printk／log無効、
LED_GPIO／INPUT_GPIO_KEYSが有効でないことを確認する。
生成ヘッダーのGPIO0アドレス、led0のgpio0／pin 10／Active Low、ELFのMMIO関数も照合する。

2026-11-07に本人からファイル照合PASS・終了コード0の報告と、166/166リンク完了・終了コード0のビルド抜粋を受領した。
本人用`C:/Users/grace/zephyrproject-mcxn947/build/09_register_access`、CPU0向け生成を提供表示で確認した。
チェックサム照合の出力全文は未受領で、本人による公式アーカイブ同時照合の実施は未確認。
Codexが本人用CMakeCache・.config・Devicetree・ELF・runners.yamlを読み取り照合した結果も、
[本人実習記録](user-evidence/README.md#本人のファイル照合とビルド)へ別の実施者として記録した。

| 対象 | 本人のビルド使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 34,436 B | 2,097,152 B | 1.64% |
| SRAM | 8,744 B | 524,288 B | 1.67% |

ビルド表示のRAM 2.67%はCPU0割当320 KiBを分母とする。SRAMX・IDT_LISTは0 B。
静的なビルド使用量であり、実行時最大スタック・ヒープや実機動作の確認とは区別する。

### 3. 今回の接続先確認と書き込み

本人が以下を実行し、今回のCOM・プローブ列挙結果を受領した（2026-11-07）。

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
LinkServer probes --no-boot
```

今回の提供表示はCOM5／MCU-Link VCom Port (COM5)、プローブ1番の
MCU-LINK FRDM-MCXN947 (r0E7)、CMSIS-DAP V3.128、シリアルHN2APUBRDCEPT。
前段階の値の流用ではなく、今回の本人の実行表示で確認した。
Codexが本人用runners.yamlのMCXN947:FRDM-MCXN947／cm33_core0／Flash 0x10000000と、
採用LinkServer runnerのプローブ指定を再確認した。
同じPowerShellで、確認済みシリアルを指定して本人が実行する。

```powershell
west -v flash -d $registerBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

`-v`で詳細ログを出し、`Finished writing Flash successfully.`などのFlash成功表示と終了コード0を確認する。

本人から`Finished writing Flash successfully.`、0x8684 Bロード、終了コード0の表示抜粋を受領した（2026-11-07）。
34,436 Bは本人ビルドのFlash使用量と本人用zephyr.binの長さに一致する。
resetとboot ROM stall用watchpointの表示も含まれるが、アプリの実際のUART起動は次の手順で確認する。
抜粋の転記と根拠の範囲は[user-evidence](user-evidence/README.md#本人のflash書き込み)に記録した。

書き込み後、Tera TermをCOM5・115200・8N1・フロー制御なし・ローカルエコーOFF・送信CR+LF／受信CRで開く。
受信準備後にRESETを押して離し、`Stage 9: NT-Shell register commands`、許可表、RAM情報、`mcxn947>`、赤色LED消灯を確認する。
続いてhelpを入力し、reg／memのUsage表示とプロンプト復帰を確認する。
本人から起動表示・mcxn947>・赤色LED消灯、helpによる使い方表示・プロンプト復帰を確認できたとの報告を受領した（2026-11-07）。
起動・helpのUART原文や端末設定画面は未受領で、本人の一致報告・目視報告に基づく確認として区別する。
後続のreg infoとreadのUART表示は手順4へ記録した。

### 4. 読み書き・動作の確認

以下のアドレスは上の構成に対する期待値。先に本人の`reg info`と生成物が一致することを確認する。
ポート全体の固定値を仮定せず、`読み値 & 0x400`を比較する。

最初の操作として以下をTera TermのShellで1行ずつ案内し、本人の実行表示を受領した。

```text
reg info
reg read 0x50096040 32
reg read 0x50096054 32
reg read 0x50096050 32
```

PDORとPDDRのbit 10は1が期待値。PDIRは入力値として記録し、ポート全体の一致は要求しない。
2026-11-07にreg infoの6アドレスと許可方向、PDOR=0x00000400、PDDR=0x00000400、PDIR=0x00000449の表示を受領した。
PDOR／PDDR／PDIRのbit 10はいずれも1で、赤色LEDが消灯のままとの本人報告も受領した。
PDIRの他ビットがPDORと異なることを異常とは扱わず、入力値として記録した。
表示の転記は[本人の初期読み出しログ](user-evidence/register-read-excerpt.log)、確認範囲は[user-evidence](user-evidence/README.md#本人の許可表表示と初期レジスタ読み出し)を参照。
続いてPCOR書き込み・readを2回、PSOR書き込み・readを1回案内し、本人の結果を受領した（2026-11-07）。
書込み応答はすべてOK、各readはPDOR=0x00000000→0x00000000→0x00000400で、
本人が赤色LEDの点灯→点灯維持→消灯を目視したと報告した。
最後はPDOR bit 10=1、LED消灯。提供表示は[PCOR・PSOR操作ログ](user-evidence/register-set-clear-excerpt.log)へ転記した。
その後、PTORへ0x400を2回書く反転操作と、PSOR／PCOR／PTORへ値0を書く状態維持の結果を受領した（2026-11-07）。
PTORの書込・readはPDOR=0→0x400、LED点灯→消灯。続く値0の3操作はすべてOK、PDOR=0x400、LED消灯を維持した。
提供表示は[反転・値0操作ログ](user-evidence/register-toggle-zero-excerpt.log)へ転記した。値0の実機確認は消灯状態で行った範囲。
続いて以下の不正入力の拒否と正常復帰を案内し、本人の結果を受領した。

| 操作 | 期待するPDOR bit 10／LED |
|---|---|
| `reg read 0x50096040 32` | 起動後は1／消灯 |
| `reg read 0x50096054 32` | PDDR bit 10が1（出力） |
| `reg read 0x50096050 32` | PDIRを記録。PDORと意味を区別する |
| `reg write 0x50096048 32 0x400` | PCORで0／点灯 |
| 同じPCOR書き込みをもう1回 | 0／点灯を維持 |
| `reg write 0x50096044 32 0x400` | PSORで1／消灯 |
| `reg write 0x5009604c 32 0x400`を2回 | 0→1／点灯→消灯 |
| 上記3つの書き込み先へそれぞれ値0 | 直前の状態を維持 |

各操作の直後にPDORをreadし、応答・ラッチ・LEDを比較する。PSOR／PCOR／PTORを読み戻して比較しない。
不正入力は`reg read 0x50096041 32`、`reg read 0x50096120 32`、`reg read 0x50096044 32`、
`reg write 0x50096050 32 0`、`reg write 0x50096040 32 0x400`、
`reg write 0x50096048 32 0x401`、`reg read 0x50096040 16`、
`reg write 0x50096048 32 -1`、`reg write 0x50096048 32 0x100000000`の9件を1行ずつ試す。
すべてERRORとプロンプト復帰、各操作時のLED消灯維持を確認する。
最後に`reg read 0x50096040 32`と`help`で正常応答・PDOR bit 10=1・プロンプト復帰を確認する。
2026-11-07に9件のERROR表示を受領し、アドレス／方向5件・許可外bit 1件・幅／符号／桁あふれ3件の拒否を確認した。
本人が各ERROR後のプロンプト復帰・LED消灯維持、最後のPDOR=0x00000400・help正常応答を報告した。
正常read・helpの出力全文は未受領のため、その2操作は本人報告による確認として区別する。
提供表示は[不正入力ログ](user-evidence/register-invalid-excerpt.log)、詳細は[user-evidence](user-evidence/README.md#本人の不正入力9件と正常コマンドへの復帰)を参照。
最後の報告状態はLED消灯・PDOR bit 10=1。

### 5. GDB照合の省略とZephyr学習への整理

当初は同じELFを使うGDB照合を案内したが、本人が目的を確認し、今回は学習せず省略するよう依頼した（2026-11-07）。
GDB接続・再ロード・レジスタ値照合の結果は未受領で、実施済みにはしない。
この段階では、本人が確認したUART・LED・レジスタの読み書き・不正入力の拒否を実機結果として残す。
GDBの必要がないまま実習を追加せず、上のZephyr機能とコードの対応を整理する。

今後は対応するZephyrドライバAPIの利用を基本とし、問題が生じたときに目的を示してデバッグを使う。
register_commands.cの詳細理解は保留という本人の報告を保持し、ビルドや実機成功を理解完了へ読み替えない。

### 6. 実習記録の本人確認

2026-11-08、本人が見直した学習方針で進めることを確認し、段階9を終えるための残る確認と次の手順の案内を依頼した。
見直し後のイシュー #10を再確認し、残る学習上の確認は以下の記録が本人の実習と一致するかの確認と整理した。
追加の実機操作やGDB照合は必須にしない。コード全体の理解や再説明を確認の条件にしない。

| 本人に確認してほしい記録 | 記録している内容 |
|---|---|
| 学習の目的 | ZephyrのDevicetree・Kconfig・GPIO API・UART割り込みとキューの担当を確認する。直接レジスタ操作は補助教材 |
| 実施済み | ファイル照合PASS、CPU0ビルド・Flash書き込み成功と終了コード0、起動・help、初期read、PCOR／PSOR／PTOR、消灯状態での値0、不正入力9件と正常復帰 |
| 省略・対象外 | GDB照合は本人の依頼で省略。割り込みステータスW1Cの実機操作は対象外 |
| 理解の状況 | MMIO・許可表の意味とregister_access.h/.cは本人が確認を報告。register_commands.cの詳細理解は保留。後続ファイルの理解完了とは記録しない |
| 使用量 | 本人のFlash 34,436 B・RAM 8,744 B。RAMは全体512 KiBの1.67%、今回の割当320 KiBの2.67% |
| 最後に受領した実機状態 | 2026-11-07の本人報告で、赤色LED消灯・PDOR=0x00000400・help正常応答。現在の状態の再測定を求めるものではない |

全行の再読やコマンドの再実行ではなく、上記の内容に事実と異なる記載や抜けがないかを本人が確認する。
2026-11-08に本人から6項目を確認し、実際の実習内容と一致しているとの報告を受領した。
README本人確認は完了。コミット・プッシュ・段階9のPR作成の明示的な依頼も受領した。

その後は、段階9の実装・証跡・継続方針の変更をコミット・プッシュし、PRを作成する。
本人がPRを確認した後、マージ・イシュー #10のクローズは明示的な承認に基づいて行う。
現時点ではPRの本人確認・マージ・クローズは未実施。README確認とPR作成の依頼を、マージ・クローズの承認とは扱わない。

段階9の整理後、段階10はshield・Devicetree・KconfigでLCDドライバを選び、Display APIで単色表示する最小動作から始める。

## 確認状況と未確認事項

| 項目 | 状況 |
|---|---|
| 公式RMの版・GPIO本文照合 | Codexが本人提供PDF Rev. 7, 2025-10-10と照合済み。[節・ページと結果](REGISTER_REFERENCE.md)。本人の読解・実機結果とは区別 |
| SDK／Zephyrの定義照合 | Codexがレジスタ型・オフセット・bit 10の動作・端子・ドライバ初期設定を確認 |
| Codexのビルド・テスト | [事前検証記録](evidence/README.md)参照 |
| 本人の環境確認 | ZEPHYR_BASE、west 1.5.0、採用Zephyr SHA、作業ブランチの4項目の一致を本人の提供表示で確認（2026-11-01受領） |
| 本人のソース確認 | MMIO・許可表の理解とregister_access.h/.cの確認完了報告を受領（2026-11-07）。register_commands.cは未理解との本人報告。理解確認を保留し先へ進める依頼を受領。後続ファイルは役割を説明し、確認完了報告は未受領 |
| 本人の外部OSS照合 | ファイル照合PASS・終了コード0との本人報告を受領（2026-11-07）。出力全文・公式アーカイブ同時照合の結果は未受領 |
| 本人のビルド | 166/166リンク完了・終了コード0、本人用09_register_access・CPU0。Flash 34,436 B／RAM 8,744 B／SRAMX 0 B（2026-11-07受領） |
| 本人用生成物のCodex照合 | CMakeCache・設定・GPIO0アドレス／LED端子・ELFの使用量／MMIO書込命令・runnerを読み取り確認。本人による生成設定の表示確認とは区別 |
| 本人の今回の接続先 | COM5／MCU-Link VCom Port、MCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTを本人の列挙表示で確認（2026-11-07受領） |
| 本人の書き込み | Flash成功表示・0x8684 B（34,436 B）ロード・終了コード0を提供抜粋で確認（2026-11-07受領）。ビルド先・プローブの選択行自体は抜粋に含まれない |
| 本人の起動・初期LED・help | 起動表示・mcxn947>、赤色LED消灯、helpの使い方表示とプロンプト復帰を確認できたとの本人報告を受領（2026-11-07）。UART原文・端末設定画面は未受領 |
| 本人の初期レジスタ読み出し | reg infoの6アドレス・許可方向が一致。PDOR=0x00000400／PDDR=0x00000400／PDIR=0x00000449を提供UARTで確認、LED消灯維持の本人報告も受領（2026-11-07） |
| 本人のPCOR・PSOR書き込み | PCOR 2回／PSOR 1回はすべてOK、PDOR readは0→0→0x400。点灯→点灯維持→消灯を本人が目視報告（2026-11-07受領） |
| 本人のPTOR・値0 | PTOR 2回のOK応答・PDOR 0→0x400と点灯→消灯を確認。消灯状態でのPSOR／PCOR／PTORへの値0はすべてOK、PDOR 0x400・消灯を維持（2026-11-07受領） |
| 本人の不正入力・正常復帰 | 9件のERRORを提供UARTで確認。全件のプロンプト復帰・LED消灯維持、最後のPDOR 0x400・help正常応答を本人が報告（2026-11-07受領）。最後のread／helpの全文は未受領 |
| 本人のGDB照合 | 本人の依頼により省略（2026-11-07）。未実施・結果未受領。今回の作業内容・完了条件から除外 |
| Flash／RAM使用量 | 本人の値を手順2と本人記録へ保存。Codexの事前ビルド値とは別に記録し、今回は同じ使用量 |
| W1C割り込みフラグ・読み出し副作用の実機試験 | 対象外。資料は補助として保持し、深掘りをZephyr学習の必須条件にはしない。出力クリアと混同しない |
| UART異常・キュー満杯復帰、実行時最大スタック・ヒープ | 実機未確認／未測定 |
| README確認 | 6項目を本人が確認し、実際の実習内容と一致していると報告（2026-11-08受領）。コミット・プッシュ・PR作成も依頼済み |
| PR確認、マージ、イシューclose | 未実施。本人のPR確認と明示的な承認後に進める |

## 参照した一次資料

- 本人提供のMCXNP184M150F70RM、Rev. 7, 2025-10-10：[識別情報と本文照合](REGISTER_REFERENCE.md)。
- [NXP MCX N製品資料一覧](https://www.nxp.com/products/MCX-N94-N54-N53-N52-N24)：以前のWeb取得ではログインが必要で本文未取得だったが、その後に本人提供PDFで照合した。
- 採用NXP HALの`mcux/mcux-sdk-ng/devices/MCX/MCXN/periph/PERI_GPIO.h`：
  `GPIO_Type`、PDOR／PSOR／PCOR／PTOR／PDIR／PDDR／ISFRの各フィールド。
- 同`MCXN947/MCXN947_cm33_core0_COMMON.h`：Secure／Non-secure GPIO0のベース。
- 採用Zephyrの`boards/nxp/frdm_mcxn947/frdm_mcxn947.dtsi`、CPU0 dtsi、
  `dts/arm/nxp/mcx/nxp_mcxnx4x_common.dtsi`、`drivers/gpio/gpio_mcux.c`。
  いずれも上記固定コミットのローカル実物を参照した。
