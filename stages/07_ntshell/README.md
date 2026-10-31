# 段階7：NT-ShellでLEDをコマンド操作する

対象：[イシュー #8](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/8)。前提：[段階6](../06_uart_echo/README.md)。
開始日：2026-10-30（JST）。作業ブランチ：`codex/stage7-ntshell`。
成果物：[PR #25](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/25)（2026-10-31作成、本人のPR確認待ち）。

## 目的と現在の状態

外部OSSをCMakeへ組み込み、UARTの入出力コールバックとコマンド解析を学ぶ。
Tera Termから`help`、`led on`、`led off`、`led blink`を実行し、入力待ち中も点滅が続くことを本人が確認する。

Codexが課題#8のOPEN、ローカルmainの変更なしを確認し、作業ブランチを作成した。
公式NT-Shell 0.3.1の取り込み、入出力、コマンド、LED制御と実習手順を準備した。
CodexのCPU0向けビルドは終了コード0、Flash 31,376 B・RAM 10,864 B。
コマンド解析のQEMUテストは7件成功。実機動作とは区別し、[事前検証結果](evidence/README.md)へ記録した。
2026-10-31（JST）に本人の表示結果を受領し、ZEPHYR_BASE、west 1.5.0、採用Zephyr SHA、作業ブランチの4項目が期待値と一致することを確認した。
本人から`main.c`のコードを確認したとの報告も受領した（2026-10-31）。初期化・コールバック登録・シェル開始の説明を提示済み。理解の再説明を求める確認は行っていない。
続いて本人から`commands.c`のコードと処理の流れを確認したとの報告を受領した（2026-10-31）。
続いて`led_control.h/.c`の共通API、指示キュー、セマフォによる完了通知と点滅待機を説明し、本人からコード確認完了の報告を受領した（2026-10-31）。
本人から関数ポインタの登録・呼出しの仕組みが分かり、`serial_port.c`も確認済みとの報告を受領した（2026-10-31）。
本人から図で関数の分類・関係性が分かり、CMakeLists.txtとprj.confも確認済みとの報告を受領した（2026-10-31）。
本人からNT-Shellヘッダ先頭の版・著作権・MIT許諾表示と、`verify_vendor.py --archive`の16ファイル一致・終了コード0を受領した（2026-10-31）。
本人のCPU0向けビルドは164/164のリンク完了・終了コード0で成功し、Flash 31,376 B・RAM 10,864 Bを確認した（2026-10-31受領）。Codexが本人用生成物のCMakeCache・.config・Devicetree・mapも読み取り照合した。
本人の生成.config表示も受領し、UART割り込み・GPIO有効、CONSOLE・CONSOLE_SUBSYS・SHELL・PRINTK・LOG無効、mainスタック4096 Bを確認した（2026-10-31）。
本人の接続先列挙でCOM5／MCU-Link VCom Portと、MCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTを確認した（2026-10-31受領）。
本人の通常モードの書き込みコマンドは終了コード0だったが、成功表示が非表示だったため詳細モードで再確認した。
本人の`west -v flash`で`Finished writing Flash successfully.`、31,376 Bのロード、終了コード0を確認した（2026-10-31受領）。
本人からTera Termで期待どおりの起動表示と`mcxn947>`、赤色LEDの消灯を確認したとの報告を受領した（2026-10-31）。
本人から、案内したhelp・led on・led offの各コマンドの動作を確認できたとの報告を受領した（2026-10-31）。
本人から、待機中・入力途中も点滅が続き、led blink 250で速くなること、led onで点滅停止・点灯維持、led offで消灯することを確認したとの報告を受領した（2026-10-31）。
本人から、案内した各不正入力でERROR・Usage表示とプロンプト復帰、同じ速さの点滅継続、最後のhelpの正常応答とled offによる消灯を確認したとの報告を受領した（2026-10-31）。
本人から空行・空白、Backspaceによる編集、上矢印履歴、Ctrl+Cによる取消し、取消したled onで点灯しないこと、その後のhelp正常応答を確認したとの報告を受領した（2026-10-31）。
本人からCR・LF・CR+LFの各方式でhelpが1回だけ実行されてプロンプトへ戻り、最終設定を送信CR+LF・受信CRへ戻したとの報告を受領した（2026-10-31）。
本人からREADMEの内容に問題ないとの確認を受領した（2026-10-31）。
本人のREADME確認後、Codexが実装・記録をコミット`5cd48bd`としてプッシュし、2026-10-31にPR #25を作成した。
**段階7の基本的な実機確認と本人のREADME確認は完了。最終LED状態は消灯、送信改行CR+LF・受信改行CR。PR #25は本人の確認待ち。マージとイシューのクローズは未実施。**
イシューのチェック項目はまだ更新していない。

本人がビルド・書き込み・端末操作・実機確認を行い、Codexが理由を説明し、結果を確認しながらソースと記録を整える。
実機確認・README確認後にコミット・プッシュ・PR作成へ進む。マージとイシューのクローズは本人の承認後に行う。

## 完了条件との照合（基本実機・READMEは本人確認済み）

| イシュー #8の完了条件 | 現在の根拠 |
|---|---|
| NT-Shellのプロンプトとhelpが使える | 本人が期待する起動表示・mcxn947>とhelpのUsage表示を確認 |
| 各LEDコマンドが期待どおり動く | 本人がon／off、既定blink、250 msへの速度変更、blinkからon／offへの切替えを確認 |
| 不正コマンドや不正引数に使い方を表示する | 本人が7種類の不正入力でERROR・Usage・プロンプト復帰と、点滅の継続を確認 |
| 操作待ち中もLED点滅が継続する | 本人が無入力時とhelpを入力してEnterを押す前の待機中の継続を確認 |
| READMEにバージョン・手順・期待動作・結果・未確認事項を記録する | 本README、UPSTREAM、Codex事前検証と本人の書き込みログへ記録済み。本人がREADMEの内容に問題ないと確認（2026-10-31受領） |

空行・空白、Backspace、履歴、Ctrl+C取消しとその後の正常応答、CR／LF／CR+LFの各1回実行も本人確認済み。
実機確認は本人の報告に基づき、UART全文・設定画面そのものの提供や厳密な周期測定と区別する。
UARTエラー／キュー満杯からの復帰、追加の入力長・数値境界ケースは以下に未確認として残す。
これらの未実施ケースを基本実機確認の成功と混同しない。本人のREADME確認は完了し、PR確認・マージ・クローズは別途の段階として扱う。

## 学習に使用したAIモデル

| 利用期間（JST） | AIツール | AIモデル（モデルID） | AIモデルバージョン | 推論設定 | 確認根拠 |
|---|---|---|---|---|---|
| 2026-10-30〜2026-10-31（確認した保存済みturn_contextの範囲） | OpenAI Codex（デスクトップ） | gpt-6.1-sol | モデルID中の6.1表記。固定スナップショットID・内部ビルド番号は未取得 | high | 実行環境のCODEX_THREAD_IDと一致する保存済みチャット`01a18d11-32c0-7c53-baaf-a30975b21a2a`を2026-10-31に再確認。turn_context 24件のmodel／effortはすべて同じ値。チャット送信時刻の測定とは区別 |

[記録方針](../../docs/ai-models.md)に従う。セッション本文・ログ全体はコピーしていない。

## 使用環境と接続

[段階0の固定環境](../00_versions/versions.md)を継続する。本人のZEPHYR_BASE・west・Zephyr SHA・作業ブランチは2026-10-31に再確認済み。ほかのツールの今回の本人による実測とは区別する。

| 項目 | 採用値／確認範囲 |
|---|---|
| ボード | FRDM-MCXN947、`frdm_mcxn947/mcxn947/cpu0` |
| Zephyr | 4.4.2、`dccb09599635bdff17633fa7e9dab014b91dce90`。Codexが開始時のローカルHEADと変更なしを確認 |
| SDK／Python／west／CMake | 1.0.1／3.12.10／1.5.0／3.31.8を継続 |
| NT-Shell | 公式配布パッケージ0.3.1。取得元・SHA-256・ライセンスは[UPSTREAM.md](UPSTREAM.md) |
| USB／プローブ | J17のオンボードMCU-Linkを継続する方針。今回の本人の列挙でMCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTを確認済み（2026-10-31受領） |
| UART | 今回の本人の列挙でCOM5／MCU-Link VCom Portを確認済み（2026-10-31受領）。`zephyr,console`のUARTをアプリから直接使用。115200 baud、8N1、フロー制御なしを使用する方針。端末設定の今回の本人確認はこれから。独自overlayなし |
| 端末 | Tera Term 5.4.0を使用する方針。最終の送信CR+LF・受信CRは本人が明示確認（2026-10-31）。ローカルエコーOFFで実習するよう案内し、編集とコマンド動作は本人確認済み。設定画面そのものと通信条件各値の今回の提供は未受領 |
| LED | `led0`の赤色LED。GPIO APIの論理値1で点灯、0で消灯（Active Lowの反転はDevicetreeに従う） |
| 本人用ビルド先 | `C:\Users\grace\zephyrproject-mcxn947\build\07_ntshell` |
| Codexの事前ビルド先 | リポジトリ内の`.tmp/build/07_ntshell_codex`。本人用とは別 |

## ファイルと処理のつながり

```text
UART割り込み
  serial_cb → uart_fifo_read → rx_bytes（1 Bを最大256件）
                                      ↓
main: ntshell_execute → serial_read → NT-Shellの編集・履歴処理
                                      ↓ Enter
                command_callback → ntopt_parse → dispatch
                                      ↓
                led_control_apply → led_requests → LEDスレッド
                                      ↑                 ↓ GPIO更新
                                  led_doneで結果を待つ

mainの表示 → serial_write → uart_poll_out → Tera Term
```

- `src/main.c`：初期化、起動表示、`ntshell_init`とプロンプト設定、シェル実行。
- `src/serial_port.c`：ISRはFIFOを読み、待たずにバイトをキューへコピーする。送信やコマンド実行は行わない。mainのreadコールバックは入力を待ち、LEDスレッドへCPUを譲る。
- `src/commands.c`：行の長さを検査し、`ntopt_parse`で引数へ分ける。引数を検査して共通LED関数へ依頼する。
- `src/led_control.c`：初期GPIO設定後はLEDスレッドだけがGPIOと点滅状態を更新する。mainは指示の処理完了をセマフォで待ち、成功したときだけOKを表示する。
- `third_party/ntshell/`：未変更の上流ソース。CMakeの`target_sources`と`target_include_directories`で組み込む。

段階6のキューは「完成行」だった。今回はBackspaceや矢印などもNT-Shellが処理できるよう「受信バイト」を送る。
`ntshell_t`は静的領域へ置く。mainの要求スタック4096 B、LEDスレッド2048 B、優先度はmain=6・LED=5。
LEDスレッドは点滅中に指示または切り替え時刻を待ち、点灯・消灯中は次の指示を待つ。

## コマンドと期待動作

| 入力 | 期待する結果 |
|---|---|
| `help` | Usageと全コマンドを表示し、プロンプトへ戻る |
| `led on` | 点滅を停止し、赤色LEDを点灯。`OK: led on` |
| `led off` | 点滅を停止し、赤色LEDを消灯。`OK: led off` |
| `led blink` | すぐ点灯し、500 msごとにON/OFF切り替え。名目上の1周期1000 ms |
| `led blink 250` | すぐ点灯し、250 msごとに切り替え。名目上の1周期500 ms |
| `led blink 100`／`led blink 2000` | 引数の下限・上限を受け付ける |
| `led blink 99`、`led blink 2001`、`led blink -100`、`led blink abc` | ERRORとUsage。LEDへの指示を送らず、直前の動作を継続 |
| `unknown`、`led`、`led on extra`、`help extra` | ERRORとUsage。直前のLED動作を継続 |
| 空行／空白だけ | LEDを変更せず、プロンプトへ戻る |

大文字・小文字は区別する。数値は10進数字のみ。単位・符号・小数点は受け付けない。
blinkの再指定ではすぐ点灯して新しい切り替え間隔で開始する。時間はOSの待機による名目値であり、厳密な周期は未測定。

### 入力と出力の方針

UART FIFOの受信者はアプリのISRだけ。NT-Shellはキュー経由で受け取る。送信はmainだけが行う。
`CONFIG_CONSOLE`、`CONFIG_UART_CONSOLE`、`CONFIG_CONSOLE_SUBSYS`、Zephyr標準の`CONFIG_SHELL`、`CONFIG_PRINTK`、`CONFIG_LOG`、`CONFIG_BOOT_BANNER`を無効にし、UART出力をNT-Shellとアプリの応答に統一する。
NT-Shellを使うことと、Zephyr標準Shellを有効にすることは別である。

CRで行を確定。CR直後のLFは読み捨て、LF単独はCRへ変換する。Backspace、矢印キー、履歴、Ctrl+Cは上流の処理を使う。
英数字・ASCIIコマンドを対象とし、日本語コマンドは対象外。

上流の編集領域は64 Bで、保持できる本文は63 B。上限後の入力を黙って無視するため、
このアプリでは**確定時に63 Bの満杯行を拒否し、最大62 Bまで**を受け付ける。
上流の編集機能は残しているため、Backspaceで短くした行は編集後の長さで判定する。

UARTエラーまたは256 Bキューの満杯を検出した場合、未処理バイトを捨て、VT解析と編集中の行をキャンセルする。
ERROR表示後は**まずEnterを1回押し、その後コマンドを入力し直す**。欠落した行の途中をコマンドとして実行しない。
この異常復帰経路とキュー満杯は実機未確認。高速貼り付け時の無損失を保証しない。
点滅中のGPIO書き込みエラーは自動切り替えを止め、次のコマンド時に通知する。GPIO失敗後の実際のLED状態は不明と扱う。

## 本人の操作手順

一度に全部を進めず、各操作の結果を確認してから次へ進む。

### 1. 環境と作業ブランチの確認（本人確認済み）

新しいPowerShellで次を実行する。環境スクリプトはこのPowerShellだけのPATH・変数を設定する。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
Set-Location $env:ZEPHYR_BASE
$ntshellBuild = Join-Path $zephyrWorkspace 'build\07_ntshell'
$env:ZEPHYR_BASE
west --version
git rev-parse HEAD
git -C $learningRepo branch --show-current
```

期待値はZEPHYR_BASEが`C:\Users\grace\zephyrproject-mcxn947\zephyr`、west 1.5.0、
Zephyr SHAが`dccb09599635bdff17633fa7e9dab014b91dce90`、学習リポジトリのブランチが`codex/stage7-ntshell`。

本人から2026-10-31に4項目の実行結果を受領し、すべて一致することを確認した。
受領日を記録しており、操作した正確な日時は未取得。本人のソース確認・ビルド・実機結果とは区別する。

### 2. ソース・設定と外部OSSの確認

`main.c` → `commands.c` → `led_control.c` → `serial_port.c`の順に、上の処理図と対応を確認する。
CMakeLists.txt、prj.confとUPSTREAM.mdも確認する。各関数・設定の理由は実習中に説明する。

2026-10-31に`main.c`の初期化順序とコールバック登録を説明し、本人からコード確認済みの報告を受領した。
続いて`commands.c`の行解析、引数検査、LED共通関数への依頼を説明し、本人からコードと処理の流れを確認済みとの報告を受領した（2026-10-31）。
続いて`led_control.h/.c`の共通API、指示キュー、セマフォによる処理完了通知、点滅時の待機を説明し、本人からコード確認完了の報告を受領した（2026-10-31）。
続いて`serial_port.c`のISR受信、バイトキュー、NT-Shellのread／write、CR/LFと入力欠落時の復帰を説明した。
本人から関数ポインタの登録・呼出しの仕組みが分かり、`serial_port.c`のコードも確認済みとの報告を受領した（2026-10-31）。
本人の依頼に基づく関数の所属・呼出し・実行文脈の整理図は[QA-043](../../docs/qa.md#qa-043-supplement)に記録した。
本人から図で関数の分類・関係性が分かり、CMakeLists.txt・prj.confも確認済みとの報告を受領した（2026-10-31）。
UPSTREAM.mdの取得元・パッケージ版・SHA-256・ライセンス表示の保持と、取り込みファイルのハッシュ照合を案内した。
本人から`ntshell.h`先頭32行の表示と、`verify_vendor.py --archive`の`PASS: 16 NT-Shell files match the official archive and checksums`、終了コード0を受領した（2026-10-31）。
配布アーカイブのSHA-256と取り込み16ファイルの内容一致を、本人の検証結果で確認済み。UPSTREAM.md本文の表示結果自体は未受領として区別する。次は本人のビルドへ進む。

```powershell
python "$learningRepo\stages\07_ntshell\scripts\verify_vendor.py"
```

期待結果：`PASS: 16 NT-Shell files match checksums`。

### 3. ビルドと生成設定の確認（本人確認済み）

実行場所はZEPHYR_BASE。`-p always`は指定した**段階7専用ビルド先**を作り直す。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\07_ntshell" -d $ntshellBuild
$LASTEXITCODE
```

リンク完了、終了コード0、Flash／RAM使用量の表示を確認する。

本人から164/164リンク完了、Zephyr 4.4.2、CPU0、本人用`build/07_ntshell`への生成、終了コード0の表示を受領した（2026-10-31）。
Flash 31,376 B・RAM 10,864 B・SRAMX 0 B・IDT_LIST 0 B。Codexの事前ビルドとは別の本人の実行結果である。
Codexが本人用CMakeCacheの段階7アプリ・prj.conf・CPU0・採用Zephyr・SDKコンパイラ、.config、生成Devicetreeとmapを読み取り照合した。
本人による以下の生成設定表示も受領し、期待する値と一致した（2026-10-31）。

```powershell
Select-String -Path "$ntshellBuild\zephyr\.config" `
    -Pattern 'CONFIG_(SERIAL|UART_INTERRUPT_DRIVEN|UART_MCUX_LPUART|GPIO|CONSOLE|CONSOLE_SUBSYS|SHELL|PRINTK|LOG|MAIN_STACK_SIZE)(=| )'
```

UART割り込み・GPIO有効、console／shell／printk／log無効、mainスタック4096 Bを確認する。

本人の提供表示でMAIN_STACK_SIZE=4096、SERIAL・UART_INTERRUPT_DRIVEN・UART_MCUX_LPUART・GPIO=y、
LOG・CONSOLE・CONSOLE_SUBSYS・PRINTK・SHELLの無効化を確認済み。

### 4. 接続先の再確認と書き込み

まず接続先を表示する。段階6のCOM番号・プローブ番号を今回の結果として流用しない。

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
LinkServer probes --no-boot
```

本人からCOM5／MCU-Link VCom Port (COM5)と、プローブ1番のMCU-LINK FRDM-MCXN947 (r0E7)、
CMSIS-DAP V3.128、シリアルHN2APUBRDCEPTの表示を受領した（2026-10-31）。今回の接続先を確認済み。
Codexが本人用runners.yamlのLinkServer、MCXN947:FRDM-MCXN947、cm33_core0と、採用runnerの--probeがシリアル番号を受け付けることも読み取り確認した。

対象がFRDM-MCXN947のオンボードMCU-Linkであることを確認後、本人が実行する。

```powershell
west -v flash -d $ntshellBuild --runner linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

今回は列挙したシリアル番号HN2APUBRDCEPTを指定する。Flash成功表示と終了コード0を記録する。

本人の最初の実行は`west flash`（-vなし）で、LinkServer v1.4.85、対象MCXN947:FRDM-MCXN947、
プローブHN2APUBRDCEPTの選択と終了コード0を確認した（2026-10-31受領）。
補助のプローブブート処理で`Failed to boot probes`と`UnicodeDecodeError`（byte 0x94、position 21）が表示された。
その後もプローブ選択・ローカルFW 3.133の更新候補案内へ進んでいる。FW更新実行の表示ではない。
最初の通常モードの提供ログにはFlash消去・書き込み・成功の表示がなく、その時点では成功表示の確認待ちとした。

採用`linkserver.py`のflash処理は、LinkServer 1.4.85では通常モードのstdoutをDEVNULLへ送る。
したがって成功表示がない理由として出力抑制があり、上の補助処理のエラーだけからFlash失敗を断定しない。
これは[段階1](../01_hello_world/README.md#書き込みとuart確認)にも記録済みの挙動。
Codexの最初の案内は成功表示確認に必要な-vを欠いていたため、上の手順を詳細モードへ訂正した。
本人へ同じビルド先・プローブを使う`west -v flash`での再実行を案内し、`Finished writing Flash successfully.`と終了コードを確認する。
段階5に同じ種類の補助処理エラーの後で書き込み成功した履歴はあるが、今回の成功根拠として流用しない。

その後、本人から詳細ログを添付で受領した（2026-10-31）。本人用`build/07_ntshell/zephyr/zephyr.bin`を
0x10000000へ指定し、`Finished writing Flash successfully.`、`Loaded 0x7A90 bytes`（31,376 B）、終了コード0を確認した。
`Sectors written: 0, unchanged: 4, total: 4`なので、今回の詳細モードの処理では既存の4セクタが一致し、追加セクタ書き込みはなかった。
補助処理のFailed to boot probes／UnicodeDecodeErrorも表示されたが、その後の書き込み処理の成功を確認できた。
FW更新実行の表示はない。添付原文を[本人の詳細書き込みログ](user-evidence/flash-verbose.log)へ無変更で保存し、ハッシュ一致を確認した。
UART起動・LED動作の確認はこれから行う。

### 5. 起動とコマンドの実機確認

Tera Termを今回のCOM、115200、8N1、フロー制御なし、ローカルエコーOFF、送信CR+LF・受信CRへ設定する。
受信準備後、RESETを押して離す。起動時のLEDは消灯。

```text
Stage 7: NT-Shell LED commands
NT-Shell package 0.3.1; local echo OFF; type help.
LED starts OFF; blink default toggle interval=500 ms.
mcxn947>
```

これは期待する表示例であり、実測ログではない。

本人からTera Termで期待どおりの起動表示と`mcxn947>`、赤色LEDの消灯を確認できたとの報告を受領した（2026-10-31）。
起動表示は本人の一致報告、LEDは本人の目視報告に基づく。UART表示の原文や端末設定値そのものの提供とは区別する。
続いて以下1のhelp・点灯・消灯を1コマンドずつ案内し、本人から各コマンドの動作を確認できたとの報告を受領した（2026-10-31）。
案内したUsage表示、led on／offの応答と赤色LEDの点灯・消灯、各コマンド後のプロンプト復帰を本人の報告に基づき確認済みとする。
UART表示原文は未受領。以下2〜3も案内し、本人から待機中・入力途中の点滅継続、led blink 250による速度上昇、led onによる点滅停止と点灯維持、led offによる消灯を確認したとの報告を受領した（2026-10-31）。
名目上の切り替え間隔と速度の目視変化を区別し、厳密な周期は未測定とする。
以下4としてled blink 250の後、unknown、led、led on extra、led blink 99／2001／-100／abcを案内した。
本人から各不正入力で`ERROR: invalid command or argument`とUsage、プロンプト復帰、同じ速さの点滅継続、最後のhelp正常応答とled offによる消灯を確認したとの報告を受領した（2026-10-31）。
エラー文は本人の報告中に引用されているが、UART出力全文は未受領。
以下5として空行・空白だけの入力、helxをBackspaceでhelpへ修正、上矢印で直前のhelpを再実行、未確定のled onをCtrl+Cで取り消す操作を案内した。
本人から空行・空白、編集・履歴・取消し、取消したled onでは点灯しないこと、最後のhelpが正常に応答することを確認したとの報告を受領した（2026-10-31）。
以下6も案内し、本人からCR・LF・CR+LFの3方式ともhelpが1回だけ実行されてプロンプトへ戻り、最終設定を送信CR+LF・受信CRへ戻したとの報告を受領した（2026-10-31）。
基本実機確認の結果を記録してREADME確認を案内し、本人から内容に問題ないとの確認を受領した（2026-10-31）。
Codexがコミット・プッシュし、PR #25を作成した。次は本人のPR確認へ進む。

1. `help`、`led on`、`led off`を手入力し、応答とLEDを確認する。
2. `led blink`後、約10秒入力せず点滅の継続を観察する。`help`の途中でEnterを押さず待ち、点滅が続くことも確認する。
3. `led blink 250`で速くなることを確認し、`led on`で点滅停止・点灯、`led off`で消灯を確認する。
4. 点滅中に上の不正コマンド・不正引数を試す。ERRORとUsage後も入力ができ、LEDが直前の動作を続けることを確認する。
5. 空行、Backspaceで`helx`を`help`へ修正、上矢印による履歴呼出し、Ctrl+Cによる編集中の行の取消しを確認する。
6. 送信CR単独・LF単独でも`help`が各1回実行されることを確認し、CR+LFへ戻す。

UART表示とLEDの目視、本人の操作・理解、厳密な周期測定を区別して記録する。

## 確認結果と未確認事項

| 項目 | 実施者／結果 |
|---|---|
| 公式配布物とライセンス | Codex：公式0.3.1を取得、SHA-256記録、16ファイルを原本と照合。ライセンス表示を保持 |
| 実装・事前検証 | Codex：[evidence/README.md](evidence/README.md)参照 |
| 本人の環境確認 | ZEPHYR_BASE・west 1.5.0・採用Zephyr SHA・作業ブランチの一致を本人の表示結果で確認（2026-10-31受領） |
| 本人のソース・設定確認 | `main.c`、`commands.c`、`led_control.h/.c`、`serial_port.c`、CMakeLists.txt・prj.confの確認済み報告を受領（2026-10-31）。関数ポインタの仕組みと、図で関数の分類・関係性が分かったと報告。ライセンス表示と本人の取り込み原本照合も確認済み（次行参照）。本人による再説明の確認は未実施 |
| 本人の外部OSS照合 | `ntshell.h`のパッケージ版0.3.1・著作権・MIT許諾表示、`verify_vendor.py --archive`の16ファイル一致・終了コード0を確認（2026-10-31受領）。UPSTREAM.md本文の表示結果自体は未受領 |
| 本人のビルド | 164/164リンク完了、Zephyr 4.4.2、CPU0、終了コード0。Flash 31,376 B・RAM 10,864 B・SRAMX 0 B（2026-10-31受領） |
| 本人用生成物のCodex照合 | CMakeCacheのアプリ・設定・CPU0・Zephyr・SDK、.configのUART割り込み・GPIO有効と競合機能無効、mainスタック4096 B・優先度6、DevicetreeのUART／LED参照、mapの使用量を読み取り確認。本人の表示確認とは区別する |
| 本人の生成設定表示 | MAIN_STACK_SIZE=4096、SERIAL・UART_INTERRUPT_DRIVEN・UART_MCUX_LPUART・GPIO=y、LOG・CONSOLE・CONSOLE_SUBSYS・PRINTK・SHELL無効を本人の表示結果で確認（2026-10-31受領） |
| 今回のCOM・プローブ | 本人の列挙でCOM5／MCU-Link VCom Port、MCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPTを確認（2026-10-31受領） |
| 本人の書き込み | 詳細モードで本人用zephyr.bin、0x10000000、HN2APUBRDCEPT、Flash成功表示、31,376 Bロードと終了コード0を確認（2026-10-31受領）。4セクタがunchanged。補助ブート処理のエラーとFW更新候補案内は残るが、後続の成功表示を確認済み。[本人ログ](user-evidence/flash-verbose.log) |
| NT-Shell起動・初期LED | 本人がTera Termの期待どおりの起動表示・mcxn947>と、赤色LED消灯を確認したと報告（2026-10-31受領）。起動UARTの原文は未受領。最終改行設定は下記の本人報告で確認 |
| help・LED点灯／消灯 | 本人が案内したhelp・led on・led offの各コマンドの動作を確認できたと報告（2026-10-31受領）。Usage・OK応答、点灯・消灯、プロンプト復帰を報告に基づき確認済み。UART原文は未受領 |
| LED点滅・速度変更・停止 | 本人がled blink 250で速くなること、led onで点滅停止・点灯維持、led offで消灯を確認したと報告（2026-10-31受領）。速度変化は目視、厳密な周期は未測定 |
| 入力待ち・入力途中の点滅継続 | 本人が案内した無入力約10秒・help入力途中でEnter前に約5秒待つ操作を行い、どちらも点滅が続くことを確認したと報告（2026-10-31受領）。正確な待機時間の測定ではない |
| 不正コマンド・引数 | 本人が案内した7種類の不正入力すべてでERROR・Usageとプロンプト復帰、同じ速度の点滅継続、最後のhelp正常応答とled off消灯を確認したと報告（2026-10-31受領）。UART全文は未受領 |
| 空行・編集・履歴・取消し | 本人が空行・空白、Backspace編集、上矢印履歴、Ctrl+C取消し、取消したled onで点灯しないこと、その後のhelp正常応答を確認したと報告（2026-10-31受領） |
| CR／LF／CR+LF | 本人が3方式ともhelpが各1回だけ実行され、プロンプトへ戻ることを確認したと報告。最終設定は送信CR+LF・受信CR（2026-10-31受領） |
| 追加の境界ケース | led blink 100／2000の受理、62／63 Bの行長境界はコマンド単体テスト済み。今回の本人の実機では未確認。99／2001等の拒否は本人確認済み |
| UARTエラー・キュー満杯からの復帰 | 実機未確認。自動テストのコマンド抑止確認と区別する |
| 周期・割り込み遅延・実行時最大スタック／ヒープ | 未測定 |
| 最終状態 | 最後のled offで消灯後、編集・改行試験による点灯なし。端末は送信CR+LF・受信CR |
| README確認 | 本人が内容に問題ないと確認（2026-10-31受領） |
| PR確認・マージ・イシュー | [PR #25](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/25)を2026-10-31に作成。本人のPR確認待ち。マージ承認・イシュー更新／クローズは未実施。PR本文はRefs #8として自動クローズ指定を付けていない |

### 本人のビルド時メモリ使用量

本人のビルド表示（2026-10-31受領）。Codexの事前ビルド結果は[evidence](evidence/README.md)へ別に記録しており、今回は同じ使用量だった。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| 内蔵Flash | 31,376 B | 2,097,152 B | 1.50% |
| 内蔵SRAM | 10,864 B | 524,288 B | 2.07% |

RAM 10,864 B＋SRAMX 0 B。IDT_LISTも0 B。CPU0の320 KiB領域を分母にするとRAM使用率は3.32%で、本人のビルド表示に一致する。
Codexが本人用mapの`_flash_used=0x7a90`、`_image_ram_size=0x2a70`も読み取り確認した。
ビルド時に確定した使用量であり、実行時最大スタック・ヒープやCPU使用率、実機動作の確認とは区別する。
