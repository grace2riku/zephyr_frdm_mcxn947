# 段階10：Consoleの文字入出力と標準Shell

対象：[イシュー #28](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/28)。
開始日：2026-11-08（JST）。作業ブランチ：`codex/stage10-console-shell`。

## 現在の状態

2026-11-08に予定した実機操作と、本人確認用6項目の記録確認が完了した。
段階10の学習・記録確認は完了。設定・コード全体の理解完了とは区別する。
最終実機状態は赤色LED消灯・点滅停止。2026-11-08に本人から変更のコミット・プッシュ・PR作成を依頼され、実装コミット`fc4a5d5`をプッシュし、[PR #31](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/31)を作成した。PRはOPEN・未マージ。
イシューは本人のクローズ依頼・許可を受けるまでOPENを維持し、PRマージも本人の別途の確認・承認後に行う。

Codexが`console/`に最小教材を準備した。2026-11-08に本人のCPU0向けビルド成功・書き込み成功・各終了コード0を確認した。同日に本人から、起動案内が表示され、Enter前に入力した`a`が1回だけ表示されたとの報告を受領した。Consoleの最小文字入出力は本人確認済み。ソース・設定の理解完了は未確認。
Console版を保持し、別アプリ`shell/`に標準Shellの最小`hello`コマンドを準備した。
標準Shell最小版も2026-11-08に本人のCPU0向けビルド・Flash書き込み成功・各終了コード0を確認済み。同日に本人からhelloによる挨拶表示・zephyr>への復帰を確認したとの報告を受領した。
本人はhelloとcmd_helloの対応は登録マクロで指定し、cmd_接頭辞だけで登録されるものではないと理解した旨を報告した。設定全体の理解完了とは区別する。
次にhelloを保持したままLEDのon／offを追加した。LED版も本人の182/182リンク完了・終了コード0とFlash書き込み成功・終了コード0を確認済み。
2026-11-08に本人からled onでLED ON表示・赤色LED点灯、led offでLED OFF表示・消灯、両方ともzephyr>へ復帰したとの報告を受領した。このon／off確認時点の最終状態は消灯。
既存の登録済みledコマンドでヘルプ・Tab補完・履歴を本人確認済み。履歴では↑キーでled --helpが表示され、Enterを押さずCtrl+Cで取り消したとの報告を2026-11-08に受領した。
次に`led blink <ms>`とLEDスレッド・キューを準備した。点滅版も本人の183/183リンク完了・終了コード0とFlash書き込み成功・終了コード0を確認済み。
本人がled blink 500の応答・プロンプト復帰・赤色LED点滅と、約10秒の入力待ち中の点滅継続を確認済み。250 msへの変更後も応答・プロンプト復帰を確認し、500 ms時より速くなったと報告した。
led offによるLED OFF表示・プロンプト復帰・消灯維持も本人確認済み。最新の本人報告時は赤色LED消灯・点滅停止。設定・処理担当の対応を整理し、本人確認用6項目が実際の学習・観察結果と一致することの確認も受領した。学習・記録確認の完了を、PRマージ・イシュークローズの承認とは扱わない。
各操作の結果を本人から受け取ってから次へ進む。

段階0〜9の成果は保持する。段階9の`register_commands.c`の理解保留とGDB照合の省略は引き継ぎ、再説明・再実行を開始条件にしない。

以下の段階別手順と使用量は、今回の実習時点の履歴を保持したもの。現行の`shell/`は点滅間隔指定まで含む最終版であり、再ビルドするとこの最終版が生成される。helloのみ・on／offのみの当時のビルド出力は、記載した本人用の別フォルダに保持している。`console/`は最小文字入出力版のまま保持する。

## 継続する環境と設定

- WindowsネイティブのPowerShell、段階1の`scripts/env.ps1`を継続する。
- ボード：`frdm_mcxn947/mcxn947/cpu0`。
- Zephyr 4.4.2：`dccb09599635bdff17633fa7e9dab014b91dce90`。
- SDK 1.0.1、Python 3.12.10、west 1.5.0、CMake 3.31.8、LinkServer 1.4.85。環境の再選定はしない。
- ボード既定の`zephyr,console`と`zephyr,shell-uart`は、ともに`flexcomm4_lpuart4`を指す。overlayは追加しない。
- `CONFIG_CONSOLE_GETCHAR=y`がUART割り込みを選択し、Consoleが入力を担当する。アプリからUART受信APIを直接呼ばない。
- `CONFIG_SHELL=n`、`CONFIG_CONSOLE_GETLINE=n`、`CONFIG_CONSOLE_HANDLER=n`。NT-Shellはリンクしない。
- `printk()`は起動・エラー表示に使い、受信した文字は`console_putchar()`で返す。
- 今回の本人による列挙表示でCOM5／MCU-Link HN2APUBRDCEPTを確認済み（2026-11-08）。
- 端末の予定設定は115200・8N1・フロー制御なし・ローカルエコーOFF。最初は英字1文字を手入力し、Enterは押さない。

`console_init()`は内部でDevicetreeのConsoleデバイスと準備状態を確認する。
`console_getchar()`は入力がないと待機し、受信すると0〜255、エラー時は負値を返す。
そのため戻り値は`int`で受け、負値を確認してから`console_putchar()`へ渡す。
出力の戻り値も負値をエラーとして確認する。

この最小版は受信したバイトをそのまま返す。行編集・コマンド解釈・改行変換は追加していない。
文字列の貼り付けや受信バッファ超過の網羅試験は今回の操作に含めない。

## 最初の本人操作：Console版をビルドする

使う機能はwestとKconfig。目的はConsole版だけをCPU0向けに生成すること。
本人が変える点はビルド対象を`console/`へ指定すること。見える結果はリンク完了と終了コード0。
この操作では実機の書き込みを行わない。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
Set-Location $env:ZEPHYR_BASE
$consoleBuild = Join-Path $zephyrWorkspace 'build/10_console'
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\10_zephyr_shell\console" -d $consoleBuild
$LASTEXITCODE
```

2026-11-08に本人から160/160リンク完了・終了コード0を受領した。
出力先は`C:/Users/grace/zephyrproject-mcxn947/build/10_console`、使用量はFlash 28,772 B・RAM 4,592 B。
Codexが本人用CMakeCache・.config・Devicetreeを読み取り照合し、対象アプリ・CPU0・採用Zephyr／SDK、Console入力有効・Shell等の受信競合無効、UART4の指定を確認した。
これは本人のビルド結果とCodexの生成物照合であり、実機での入出力確認とは区別する。

## 本人操作：今回の接続先を確認する（確認済み）

- **使うZephyr機能**：Consoleで使うUARTと、west flashで使うLinkServer接続の対応。
- **使う目的**：今回のボードのCOM番号とプローブ識別子を確認する。
- **本人が変える点**：コード変更はなし。FRDM-MCXN947のMCU-Link USB（J17）を接続し、列挙する。
- **見える動作**：COM番号とMCU-Linkの識別子が表示される。書き込みはまだ行わない。

ビルドしたPowerShellで実行する。

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
LinkServer probes --no-boot
```

2026-11-08に本人から、COM5／MCU-Link VCom Port (COM5)、プローブ1番／MCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTの表示を受領した。
保存済みの接続先と一致する。今回の本人の実行結果に基づく確認であり、前段階の値の流用ではない。
Codexは本人用runners.yamlを読み取り、LinkServerの対象がMCXN947:FRDM-MCXN947／cm33_core0、Flashの配置が0x10000000であることを確認した。

## 本人操作：Console版を書き込む（成功確認済み）

- **使うZephyr機能**：west flashとLinkServer runner。
- **使う目的**：本人がビルドしたConsole版をCPU0の内蔵Flashへ書き込む。
- **本人が変える点**：実機で動くアプリをConsole版へ切り替える。確認済みのプローブシリアルを指定する。
- **見える動作**：Flash書き込み成功表示と終了コード0。

ビルド・接続先確認に使ったPowerShellで実行する。

```powershell
$consoleBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_console'
west -v flash -d $consoleBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

2026-11-08に本人から`Finished writing Flash successfully.`、`Loaded 0x7064 bytes in 262ms`、終了コード0の表示抜粋を受領した。
0x7064は28,772 Bで、本人ビルドのFlash使用量およびCodexが読み取った本人用zephyr.binの長さと一致する。
system resetとboot ROM stall用watchpointの表示も受領した。アプリの実際のUART起動・文字入出力は次の操作で確認する。
Codexは実機へ書き込んでいない。

## 本人操作：Consoleで英字を1文字入力する（確認済み）

- **使うZephyr機能**：`printk()`の起動表示、`console_getchar()`と`console_putchar()`。
- **使う目的**：Consoleが受け取った1文字を返す動作を実機で確認する。
- **本人が変える点**：Tera TermをCOM5・115200・8N1・フロー制御なし・ローカルエコーOFFにし、英字`a`を1回入力する。コード変更はない。
- **見える動作**：RESET後に起動案内が表示され、Enterを押さずに`a`が1回だけ表示される。

1. Tera Termで上記の接続・設定を確認する。
2. ボードのRESETを押して離し、下記の起動案内2行を確認する。Zephyrのブートバナーが先に出てもよい。
3. Tera Termの端末画面で`a`を1回だけ入力する。Enterは押さない。

2026-11-08に本人から「起動案内が表示され、Enterを押す前に、入力したaが1回だけ表示されました」との報告を受領した。
本人の一致報告に基づく動作確認であり、UART原文や端末設定画面の提供は受けていない。端末設定画面自体の照合と区別する。

実機での期待表示は以下。これは期待値であり、観測結果ではない。

```text
Stage 10: Console character echo
Type one letter without Enter.
a
```

最後の`a`は本人がEnterを押さずに手入力し、ボードが返す文字。

## 本人操作：標準Shell最小版をビルドする（成功確認済み）

- **使うZephyr機能**：標準Shell、UARTバックエンド、`SHELL_CMD_ARG_REGISTER()`、`shell_print()`。
- **使う目的**：文字を返すConsoleから、コマンドを実行するShellへ進む。
- **本人が変える点**：ビルド対象を別アプリ`shell/`へ切り替える。Console版のファイルと出力先は保持する。
- **見える動作**：この操作ではリンク完了・終了コード0。後の実機確認では`zephyr> `で`hello`とEnterを入力すると挨拶が返る。

`shell/prj.conf`は`CONFIG_SHELL=y`・`CONFIG_SHELL_BACKEND_SERIAL=y`。
Console文字入力を起動しないよう`CONFIG_CONSOLE_SUBSYS=n`・`CONFIG_CONSOLE_HANDLER=n`とし、NT-Shell・独自UART受信をリンクしない。
UART受信は標準Shellだけが担当する。`CONFIG_CONSOLE=y`・`CONFIG_UART_CONSOLE=y`はConsoleの出力経路も使える設定であり、これだけでConsole文字入力が起動するわけではない。
Devicetreeの`zephyr,shell-uart`はConsole版と同じUART4を指す。二つのアプリは別々にビルド・書き込みし、同時に動かさない。

`SHELL_CMD_ARG_REGISTER(hello, NULL, "Print a greeting.", cmd_hello, 1, 0)`でコマンド名と処理を登録する。
必須引数数1はコマンド名`hello`自体を含むため、追加引数なしで使う。
入力の受信・行確定・解析はShell、挨拶の出力は`cmd_hello()`が担当する。
`main()`が戻った後もShell用スレッドが入力を待つ。

同じPowerShellで次だけを実行し、ビルド末尾と終了コードを受領してから次へ進む。

```powershell
$shellBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_shell'
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\10_zephyr_shell\shell" -d $shellBuild
$LASTEXITCODE
```

後の実機での期待表示。本人の観測結果ではない。

```text
zephyr> hello
Hello from Zephyr Shell.
zephyr>
```

Codexは採用Zephyrのshell_moduleサンプル、コマンド登録API、ShellバックエンドKconfigを照合して準備した。
hello最小版の時点ではLED制御を実装していない。on／offの追加は後の節で区別する。

CodexのShell版事前ビルドは2026-11-08に182/182リンク完了・終了コード0。
出力先は本人用と分けた`build-10-shell-codex/`。段階1の環境スクリプトを読み込み、リポジトリ直下から次を実行した。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    '.\stages\10_zephyr_shell\shell' -d '.\build-10-shell-codex'
```

生成.configのShell・UARTバックエンド・UART割り込み有効、Console入力サブシステム・別受信ハンドラ・MCUmgr・Log無効、
DevicetreeのUART4、CMakeCacheの対象アプリ・CPU0・採用Zephyr／SDK、mapのhelloコマンド登録を照合済み。
これはCodexの準備・検証であり、本人の実施結果とは区別する。

2026-11-08に本人からShell版の182/182リンク完了・終了コード0を受領した。
本人用出力先は`C:/Users/grace/zephyrproject-mcxn947/build/10_shell`。Flash 54,984 B・RAM 8,424 BでCodex事前ビルドと一致した。
Codexが本人用CMakeCache・.config・Devicetreeを読み取り、対象アプリ・CPU0・採用Zephyr／SDK、ShellとUARTバックエンド・割り込み有効、Console入力・MCUmgr等の受信競合無効、UART4の指定を照合した。
本人用runners.yamlのLinkServer対象MCXN947:FRDM-MCXN947／cm33_core0／Flash 0x10000000と、zephyr.binの長さ54,984 Bも読み取り確認した。
Shell版の本人書き込み結果とhello実行結果は後段に記録する。

| 対象 | Shell版の本人・Codex各ビルド使用量 | MCXN947全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 54,984 B | 2,097,152 B | 2.62% |
| SRAM | 8,424 B | 524,288 B | 1.61% |

ビルド表示のRAM 2.57%はCPU0割当320 KiBが分母。SRAMX・IDT_LISTは0 B。静的使用量で、実行時最大使用量ではない。

## 本人操作：標準Shell最小版を書き込む（成功確認済み）

- **使うZephyr機能**：west flashとLinkServer runner。
- **使う目的**：本人がビルドした標準Shell版をCPU0の内蔵Flashへ書き込む。
- **本人が変える点**：実機で動くアプリをConsole版からShell版へ切り替える。確認済みのプローブを指定する。
- **見える動作**：Flash書き込み成功表示と終了コード0。

同じPowerShellで実行する。今回確認済みのCOM5／HN2APUBRDCEPTを継続し、接続先の再列挙は繰り返さない。

```powershell
$shellBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_shell'
west -v flash -d $shellBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

2026-11-08に本人から`Finished writing Flash successfully.`、`Loaded 0xD6C8 bytes in 364ms`、終了コード0の表示抜粋を受領した。
0xD6C8は54,984 Bで、Shell版の本人ビルド使用量と本人用zephyr.binの長さに一致する。
system reset・boot ROM stall用watchpointの表示も受領した。ShellのUART起動・hello実行は次の操作で確認する。
Console版のソースとビルド出力は保持する。Codexによる実機書き込みは行っていない。

## 本人操作：標準Shellでhelloを実行する（確認済み）

- **使うZephyr機能**：標準Shellの入力・コマンド登録・ハンドラ呼び出しと`shell_print()`。
- **使う目的**：登録したコマンドがEnterで実行されることを確認する。
- **本人が変える点**：コード変更なし。Tera Termで`hello`を入力してEnterを押す。
- **見える動作**：`zephyr> `プロンプトと挨拶が表示され、実行後にプロンプトへ戻る。

1. Tera Termを今回のCOM5で開く。115200・8N1・フロー制御なし・ローカルエコーOFFを継続する。
2. ボードのRESETを押して離し、`zephyr> `の表示を確認する。
3. `hello`と入力してEnterを1回押す。

期待表示（本人の実行結果ではない）：

```text
zephyr> hello
Hello from Zephyr Shell.
zephyr>
```

2026-11-08に本人から、helloを入力してEnterを押すと「Hello from Zephyr Shell.」が表示され、zephyr>へ戻ることを確認したとの報告を受領した。
本人の一致報告に基づく確認であり、UART原文は未受領。コマンド登録の理解報告も受領した（[QA-057](../../docs/qa.md#段階10の補足)）。

## 本人操作：LED on／off追加版をビルドする（成功確認済み）

- **使うZephyr機能**：Shellサブコマンド、`DT_ALIAS(led0)`、`GPIO_DT_SPEC_GET()`、GPIO API。
- **使う目的**：入力した`led on`／`led off`をGPIO操作につなぐ。
- **本人が変える点**：Shell版の`CONFIG_GPIO=y`とLEDコマンドの追加。`hello`とConsole版は保持する。
- **見える動作**：今回はリンク完了と終了コード0。後の実機確認では内蔵赤色LEDが点灯／消灯する。

`led0`はボード定義の赤色LED（GPIO0・pin 10・Active Low）を参照する。overlayは不要。
`SHELL_STATIC_SUBCMD_SET_CREATE()`でon／offをまとめ、`SHELL_CMD_REGISTER(led, ...)`で親コマンドへ接続する。
`cmd_led_on()`は論理値1、`cmd_led_off()`は論理値0を`gpio_pin_set_dt()`へ渡す。Active Lowの反転はGPIO APIへ任せる。
最初のLEDコマンドで`gpio_is_ready_dt()`を確認し、`gpio_pin_configure_dt(..., GPIO_OUTPUT_INACTIVE)`を一度実行する。
GPIO出力設定と状態変更の戻り値を確認し、失敗時はShellへエラーを返す。
このon／off版ではLEDの準備・操作をShellのコマンド処理だけが担当し、mainとの初期化競合を作らない。起動時のLED状態はこの版の確認項目に含めない。
このon／off版の時点では点滅・LEDスレッド・キューは未追加。後の点滅版で処理担当を分ける。

同じPowerShellでビルドだけを実行する。出力先を`10_shell_led`に分け、本人確認済みhello最小版の`10_shell`も保持する。

```powershell
$shellLedBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_shell_led'
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\10_zephyr_shell\shell" -d $shellLedBuild
$LASTEXITCODE
```

この版はソース・ビルド出力がそろっても、本人の実機動作確認とは数えない。本人ビルド結果を受けてから書き込みへ進む。
CodexのLED版事前ビルドは2026-11-08に182/182リンク完了・終了コード0。
出力先は`build-10-shell-led-codex/`。段階1の環境スクリプト読込み後、リポジトリ直下で次を実行した。

```powershell
west build -b frdm_mcxn947/mcxn947/cpu0 `
    '.\stages\10_zephyr_shell\shell' -d '.\build-10-shell-led-codex'
```

生成.configでGPIO・Shell・UARTバックエンドが有効、Console入力・MCUmgr等の受信競合が無効、LEDドライバが未有効であることを確認した。
生成Devicetreeのled0→赤色LED／gpio0 pin 10／Active Lowと、mapのledサブコマンドを照合済み。実機動作は未確認。

2026-11-08に本人からLED版のCPU0向け182/182リンク完了・終了コード0を受領した。
本人用出力先は`C:/Users/grace/zephyrproject-mcxn947/build/10_shell_led`。Flash 55,424 B・RAM 8,424 BでCodex事前ビルドと一致した。
Codexが本人用CMakeCache・.config・Devicetreeを読み取り、対象アプリ・CPU0・採用Zephyr／SDK、GPIO・Shell・UART割り込みの有効、受信競合無効、led0→赤色LED／GPIO0 pin 10／Active Lowを照合した。
本人用runners.yamlのLinkServer対象MCXN947:FRDM-MCXN947／cm33_core0／Flash 0x10000000と、zephyr.binの長さ55,424 Bも読み取り確認した。
これは本人のビルド結果とCodexの生成物照合であり、実機でのLED確認とは区別する。

| 対象 | LED on／off版の本人・Codex各ビルド使用量 | MCXN947全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 55,424 B | 2,097,152 B | 2.64% |
| SRAM | 8,424 B | 524,288 B | 1.61% |

ビルド表示のRAM 2.57%はCPU0割当320 KiBが分母。SRAMX・IDT_LISTは0 B。静的使用量であり、実行時最大使用量ではない。

## 本人操作：LED on／off追加版を書き込む（成功確認済み）

- **使うZephyr機能**：west flashとLinkServer runner。
- **使う目的**：本人がビルドしたLED版をCPU0の内蔵Flashへ書き込む。
- **本人が変える点**：実機で動くアプリをhello最小版からLED on／off追加版へ切り替える。
- **見える動作**：Flash書き込み成功表示と終了コード0。

同じPowerShellで実行する。今回確認済みのプローブを継続し、再列挙は繰り返さない。

```powershell
$shellLedBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_shell_led'
west -v flash -d $shellLedBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

2026-11-08に本人から`Finished writing Flash successfully.`が表示され、終了コード0だったとの報告を受領した。
本人の成功報告に基づく確認であり、今回のロード量の表示抜粋は未受領。LEDの点灯・消灯確認とは区別する。

## 本人操作：led on／led offで赤色LEDを操作する（確認済み）

- **使うZephyr機能**：標準ShellのサブコマンドとGPIO API。
- **使う目的**：入力コマンドと赤色LEDの点灯・消灯を対応づける。
- **本人が変える点**：コード変更なし。端末で`led on`、続いて`led off`を実行する。
- **見える動作**：`LED ON`と赤色LED点灯、`LED OFF`と消灯。各コマンド後にプロンプトへ戻る。

Tera TermはCOM5・115200・8N1・フロー制御なし・ローカルエコーOFFを継続する。
RESETを押して離し、`zephyr> `を確認する。その後、次を1行ずつ入力してEnterを押す。
onの結果を端末と基板で確認してからoffへ進む。

```text
led on
led off
```

期待表示（本人の実行結果ではない）：

```text
zephyr> led on
LED ON
zephyr> led off
LED OFF
zephyr>
```

2026-11-08に本人から、led onでLED ON表示・赤色LED点灯、led offでLED OFF表示・消灯、各実行後にzephyr>へ戻ることを確認したとの報告を受領した。
本人の応答・目視の一致報告に基づく確認であり、UART原文は未受領。最後は消灯との報告。
`LED ON`／`LED OFF`はGPIO APIが成功した際の応答であり、実際の発光は本人の目視で確認する。

## 本人操作：ledのヘルプを表示する（表示確認済み）

- **使うZephyr機能**：標準Shellのヘルプと登録済み説明文。
- **使う目的**：コマンド登録時の説明文・サブコマンドがヘルプへ反映されることを確認する。
- **本人が変える点**：コード変更・ビルド・書き込みなし。同じTera Termで`led --help`を入力する。
- **見える動作**：`Control the red LED.`、on／offとそれぞれの説明が表示され、zephyr>へ戻る。

現在のプロンプトで次だけを入力し、Enterを押す。RESETは不要。

```text
led --help
```

表示の根拠はshell/src/main.cの`SHELL_CMD_REGISTER(led, ...)`とon／offの`SHELL_CMD_ARG(...)`に渡した説明文。
Codexは本人用.configのSHELL_HELPと、採用ソースの--help解析を照合した。
2026-11-08に本人から次のヘルプ表示を受領し、登録した説明文との一致を確認した。

```text
zephyr> led --help
led - Control the red LED.
Subcommands:
  on   : Turn the red LED on.
  off  : Turn the red LED off.
```

## 本人操作：Tabでoffを補完する（確認済み）

- **使うZephyr機能**：標準ShellのTab補完。
- **使う目的**：登録済みサブコマンド名を入力途中から補えることを確認する。
- **本人が変える点**：コード変更・ビルド・書き込みなし。同じTera Termで入力とTab操作を行う。
- **見える動作**：`led of`が`led off`に補完される。

1. 現在のプロンプトで`led of`まで入力し、Enterを押さずTabを1回押す。
2. 入力行が`led off`になったかを確認する。末尾に空白が付いてもよい。
3. Enterで実行せず、Ctrl+Cで入力行を取り消す。

Tab補完は登録コマンド名を参照する。今回のoffは過去にも実行済みだが、履歴の検索と同じ仕組みだと扱わない。
Codexは本人用.configのSHELL_TAB・SHELL_TAB_AUTOCOMPLETION有効と採用Shell資料を確認した。
2026-11-08に本人から`led off`へ補完されたとの報告を受領した。Tab補完の本人確認済み。Ctrl+Cでの取消し結果は個別には未報告。

## 本人操作：上矢印キーで入力履歴を呼び出す（確認済み）

- **使うZephyr機能**：標準Shellの履歴。
- **使う目的**：以前実行したコマンドを打ち直さずに入力行へ戻す。
- **本人が変える点**：コード変更・ビルド・書き込みなし。上矢印キーを1回押す。
- **見える動作**：過去に実行したコマンドが入力行へ表示される。

1. 入力行が空の`zephyr> `で上矢印キーを1回押す。補完した入力が残っていれば、先にCtrl+Cで取り消して空にする。RESETはしない。
2. 呼び出されたコマンド名を確認し、Enterは押さずCtrl+Cで取り消す。

今回の手順どおりなら直前に実行した`led --help`が候補となる。別の操作を行っていた場合は、その実行履歴に応じたコマンドが表示される。
Tabは登録コマンド名に基づく補完、上矢印キーは実行したコマンドの履歴を呼ぶ操作として区別する。
Codexは本人用.configのSHELL_HISTORY有効と採用Shell資料の矢印キー操作を照合した。
2026-11-08に本人から、↑キーでled --helpが表示され、Enterを押さずCtrl+Cで取り消したとの報告を受領した。

## 本人操作：点滅間隔指定版をビルドする（成功確認済み）

- **使うZephyr機能**：Shell引数、`shell_strtoul()`、GPIO API、LEDスレッド、`k_msgq`、完了通知のセマフォ。
- **使う目的**：点滅の待機をLED担当へ分け、Shellが次の入力を受けられるようにする。
- **本人が変える点**：blinkサブコマンドと`led_control.c/.h`を追加し、on／offもLED担当への要求にする。
- **見える動作**：今回はリンク完了・終了コード0。後の実機確認では`led blink 500`で500 msごとに反転し、プロンプトへ戻る。

段階7で使用したLEDスレッド・キュー・GPIO反映完了の通知パターンを、段階10の独立した`led_control.c/.h`へ取り入れた。段階7のファイルは変更しない。
初回のLEDコマンドでGPIOの準備状態・出力設定を確認してLEDスレッドを起動し、その後のGPIO更新・mode・間隔・論理状態はLEDスレッドだけが担当する。
Shellはモードと間隔を要求構造体としてキューへコピーし、一度のGPIO反映結果をセマフォで受け取って戻る。点滅間隔の待機や点滅ループはShellで実行しない。
LEDスレッドは点滅中なら`k_msgq_get(..., K_MSEC(interval_ms))`で要求か時間満了を待つ。時間満了でGPIOを反転し、点灯／消灯固定中は`K_FOREVER`で次の要求を待つ。
周期的なUART出力はしない。点滅中のGPIOエラーは点滅を止めて保存し、次のLEDコマンドでShellへ表示する。

`led blink <ms>`は100〜2000 msの指定を必須にする。500 msは点灯／消灯の切り替え間隔で、点灯→消灯→点灯の名目上の1周期は約1000 ms。
引数数はShellの登録で確認し、文字列を`shell_strtoul()`で10進数へ変換して範囲を確認する。
`led on`は点滅を止めて点灯固定、`led off`は点滅を止めて消灯する。`hello`・ヘルプ・Tab補完・履歴・Console版は保持する。

同じPowerShellでビルドだけを実行する。出力先は`10_shell_blink`に分け、本人確認済みon／off版の`10_shell_led`も保持する。

```powershell
$shellBlinkBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_shell_blink'
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\10_zephyr_shell\shell" -d $shellBlinkBuild
$LASTEXITCODE
```

本人のビルド・書き込み・実機での点滅・間隔変更・入力待ち中の継続の結果は、以降の記録に分けて残す。
Codexの点滅版事前ビルドは2026-11-08にCPU0向け183/183リンク完了・終了コード0。
出力先は`build-10-shell-blink-codex/`。段階1の環境スクリプト読込み後、リポジトリ直下で次を実行した。

```powershell
west build -b frdm_mcxn947/mcxn947/cpu0 `
    '.\stages\10_zephyr_shell\shell' -d '.\build-10-shell-blink-codex'
```

生成.config・CMakeCacheでGPIO・Shell・UARTバックエンド・マルチスレッド有効、受信競合無効、対象アプリ・CPU0・採用Zephyr／SDKを確認した。
mapでblinkハンドラ・LEDスレッド・キュー・完了通知を照合済み。ビルドと生成物照合は実機での点滅確認とは区別する。

2026-11-08に本人から点滅版のCPU0向け183/183リンク完了・終了コード0を受領した。
本人用出力先は`C:/Users/grace/zephyrproject-mcxn947/build/10_shell_blink`。Flash 56,460 B・RAM 10,728 BでCodex事前ビルドと一致した。
Codexが本人用CMakeCache・.config・mapを読み取り照合し、対象アプリ・CPU0・採用Zephyr／SDK、GPIO・Shell・マルチスレッドと受信競合無効、blink・LED担当・キュー・完了通知を確認した。
本人用runners.yamlのLinkServer対象MCXN947:FRDM-MCXN947／cm33_core0／Flash 0x10000000と、zephyr.binの長さ56,460 Bも読み取り確認済み。
これは本人のビルド結果とCodexの生成物照合であり、実機での点滅確認とは区別する。

| 対象 | 点滅版の本人・Codex各ビルド使用量 | MCXN947全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 56,460 B | 2,097,152 B | 2.69% |
| SRAM | 10,728 B | 524,288 B | 2.05% |

ビルド表示のRAM 3.27%はCPU0割当320 KiBが分母。SRAMX・IDT_LISTは0 B。静的使用量であり、実行時最大使用量ではない。

## 本人操作：点滅間隔指定版を書き込む（成功確認済み）

- **使うZephyr機能**：west flashとLinkServer runner。
- **使う目的**：本人がビルドした点滅版をCPU0の内蔵Flashへ書き込む。
- **本人が変える点**：実機のアプリをLED on／off版から点滅間隔指定版へ切り替える。
- **見える動作**：Flash書き込み成功表示と終了コード0。

同じPowerShellで実行する。今回確認済みのプローブを継続し、接続先の再列挙は繰り返さない。

```powershell
$shellBlinkBuild = 'C:\Users\grace\zephyrproject-mcxn947\build\10_shell_blink'
west -v flash -d $shellBlinkBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

2026-11-08に本人から`Finished writing Flash successfully.`と表示され、終了コード0だったとの報告を受領した。
本人の成功報告に基づく確認であり、今回のロード量の表示抜粋は未受領。実機での点滅確認とは区別する。

## 本人操作：500 ms間隔で点滅させる（確認済み）

- **使うZephyr機能**：Shellのblinkコマンド、メッセージキュー、LEDスレッド、GPIO API。
- **使う目的**：点滅開始と、Shellの入力待ち中もLED担当が動くことを確認する。
- **本人が変える点**：コード変更なし。`led blink 500`を入力する。
- **見える動作**：`LED BLINK toggle_ms=500`を表示してzephyr>へ戻り、赤色LEDが点滅し続ける。

Tera Termは今回のCOM5・既存設定を継続する。プロンプトが見えない場合はRESETを押して離して表示させる。
次を入力してEnterを押す。

```text
led blink 500
```

期待表示（本人の実行結果ではない）：

```text
zephyr> led blink 500
LED BLINK toggle_ms=500
zephyr>
```

500 msは点灯・消灯の切り替え間隔で、名目1周期は約1秒。厳密な計時は今回の確認項目にしない。
プロンプトへ戻った状態で10秒ほど何も入力せず、点滅が続くかを見る。
2026-11-08に本人から、LED BLINK toggle_ms=500表示・zephyr>への復帰、赤色LED点滅、何も入力せず約10秒待っても点滅継続を確認したとの報告を受領した。
本人の一致・目視報告に基づく確認であり、厳密な間隔の計測やUART原文の提供は求めていない。

## 本人操作：点滅間隔を250 msへ変える（確認済み）

- **使うZephyr機能**：Shell引数、メッセージキューによる間隔指定、LEDスレッド。
- **使う目的**：点滅中に新しい間隔をLED担当へ渡し、速度を変える。
- **本人が変える点**：コード変更なし。引数を500から250へ変える。
- **見える動作**：`LED BLINK toggle_ms=250`表示とプロンプト復帰、500 ms時より速い点滅。

現在の点滅を続けたまま、同じTera Termで次を入力してEnterを押す。RESET・ビルド・書き込みは不要。

```text
led blink 250
```

250 msも切り替え間隔で、名目1周期は約500 ms。500 ms時との目視での速度比較を行い、厳密な計時は必須にしない。
2026-11-08に本人から、応答・プロンプト復帰と、500 ms時より点滅が速くなったことを確認したとの報告を受領した。
本人の一致・目視報告に基づく確認であり、UART原文や厳密な切り替え間隔の計測結果は未受領。

## 本人操作：led offで点滅を止める（確認済み）

- **使うZephyr機能**：Shellからキューで渡す停止要求、LEDスレッド、GPIO API。
- **使う目的**：点滅中のLED担当へ停止を指示し、消灯させる。
- **本人が変える点**：コード変更なし。点滅中に`led off`を入力する。
- **見える動作**：`LED OFF`表示とプロンプト復帰。点滅が止まり、赤色LEDが消灯したままになる。

現在の250 ms点滅中に、同じTera Termで次を入力してEnterを押す。RESET・ビルド・書き込みは不要。

```text
led off
```

応答とプロンプト復帰、点滅停止・消灯を確認する。数秒見ても点滅を再開しないかを観察する。
これは点滅版のモードを切り替える操作であり、先のon／offだけの版で成功済みの操作を形式のために繰り返すものではない。
2026-11-08に本人から、LED OFF表示・プロンプト復帰、赤色LED消灯と数秒たっても点滅を再開しないことを確認したとの報告を受領した。
本人の一致・目視報告に基づく確認。最新の実機状態は点滅停止・消灯。

## 設定・API・処理担当の対応

| 対象 | 設定から今回の動作へのつながり |
|---|---|
| Consoleの入力先 | ボードのzephyr,consoleはflexcomm4_lpuart4。Console版はCONSOLE_SUBSYS・CONSOLE_GETCHARを有効、SHELLを無効にしてconsole_getcharが文字を受ける |
| Shellの入力先 | zephyr,shell-uartも同じUART4。Shell版はSHELL・SHELL_BACKEND_SERIALを有効、CONSOLE_SUBSYS・CONSOLE_HANDLERを無効にする。UARTを受信するのはShellだけ。NT-Shell・独自UART受信はリンクしない |
| GPIOとLED | led0→赤色LED／gpio0 pin 10／Active Low。CONFIG_GPIOでドライバを有効にし、gpio_is_ready_dt・gpio_pin_configure_dt・gpio_pin_set_dtで準備・出力設定・操作を行う。論理値1が点灯、0が消灯で、極性の反転はAPIが担当する |
| ShellとLED担当 | Shellが入力・引数を処理し、modeとinterval_msをキューへコピーする。LEDスレッドがGPIOと点滅状態を更新し、要求か切り替え時刻まで待つ。Shellは一度のGPIO反映をセマフォで確認して戻り、点滅間隔を待ち続けない |

上記の生成設定・Devicetree・リンク結果はCodexが本人用ビルドで照合済み。本人による生成ファイルの手動照合や、設定・コード全体の理解完了を報告済みとして扱わない。

## 本人確認済みの実施結果

以下は本人の実行・観察報告に基づく記録。2026-11-08に本人から、実際の学習・観察結果と一致するか確認したとの報告を受領した。

1. Console版で起動案内と、Enter前に入力したaが1回返ることを確認した。Console版のコードとビルド出力を保持した。
2. 標準Shell版でhelloによる挨拶表示・プロンプト復帰を確認し、コマンド名と関数は登録マクロで対応づけると理解した。
3. led on／offの表示と赤色LEDの点灯／消灯、プロンプト復帰を確認した。
4. led --helpの説明文、led ofのTab補完、↑キーでled --helpの履歴呼出しとCtrl+C取消しを確認した。
5. led blink 500の点滅と約10秒の入力待ち中の継続、250 msへの変更後に速くなったことを確認した。厳密な間隔計測の結果は未受領。
6. 点滅中のled offで消灯し、数秒たっても点滅を再開しないことを確認した。最新の実機状態は消灯・点滅停止。

ソース・設定・処理担当全体の理解完了は未確認であり、コマンド登録の理解報告や実機動作確認とは区別する。
不正入力・境界値・GPIOエラー・キュー満杯等の網羅試験、GDB、RAM・レジスタコマンドの移植はこの実習では実施していない。追加の必須条件にはしない。
UART原文・端末画面を受け取った範囲と、本人の一致・目視報告に基づく範囲は各項目に記録した。未受領の画面や厳密な計測を追加提出の条件にしない。
段階9の理解保留・GDB省略は保持する。上の本人確認用6項目と実際の学習・観察結果の一致確認を受領した。全ソース・設定の理解完了としては扱わない。
2026-11-08に本人の依頼で実装コミット`fc4a5d5`をプッシュし、[PR #31](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/31)を作成した。マージ・イシュークローズは本人の別途の確認・承認後に行う。

## 準備・検証と本人の実習記録

| 実施者 | 項目 | 状態 |
|---|---|---|
| Codex | AGENTS.md・ロードマップ・QA-045・環境記録・#28を確認 | 2026-11-08確認済み |
| Codex | 開始時のGit状態 | main、b14bf17、作業ツリーcleanを確認後に作業ブランチを作成 |
| Codex | 採用ZephyrのSHA・変更状態とConsoleソース照合 | SHA一致・変更なし。APIヘッダー、Kconfig、getchar.c、公式echoサンプル、ボード定義を確認 |
| Codex | Console版の事前ビルド・生成設定照合 | 2026-11-08、CPU0向け160/160リンク完了・終了コード0。Console入力・UART割り込みが有効、Shell・行入力・別受信ハンドラ・MCUmgr・Logが無効。DevicetreeのUART4／115200とCMakeCacheの採用Zephyr・SDK・対象アプリも照合済み |
| 本人 | ソース・設定確認 | 未確認 |
| 本人 | Console版ビルド | 2026-11-08、160/160リンク完了・終了コード0。本人用build/10_console、Flash 28,772 B・RAM 4,592 B |
| Codex | 本人用ビルド生成物の照合 | 2026-11-08、CMakeCache・.config・Devicetreeを読み取り確認。対象アプリ・CPU0・採用Zephyr／SDKとConsole入力・受信担当が想定と一致 |
| 本人 | 今回の接続先確認 | 2026-11-08、COM5／MCU-Link VCom Port、プローブ1番／FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTの列挙表示を受領 |
| Codex | 本人用書き込み設定の照合 | runners.yamlのLinkServer対象MCXN947:FRDM-MCXN947／cm33_core0／Flash 0x10000000を読み取り確認 |
| 本人 | Console版の書き込み | 2026-11-08、Flash成功表示・0x7064 Bロード・終了コード0を受領 |
| Codex | 本人用バイナリの長さ照合 | zephyr.binは28,772 Bで、本人のロード量・ビルド使用量と一致 |
| 本人 | 起動表示・1文字入出力 | 2026-11-08、起動案内が表示され、Enter前にaが1回だけ表示されたとの本人報告を受領 |
| 本人 | 端末設定画面 | 未受領。案内した設定での動作確認報告と画面自体の照合は区別 |
| Codex | 標準Shell最小版 | 別アプリshell/へhelloコマンドだけを準備。2026-11-08、182/182リンク完了・終了コード0、生成設定・Devicetree・CMakeCache・map照合済み |
| 本人 | 標準Shell版のビルド | 2026-11-08、182/182リンク完了・終了コード0。本人用build/10_shell、Flash 54,984 B・RAM 8,424 B |
| Codex | 本人用Shellビルド生成物の照合 | CMakeCache・.config・Devicetree・runners.yaml・zephyr.binを読み取り照合。CPU0・採用環境・Shell受信担当・書き込み先・54,984 Bが想定と一致 |
| 本人 | 標準Shell版の書き込み | 2026-11-08、Flash成功表示・0xD6C8 Bロード・終了コード0を受領。54,984 Bで本人ビルド・バイナリ長と一致 |
| 本人 | 標準Shell最小版のhello実行 | 2026-11-08、hello＋Enterで挨拶表示・zephyr>へ復帰したとの本人報告を受領 |
| 本人 | コマンド登録の理解 | helloとcmd_helloの対応は登録マクロで指定し、cmd_接頭辞は命名慣例であると理解した旨を報告。設定全体の理解完了は未確認 |
| Codex | LED on／off版の準備 | helloを保持してサブコマンド・GPIO APIを追加。2026-11-08、182/182リンク完了・終了コード0。生成設定・LED定義・map照合済み |
| 本人 | LED on／off版のビルド | 2026-11-08、182/182リンク完了・終了コード0。本人用build/10_shell_led、Flash 55,424 B・RAM 8,424 B |
| Codex | 本人用LED版の生成物照合 | CMakeCache・.config・Devicetree・runners.yaml・zephyr.binを読み取り照合。CPU0・採用環境・受信担当・LED定義・書き込み先・55,424 Bが想定と一致 |
| 本人 | LED on／off版の書き込み | 2026-11-08、Flash成功表示・終了コード0だったとの本人報告を受領。ロード量の表示抜粋は未受領 |
| 本人 | LED on／offの応答・目視確認 | 2026-11-08、LED ON表示と点灯、LED OFF表示と消灯、両方のプロンプト復帰を本人が報告。最終状態は消灯 |
| 本人 | Shellのヘルプ | 2026-11-08、led --helpの表示を受領。led・on・offの説明文が登録内容と一致 |
| 本人 | ShellのTab補完 | 2026-11-08、led offへ補完されたとの報告を受領。取消し操作は個別には未報告 |
| 本人 | Shellの履歴 | 2026-11-08、↑キーでled --helpが表示され、EnterなしでCtrl+C取消しを行ったとの報告を受領 |
| Codex | 点滅間隔指定版の準備 | LED担当スレッド・キュー・反映完了通知、led blink 100..2000 msを追加。2026-11-08、183/183リンク完了・終了コード0。生成設定・CMakeCache・map照合済み |
| 本人 | 点滅間隔指定版のビルド | 2026-11-08、183/183リンク完了・終了コード0。本人用build/10_shell_blink、Flash 56,460 B・RAM 10,728 B |
| Codex | 本人用点滅版の生成物照合 | CMakeCache・.config・map・runners.yaml・zephyr.binを読み取り照合。CPU0・採用環境・受信担当・LED担当・キュー・完了通知・書き込み先・56,460 Bが想定と一致 |
| 本人 | 点滅版の書き込み | 2026-11-08、Flash成功表示・終了コード0だったとの本人報告を受領。ロード量の表示抜粋は未受領 |
| 本人 | 500 ms点滅・入力待ち中のLED継続 | 2026-11-08、toggle_ms=500表示・プロンプト復帰・赤色LED点滅、約10秒の入力待ち中も継続したとの報告を受領 |
| 本人 | 250 msへの点滅間隔変更 | 2026-11-08、応答・プロンプト復帰と、500 ms時より速い点滅を本人が報告。UART原文・厳密な計時は未受領 |
| 本人 | 点滅停止 | 2026-11-08、LED OFF表示・プロンプト復帰・消灯と数秒後も点滅再開なしを本人が報告。最終状態は消灯・点滅停止 |
| 本人 | READMEと実際の学習・観察結果の一致確認 | 2026-11-08、本人確認用6項目について実際の学習・観察結果と一致するか確認したとの報告を受領 |

Console版のCodex事前ビルドはリポジトリ内の`build-10-console-codex/`に分離し、本人用の`build/10_console`は使用していない。
段階1の環境スクリプトを読み込み、リポジトリ直下で次のコマンドを実行した。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    '.\stages\10_zephyr_shell\console' -d '.\build-10-console-codex'
```

| 対象 | Codexの事前ビルド使用量 | MCXN947全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 28,772 B | 2,097,152 B | 1.37% |
| SRAM | 4,592 B | 524,288 B | 0.88% |

ビルド表示のRAM 1.40%はCPU0割当320 KiBが分母。SRAMX・IDT_LISTは0 B。
静的なビルド使用量であり、実行時最大使用量ではない。Codexによる実機接続・書き込み・入出力確認は行っていない。
本人の結果はCodexの検証と区別し、PRのマージ・イシューのクローズは本人の確認・承認後に行う。

## 学習に使用したAIモデル

| 利用期間（JST） | AIツール | AIモデル（モデルID） | AIモデルバージョン | 推論設定 | 確認根拠 |
|---|---|---|---|---|---|
| 2026-11-08〜 | Codex | 未確認 | 未取得 | 未確認 | このチャットで識別子・固定スナップショット・画面の設定を未照合 |

記録方法は[AIモデルの記録方針](../../docs/ai-models.md)を参照。
