# 段階6：UARTのポーリング受信と割り込み受信でエコーを実装する

対象：[イシュー #7](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/7)。前提：[段階5](../05_uart_tx/README.md)。
開始日：2026-10-14（JST）。作業ブランチ：`codex/stage6-uart-echo`。
成果物：[PR #23](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/23)（OPEN、本人のPR確認と別途のマージ承認待ち）。

## 目的と現在の状態

文字単位のポーリングエコー → 行単位のポーリングエコー → 割り込み受信とスレッド処理の順に学ぶ。
行の上限、CR/LF、連続入力、入力超過時の動作を定義して確認する。
割り込み版ではUART受信待ち中も別スレッドの赤色LED点滅を継続する。

開始時、Codexがローカルの変更なしmain、HEAD・origin/main・GitHub mainの
`f4625b8e73920a0f3899213cbfa54af40139083e`一致、#7のOPENを確認した。
採用ZephyrのHEADと変更なしも読み取り確認済み。
Codexが文字単位版、行単位のポーリング版、割り込み受信とLEDスレッドの版を実装した。
2026-10-24（JST）に本人の環境確認結果を受領し、ZEPHYR_BASE・west 1.5.0・採用Zephyr SHAの一致を確認した。
同日に本人の文字単位版ビルド結果を受領し、Zephyr 4.4.2・CPU0向けリンク完了・終了コード0を確認した。
本人の生成.configで、UARTドライバ有効と競合する受信機能の無効化を確認した。
Codexも本人用ビルドのCMakeCache・.config・zephyr.dtsを読み取り照合した。Codexによるビルド実行ではない。
本人から段階6の文字単位版の起動表示と`abc123`のUART表示を受領した（2026-10-24）。
本人がローカルエコーOFF、Enter前から1文字ずつ1回だけ表示、Enter後に次行の先頭へ移動、
書き込み終了コード0を報告した。文字単位エコーの実機確認は完了。
今回のCOM・プローブ列挙結果は最終確認時に本人から受領した。通信条件・送受信改行の設定値の画面表示自体は未受領。
行単位版も本人のリンク完了・終了コード0を確認した（2026-10-24に結果受領）。
本人の抜粋にはECHO_MODEの表示がなかったため、CodexがCMakeCacheのECHO_MODE=lineとbuild.ninjaの
src/poll_line.cのコンパイル対象を読み取り確認した。
本人から行単位版でabc123と空行は期待どおり、31文字の貼り付け後は追加Enterにも無反応と報告を受領した。
RESET後に31文字をゆっくり手入力すると期待どおりとの追加報告を受領し、手入力の31 B上限は確認済み。
CodexがUART受信エラーの検査・解除・行破棄を追加した。本人が修正後の31 B正常受信、32 B超過エラー、
直後のok応答、31 B貼り付け時のUART RX overrun検出、RESETなしでのok応答を確認した。
行単位ポーリング修正版のビルド・書き込み終了コードは未受領だが、本人の起動表示・実機動作を確認済み。
割り込み版も本人のリンク完了・終了コード0を確認した（2026-10-24に結果受領）。
本人用ビルドのCMakeCacheでECHO_MODE=irq・prj_irq.conf、build.ninjaでirq_line.cをCodexが読み取り照合済み。
割り込み版の本人の起動表示と基本実機確認を受領し、入力なしでのLED継続、行入力中のLED継続、
空行・31 B貼り付け・32 B超過後のok、CR単独・LF単独を確認済みとした（2026-10-24）。
現在のボードは割り込み版。本人が3行と001〜100の連続入力、終了後のok応答を確認した。
この試験では満杯通知・欠番・UARTエラー表示はなく、キュー満杯の分岐自体は未確認。
ISRとキューの動作は本人の質問を受けて図解し、QA-038へ記録した。
続いて実際の関数名・変数名への対応とabc受信時の状態変化をQA-039に追加した。
本人がREADMEを確認し、内容に問題ないと報告した（2026-10-24）。本人による動作機構の再説明を求める確認は行っていない。
最終確認で割り込み版の書き込み終了コード0、COM5、プローブHN2APUBRDCEPT、100行入力前後のLED点滅継続も受領した。

本人がビルド、書き込み、端末操作、実機確認を実行し、Codexがソースと記録を編集する。
実機確認とREADME確認の後、Codexがコミット・プッシュ・PR作成をまとめて行う。
マージと#7のクローズは段階6について別途の明示的な承認を得てから行う。

## 最終確認（本人の記録確認済み・PR #23レビュー待ち）

| イシュー #7の完了条件 | 現在の根拠 |
|---|---|
| 文字単位・行単位のエコー | 本人のUART表示と操作結果で確認済み |
| 割り込み版の受信待ち中もLED点滅が続く | 本人の基本実機確認で確認済み。厳密な周期は未測定 |
| 入力上限を超えても停止しない | 32 Bの行長超過を通知し、次のokへ復帰。ポーリング版のUART overrun後の復帰も本人確認済み |
| READMEに環境・手順・期待動作・結果・問題と対処・未確認事項を記録 | 本READMEへ記録済み。2026-10-24に本人が内容に問題ないと確認 |

3行・100行の連続入力と最後のokも確認済み。今回の100行ではキュー満杯通知はなく、
満杯時の破棄・通知分岐そのものは実機未確認として残す。これを100行の試験成功と混同しない。
故意に満杯を起こすための追加ソース変更・再書き込みは、段階6の基本完了条件としては求めない。

本人がREADMEを確認し、内容に問題ないと報告した。あわせて割り込み版の書き込み終了コード0、
COM5（MCU-Link VCom Port）、MCU-LINK FRDM-MCXN947 (r0E7)／CMSIS-DAP V3.128／HN2APUBRDCEPT、
100行入力前後のLED点滅継続を報告した。
端末の通信条件・送受信改行設定の画面表示自体や、不明な過去の終了コードは推測で埋めない。
本人の依頼に従いCodexがコミット・プッシュし、PR #23を作成した。
本人によるPR内容の確認待ちで、マージとイシュー #7のクローズは別途の明示的な承認を待つ。

## 学習に使用したAIモデル

| 利用期間（JST） | AIツール | AIモデル（モデルID） | AIモデルバージョン | 推論設定 | 確認根拠 |
|---|---|---|---|---|---|
| 2026-10-14〜2026-10-24（確認したturn_contextの範囲） | OpenAI Codex（デスクトップ） | gpt-6.1-sol | モデルID中の6.1表記を確認。固定スナップショットIDは未取得、内部ビルド番号は未確認 | high | 開始時に本チャットID `01a135e5-ef86-7d41-a30c-05817bd21073` のsession_meta.id・cwdと実行環境のCODEX_THREAD_IDを照合。2026-10-24に保存済みturn_context 15件のmodel／effortを再確認し、すべて同じ値。確認範囲は2026-10-14 00:33:46〜2026-10-24 22:20:37 JST。利用開始・終了や質問送信時刻の測定とは区別 |

[AIモデルの記録方針](../../docs/ai-models.md)に従う。
確認元は`rollout-2026-10-14T00-33-44-01a135e5-ef86-7d41-a30c-05817bd21073.jsonl`のメタデータ。
チャット本文やセッションログ全体はコピーしていない。過去段階のモデル欄は変更しない。

## 使用環境と接続

[段階0の固定環境](../00_versions/versions.md)を継続する。採用値と今回の実測を区別する。

| 項目 | 採用値／今回の確認範囲 |
|---|---|
| ホスト／シェル | Windows／PowerShell 7.6.5を継続。本人の段階6の再確認は未実施 |
| ボード | FRDM-MCXN947、`frdm_mcxn947/mcxn947/cpu0` |
| Zephyr | 4.4.2／`dccb09599635bdff17633fa7e9dab014b91dce90`。開始時にCodexがローカルHEAD一致・変更なしを確認。本人のSHA一致も確認済み（2026-10-24に結果受領） |
| ZEPHYR_BASE | `C:\Users\grace\zephyrproject-mcxn947\zephyr`。本人の表示結果で一致を確認済み（2026-10-24に結果受領） |
| SDK／Python／west／CMake | 1.0.1／3.12.10／1.5.0／3.31.8を継続。west 1.5.0は本人の表示結果で確認済み（2026-10-24に結果受領）。ほかの今回の実測は未実施 |
| LinkServer | 1.4.85を継続。今回の実測は未実施 |
| USB／プローブ | J17を継続する方針。本人の最終列挙でMCU-LINK FRDM-MCXN947 (r0E7)、CMSIS-DAP V3.128、`HN2APUBRDCEPT`を確認済み。各書き込み前に再確認する |
| UART／端末 | 本人の最終列挙でCOM5／MCU-Link VCom Port (COM5)を確認。Tera Term 5.4.0、115200 baud、8N1、フロー制御なしを継続する方針。ローカルエコーOFF、CR/LF各設定での動作は本人確認済み。最終設定は送信CR+LF・受信CRを案内し、設定画面そのものは未受領 |
| 本人用ビルド先 | `C:\Users\grace\zephyrproject-mcxn947\build\06_uart_echo` |

採用コミットのボード定義で`zephyr,console`と`zephyr,shell-uart`はともに
`flexcomm4_lpuart4`、115200、CPU0ではokayと照合した。独自overlayは追加していない。

## 最初の文字単位版

`src/main.c`はUART取得・準備確認、起動表示、1バイト受信と同じバイトの送信だけを行う。
この版には行バッファもLEDスレッドもない。ASCII文字で最初の確認を行う。
ここでいう「1文字」はUART API上の1バイト。日本語の文字コードの解析は行わない。

```c
unsigned char c;
int ret = uart_poll_in(uart_dev, &c);
if (ret == 0) {
    uart_poll_out(uart_dev, c);
} else if (ret == -1) {
    k_msleep(1);
}
```

`&c`は受信した値の保存先。`uart_poll_in()`は受信待ちで停止するAPIではない。
成功時は0、入力が空なら-1を返す。cを使うのは成功時だけ。
APIの宣言には未実装の`-ENOSYS`、非同期受信中の`-EBUSY`も記載される。
採用MCUX LPUART実装は受信フラグを1回確認し、データを読めば0、それ以外は-1を返す。
このアプリは-1以外の失敗ではエラー表示してmainを終了する。

データがなければアプリが`k_msleep(1)`を呼び、mainを短時間待ち状態にする。
これはAPI内部の待機ではない。受信できたときは次の入力を続けて読む。
厳密な1 ms間隔を保証せず、高速な連続入力の無損失も保証しない。
CR（0x0d）とLF（0x0a）もそのまま返し、改行の追加や変換はしない。

## 行単位のポーリング版（基本動作・上限・UARTエラー復帰を本人確認済み）

`src/poll_line.c`を追加した。UART受信APIと待機方法は文字単位版と同じで、
受信した1バイトを返す代わりに`process_byte()`で行バッファへ蓄積する。
受信状態の読み書きはmainだけが担当する。まだISRとLEDスレッドは使わない。
貼り付け時の無反応の報告を受け、UART受信エラーの検査・解除を追加し、本人の実機で復帰を確認済み。

| 項目 | 定義した動作 |
|---|---|
| バッファ | 32 B、本文は最大31 B、残り1 BはC文字列の終端NUL |
| 受け付ける本文 | 表示可能なASCII（0x20〜0x7e）。日本語の解析やBackspaceによる編集は対象外 |
| 通常の行 | 改行まで表示を返さず、行終端で`Echo: 本文`とCRLFを送信 |
| 行の終端 | CR単独・LF単独は各1回。CR直後のLFを読み捨て、CRLFも1回 |
| 空行 | `Echo: `とCRLFを1回送信 |
| 32 B目の本文 | バッファへ書かず、その行を改行まで破棄。`ERROR: line too long (max 31 bytes); discarded`とCRLFを1回送信 |
| NUL・Backspaceなどの非対応バイト | その行を改行まで破棄。`ERROR: printable ASCII only; line discarded`とCRLFを1回送信 |
| UART受信エラー | uart_err_checkで検査し、採用ドライバでフラグを解除。欠落の可能性がある行を改行まで捨て、overrunまたはRX errorを通知 |
| 異常後 | 行終端で長さ・異常状態をリセットし、次の行を受け付ける。本文の複数異常は最初の理由を通知し、UARTエラーを検出した場合はUARTエラーを優先 |

`line_len < sizeof(line_buf) - 1`の範囲だけへ本文を書き、改行時は`line_buf[line_len] = '\0'`で終端する。
32 B目で先頭31 Bだけを返す仕様にはせず、超過した行全体を捨てる。
CRLFを処理する状態`previous_was_cr`は、直前に受け取ったバイトがCRかを覚える。
たとえば`abc\r\n`ならCRで1行を返し、その直後のLFは返答を増やさない。

ポーリング中の1 ms待機や行の送信中には、ハードウェア受信FIFOがあふれて入力を失う可能性がある。
行バッファの上限保護は、UART通信全体の無損失を保証するものではない。
最初は手入力で確認し、高速な貼り付け・連続入力の結果は別に記録する。

### UART受信エラーからの復帰（本人の実機確認済み）

`uart_err_check()`は0なら検出エラーなし、正の値ならUARTエラーのビット集合、
未実装の場合は負の値を返す。採用MCUX LPUARTドライバではoverrun・parity・framingなどを確認し、
該当ハードウェアフラグをクリアする。フラグ解除は採用ドライバの実装に照合した事実として扱う。

mainは入力の有無にかかわらず各ループで検査し、受信成功直後にも検査する。
エラーを検出したら、その行の本文を返さず、改行まで破棄する状態へ移る。
改行が届いた時点でoverrunなら`ERROR: UART RX overrun; line discarded`、
それ以外なら`ERROR: UART RX error; line discarded`を1回返し、次行を受け付ける。
検査API自体が負の値を返した場合はエラー表示してmainを終了する。

欠落した入力や改行は復元しない。貼り付け後、Enterを押しても返答がなければ追加でEnterを押し、
行終端を届けてから次の正常行を入力する。高速入力中の無損失は保証しない。
起動表示に`RX error recovery enabled.`を追加し、修正前の実機と区別する。

本人の次の操作は、同じPowerShellで`west build -d $uartEchoBuild`と終了コードの確認、
COM・プローブ再確認、同じビルド先からの書き込み。その後RESETして新しい起動表示を確認する。
手入力の31 B・32 B・超過後のok、31 B貼り付け後のEnter・エラー表示・RESETなしでのokを確認する。

### 各版の選択

CMakeの`ECHO_MODE`でビルドするソースを一つだけ選ぶ。既定値は`byte`。

| 指定 | ソース | 用途 |
|---|---|---|
| `-DECHO_MODE=byte` | `src/main.c` | 本人の動作確認済み文字単位版を再現 |
| `-DECHO_MODE=line` | `src/poll_line.c` | 次の行単位実習 |
| `-DECHO_MODE=irq` | `src/irq_line.c` | 割り込み受信＋main送信＋LEDスレッド |

byte・lineは`prj.conf`、irqは`prj_irq.conf`をCMakeがZephyr設定の読み込み前に選ぶ。
選択したソース一つだけをコンパイルし、複数の受信処理を同時に動かさない。`src/main.c`の内容は維持している。
ビルド時に`Stage 6 ECHO_MODE=byte`・`line`・`irq`のいずれかが表示される。

```powershell
# 行単位版。環境設定済みの同じPowerShellでZEPHYR_BASEから実行する。
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\06_uart_echo" -d $uartEchoBuild -- -DECHO_MODE=line
$LASTEXITCODE
```

本人のビルド成功を確認後、操作3の接続先再確認・書き込みを同じビルド先で行う。
文字単位版へ戻す場合は同じコマンドの末尾を`-DECHO_MODE=byte`へ変更して作り直す。
初回の文字単位実習は、この選択肢の追加前に実施した。

行単位版の起動表示は`Stage 6: polling line echo`。
まずローカルエコーOFFで`abc123`を入力し、Enter前は表示されず、Enter後に`Echo: abc123`が
1回表示されることを確認する。次に空行、本文31 B、32 B、超過行の後の短い正常行、CR・LF・CRLFを確認する。
具体的な端末操作はビルド・書き込みの結果に合わせて案内し、まだ実施済みとは扱わない。

### 行単位版の実機確認手順（案内済み・結果待ち）

操作3のコマンドで今回のCOMとプローブを列挙し、HN2APUBRDCEPTを確認後に本人が書き込む。
終了コードを記録し、Tera Termを今回のCOM・115200・8N1・フロー制御なし、ローカルエコーOFFへ設定。
送信改行CR+LF、受信改行CRから始め、受信準備後にRESETを押して離す。
起動表示が行単位版であることを確認し、次の順に手入力で確認する。

| 送信改行 | 入力（各行の末尾でEnter） | 期待する結果 |
|---|---|---|
| CR+LF | `abc123` | Enter前は返答なし、Enter後に`Echo: abc123`が1回 |
| CR+LF | 本文なしでEnter | `Echo: `が1回 |
| CR+LF | `1234567890123456789012345678901`（31 B） | 本文を省略せずEchoで返す |
| CR+LF | `12345678901234567890123456789012`（32 B） | line too longのエラーが1回。先頭31 BのEchoは返さない |
| CR+LF | `ok` | 超過行の後でも`Echo: ok`が返る |
| CR | `cr` | `Echo: cr`が1回 |
| LF | `lf` | `Echo: lf`が1回 |

最後に送信改行をCR+LFへ戻す。受信改行はCRのままとし、実際の設定値も報告してもらう。
高速な貼り付け・連続入力はこの手入力の確認と分け、割り込み版との比較で記録する。

### ポーリング版のUARTの受信担当

`prj.conf`でSERIALを有効にし、CONSOLE、UART_CONSOLE、CONSOLE_SUBSYS、SHELL、
PRINTK、LOG、UART_INTERRUPT_DRIVEN、UART_ASYNC_APIを無効にする。
受信はmainの`uart_poll_in()`だけ。起動表示も直接送信し、printkは使わない。
chosenはUARTを選ぶDevicetreeの参照であり、CONFIG_CONSOLE=nでもノード参照を使える。
コンソールが存在するだけで必ず受信競合するという意味ではない。
今回は専用のエコー実習として経路を絞る。

本人の段階6の生成.configでSERIAL・UART_MCUX_LPUART=y、UART_INTERRUPT_DRIVEN・UART_ASYNC_API・
CONSOLE・CONSOLE_SUBSYS・SHELL・PRINTK・LOGの無効化を確認した。
UART_CONSOLE自体の行は表示されていないが、親のCONSOLEが無効でUARTコンソールは組み込まれない。
Codexが同じ生成.configを読み取り照合し、CMakeCacheのアプリ入力・prj.conf・CPU0ボード・Zephyrの場所も確認した。
生成zephyr.dtsでchosenがflexcomm4_lpuart4、compatibleがnxp,lpuart、115200、status=okayと確認した。
段階5の生成.configではUART_INTERRUPT_DRIVEN=yだが、
CONSOLE_HANDLER・CONSOLE_SUBSYS・SHELL・UART_ASYNC_APIは無効だった。
段階5の生成値を段階6の生成値として扱わない。

## 本人の操作手順

### 1. 新しいPowerShellの環境確認（本人の結果確認済み）

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
Set-Location $env:ZEPHYR_BASE
$uartEchoBuild = Join-Path $zephyrWorkspace 'build\06_uart_echo'
$env:ZEPHYR_BASE
west --version
git rev-parse HEAD
```

期待値は上表のZEPHYR_BASE、west 1.5.0、採用Zephyr SHA。
環境設定はPowerShellセッションごとに読み直す。
2026-10-24に本人から3項目の結果を受領し、すべて期待値と一致することを確認した。
受領日を記録しており、実行した正確な日時は取得していない。

### 2. 文字単位版のビルド・生成設定確認（本人のビルド成功・設定確認済み）

実行場所：ZEPHYR_BASE。本人用の段階6専用ビルド先を使う。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\06_uart_echo" -d $uartEchoBuild
$LASTEXITCODE
```

```powershell
Select-String -Path "$uartEchoBuild\zephyr\.config" `
    -Pattern 'CONFIG_(SERIAL|UART_MCUX_LPUART|CONSOLE|UART_CONSOLE|CONSOLE_SUBSYS|SHELL|UART_INTERRUPT_DRIVEN|UART_ASYNC_API|PRINTK|LOG)(=| )'
```

本人のビルド抜粋では154/154のリンク完了、Zephyr 4.4.2、CPU0ターゲット、終了コード0を確認。
上記の生成設定表示も本人から受領済み（2026-10-24）。
CMakeCache・生成Devicetreeの照合はCodexの読み取り確認であり、本人の表示確認とは区別する。

### 3. 接続先再確認と書き込み（ビルド確認後に案内）

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
LinkServer probes --no-boot
```

COMとプローブを確認してから、本人が実行する。

```powershell
west -v flash -d $uartEchoBuild -r linkserver --probe HN2APUBRDCEPT
$LASTEXITCODE
```

### 4. Tera Termと文字単位エコー（書き込み後に案内）

今回のCOM、115200、8N1、フロー制御なしを確認する。
ローカルエコーはOFF、最初の実習では送信改行CR+LF、受信改行CRを予定する。
送信改行はEnterでPCから送る値、受信改行はボードから届く値の表示方法。両方を記録する。
受信準備後にRESETを押して離し、`Stage 6: polling byte echo`の起動表示を確認する。
ASCIIの`abc123`をゆっくり入力すると、Enter前から1文字ずつ1回だけ表示されることを期待する。
EnterではCRとLFをそのまま返す。ローカルエコーONだとPC側表示と受信表示が重なり得る。
Backspaceによる編集は実装していない。行単位版での上限・改行・連続入力テストは後で定義する。

## 割り込み版（基本実機・連続入力を本人確認済み）

採用コミットの公式echo_botをローカルで読み、次の構造を確認した。

- UART割り込みコールバックがFIFOを読み、改行まで蓄積する。
- 完成した固定サイズのメッセージを`k_msgq_put(..., K_NO_WAIT)`で渡す。
- mainが`k_msgq_get(..., K_FOREVER)`で待ち、受け取った行を送信する。

MCUX LPUARTドライバはUART ISRからアプリのコールバックを呼ぶ。
MCXN947のLP Flexcomm経由でもISRの実行文脈であり、独立した受信スレッドではない。
ISRではFIFO読出し、上限確認、短い改行判定、待たないキュー投入を行う。
sleep・待ち付きキュー投入・長い表示処理はスレッド側へ置く。
割り込み版へ切り替えたらmainでのuart_poll_inは止め、同じUARTを複数の処理で読まない。

公式例は32 Bの受信バッファ、10件のキューを持ち、文字数超過やキュー満杯でデータを捨てる。
本実習では空行・CRLF・行長超過・UARTエラーの扱いをポーリング版と揃えた。
公式例と同じ構成を参考に独立して実装した`src/irq_line.c`と`prj_irq.conf`を追加した。

### 役割と新しいAPI

| 実行場所 | 担当 | 待機・送信 |
|---|---|---|
| UART ISRのコールバック | UARTエラー検査・解除、FIFO読出し、上限・改行判定、完成した行のキュー投入 | sleepなし。送信なし。キュー投入はK_NO_WAIT |
| main（優先度6、スタック要求2048 B） | キューから受け取った行・エラー通知をUARTへ出力 | k_msgq_getのK_FOREVERで待機。送信はuart_poll_out |
| LEDスレッド（優先度5、スタック要求2048 B） | 赤色LEDを250 msごとにON/OFF | k_msleepで待機。UART出力なし |

`uart_irq_callback_user_data_set()`でコールバックを登録し、`uart_irq_rx_enable()`で受信割り込みを有効化する。
`uart_irq_err_enable()`も有効にし、データがないエラー割り込みでも検査・解除する。
採用ドライバのrx_enableは受信データとoverrunの割り込み、err_enableはnoise・framing・parityを有効にする。

コールバックは`uart_irq_update()`で割り込み状態を更新し、エラーを検査した後、
`uart_irq_rx_ready()`でデータの有無を確認して`uart_fifo_read()`でFIFOが空になるまで読み出す。
採用MCUX実装のirq_updateは1を返すが、アプリはAPIで要求される呼出し順を守る。
mainではuart_poll_inを呼ばず、割り込み有効化後の受信状態とUARTエラーの読み取りはISRだけが担当する。

`K_MSGQ_DEFINE`で固定サイズのメッセージ10件を保持するキューを定義する。
各メッセージは本文32 Bと処理結果・UARTエラーフラグを持つ。
`k_msgq_put(..., K_NO_WAIT)`はメッセージ内容をコピーするため、ISRは投入後に受信バッファを再利用できる。
mainの`k_msgq_get(..., K_FOREVER)`は行がないとき待ち状態となり、CPUを使って入力有無を調べ続けない。
LEDは別スレッドなので、入力なしでも、mainがUART送信中でも実行できる。

### バッファ・キュー超過と異常時の扱い

- 本文は31 Bまで。32 B目以降はその行を改行まで捨て、行長超過の結果をキューへ投入する。
- CRLFは1行、CR・LF単独も各1行。空行もEchoで返す。非対応バイトとUARTエラーの行は破棄する。
- キュー満杯なら新しく完成した行を捨て、すでにキュー内にある行は残す。
- ISRは満杯時も待たず、atomicフラグでmainへ通知する。mainが次にメッセージを受け取った時に
  `ERROR: RX queue full; one or more lines discarded`を出力する。通知は複数件をまとめるため、破棄行数は数えない。
- atomicはISRとmainが同時に扱う通知フラグの読み書きに使う。受信バッファ全体を共有して保護する仕組みではない。
- mainの初期GPIO設定後、LED出力の変更はLEDスレッドだけが行う。LED書込み失敗なら同スレッドを終了し、
  mainが次に行を受け取った時に通知する。LEDスレッドからUART出力しない。

割り込み化してもハードウェアFIFOとキューは有限であり、無制限の高速入力・無損失は保証しない。
受信待ち中のLED継続、31/32 B、貼り付け、CR/LF、連続入力、キュー満杯後の復帰を本人の実機で確認する。

### 本人のビルド手順（リンク完了・終了コード0を確認済み）

同じPowerShellのZEPHYR_BASEから実行する。CMakeの設定とソースが変わるため、この切替では作り直す。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\06_uart_echo" -d $uartEchoBuild -- -DECHO_MODE=irq
$LASTEXITCODE
```

ECHO_MODE=irq、prj_irq.confの採用、リンク完了・終了コード0・使用量を確認する。
2026-10-24に本人の154/154リンク完了・Zephyr 4.4.2・CPU0・終了コード0の結果を受領した。
抜粋にはECHO_MODEと設定ファイル名がなかったため、Codexが本人用CMakeCache・build.ninjaを読み取り、
ECHO_MODE=irq・prj_irq.conf・src/irq_line.cの選択を照合した。
同じ生成.configでGPIO・GPIO_MCUX・UART_INTERRUPT_DRIVEN・UART_MCUX_LPUART・ISR_SUPPORTがy、
CONSOLE・CONSOLE_SUBSYS・SHELL・UART_ASYNC_APIが無効、main優先度6・スタック2048 Bを確認した。
毎回のCOM・プローブ確認と書き込みへ進む。
起動表示は`Stage 6: interrupt line echo`。

### 割り込み版の書き込み・実機確認（基本確認済み）

操作3のCOM・プローブ列挙で今回の接続先を確認し、本人が同じビルド先から書き込む。
終了コード0を確認後、Tera Termで今回のCOM・115200・8N1・フロー制御なし・ローカルエコーOFF、
送信改行CR+LF・受信改行CRに設定し、受信準備後にRESETを押して離す。

| 確認 | 期待する動作 |
|---|---|
| 起動後に約10秒入力しない | 起動表示3行。赤色LEDは点滅を継続 |
| abc123を入力し、Enter前に少し待つ | UART返答はまだなく、LEDは点滅を継続。Enter後にEcho: abc123が1回 |
| 空行 | Echo: が1回 |
| 31文字を貼り付けてEnter | 全文のEchoを期待。実際の返答とLEDを記録し、エラーならそのまま報告 |
| 32文字をゆっくり手入力してEnter、その後ok | line too longの通知が1回、続くEcho: okが正常。LEDは継続 |
| 送信改行CRでcr、LFでlfを各Enter | 各Echoが1回。最後に送信改行をCR+LFへ戻す |

本人が「操作と期待結果を確認できた」と報告し、上表の動作を確認済みとした（2026-10-24に受領）。
LEDは本人の目視報告、UARTは提供表示に基づく。厳密なLED周期や割り込み遅延の測定とは区別する。
UARTの実機表示、LEDの目視報告、書き込み終了コード、実際の端末設定を別々に記録する。

### 連続入力（3行・100行の応答を確認済み、キュー満杯の分岐は未確認）

再ビルド・RESETは不要。本人がPowerShellでクリップボードへ文字列を設定し、Tera Termへ貼り付ける。
末尾にCRLFを含めるため、貼り付け後のEnterは不要。ローカルエコーOFF、送信改行はCR+LFに戻しておく。

```powershell
Set-Clipboard -Value "one`r`ntwo`r`nthree`r`n"
```

Tera Termへ貼り付け、Echo: one、Echo: two、Echo: threeが順に各1回返ることを確認する。

```powershell
$uartBurstText = ((1..100 | ForEach-Object { '{0:D3}' -f $_ }) -join "`r`n") + "`r`n"
Set-Clipboard -Value $uartBurstText
```

001〜100の100行をまとめて貼り付け、出力が落ち着いたらRESETせずokを手入力してEnter。
キュー満杯時は新しい完成行を捨て、RX queue fullの通知を出す仕様。欠番はこの破棄と整合する。
投入速度が十分遅ければ満杯にならないため、通知の有無を実際の結果として記録し、必ず満杯になるとは扱わない。
UART RX overrunなど別のエラーがあれば、その表示も記録する。
確認するのはエラー通知、停止しないこと、入力前後のLED点滅継続、最後のEcho: ok。
全100行のチャット貼り付けは不要で、最初と最後の数行・エラー表示・ok応答・LEDの様子を報告してもらう。

本人からEcho: one・two・three、Echo: 001〜100の全行、最後のEcho: okを受領した（2026-10-24）。
提供表示にはキュー満杯・UART受信エラーの通知も欠番もなかった。
連続入力の成功として記録し、キュー満杯時の破棄・通知経路を確認したとは扱わない。
100行試験前後のLED点滅継続も、本人の最終確認で報告を受領した。入力なし・通常入力時のLED継続も確認済み。
PC側の送信間隔・キュー最大占有数は未測定。

本人からISRとキューの動作について図解の依頼を受け、[QA-038の図と説明](../../docs/qa.md#qa-038-supplement)に記録した。
実際の関数名・変数名の図と具体例は[QA-039](../../docs/qa.md#qa-039-supplement)に記録した。

## 確認結果・問題と対処・未確認事項

| 項目 | 実施者／結果 |
|---|---|
| ソース・資料確認 | Codex：採用コミットのAPI、MCUX LPUART、ボード定義、公式echo_bot、段階5の生成.configを照合 |
| 段階6の事前ビルド | Codex：割り込み版を本人用とは別の.tmp/build/06_uart_echo_irq_codexでビルドし、終了コード0で成功。CMakeCache・.config・Devicetree・mapを確認。書き込み・実機確認は行っていない。[事前確認記録](evidence/README.md) |
| 環境再確認 | 本人：ZEPHYR_BASE・west 1.5.0・採用Zephyr SHAの一致を確認済み。2026-10-24に結果受領 |
| ビルド・生成設定確認 | 本人：文字単位版のリンク完了・終了コード0、SERIAL・UART_MCUX_LPUART=yとCONSOLE・CONSOLE_SUBSYS・SHELL・割り込み／非同期受信・PRINTK・LOGの無効化を確認。2026-10-24に結果受領 |
| 本人用ビルドの生成ファイル照合 | Codex：CMakeCacheの段階6アプリ・prj.conf・CPU0ボード・採用Zephyr、.config、生成DevicetreeのUART・115200・okayを読み取り確認。ビルド実行とは区別 |
| 文字単位版の起動・UART表示 | 本人：段階6の起動表示2行とabc123を提供。ローカルエコーOFFでEnter前から1文字ずつ1回だけ表示、Enter後に次行の先頭へ移ることを確認済み。2026-10-24に結果受領 |
| 文字単位版の書き込み | 本人：終了コード0を確認済み。2026-10-24に結果受領 |
| 接続先・端末設定 | 本人：最終列挙でCOM5／MCU-Link VCom PortとプローブHN2APUBRDCEPTを確認済み。ローカルエコーOFF、CR/LF各設定の動作も確認済み。通信条件・送受信改行設定の画面そのものは未受領 |
| 行単位ポーリング版のビルド | 本人：154/154のリンク完了・終了コード0・Zephyr 4.4.2・CPU0ターゲットを確認。2026-10-24に結果受領。CodexがCMakeCacheのECHO_MODE=line、build.ninjaのpoll_line.cを読み取り確認。Codexのビルド実行は未実施 |
| 行単位版の書き込み・実機確認 | 本人：abc123・空行、修正後の31 B正常Echo、32 B超過エラーと次行ok、貼り付け時のUART overrun通知と次行okを確認。書き込み終了コード、CR単独／LF単独は未確認 |
| UARTエラー復帰の修正 | 本人：31 B正常Echo、32 B超過エラーと直後のok、貼り付け時のUART RX overrun通知とRESETなしでのokを確認（2026-10-24に受領）。修正版のビルド・書き込み終了コードは未受領 |
| 割り込み版の本人ビルド | 本人：154/154リンク完了・終了コード0、Flash 25,136 B・RAM 8,120 B・SRAMX 0 Bを確認（2026-10-24に結果受領）。Codexが本人用生成設定のirq選択・ドライバ・GPIO・受信競合機能の無効化を読み取り照合 |
| 割り込み版の書き込み | 本人：終了コード0と報告。最終列挙でCOM5とプローブHN2APUBRDCEPTも確認済み（2026-10-24に受領） |
| 割り込み版・LED継続 | 本人：提示した操作と期待結果を確認したと報告。入力なし約10秒とEnter前の待機中のLED点滅、空行、31 B貼り付け全文Echo、32 B超過通知と次行ok、CR・LF各1回の応答を確認済み（2026-10-24に受領）。厳密な周期は未測定 |
| 割り込み版の連続入力 | 本人：3行、001〜100の全行、最後のok応答を確認（2026-10-24に受領）。提供UARTに欠番・キュー満杯・UARTエラー通知なし。最終確認で100行試験前後のLED点滅継続も報告 |
| キュー満杯時の破棄・通知 | ソースで仕様を定義し、事前ビルド済み。今回の100行入力では通知を観測せず、満杯分岐の実機確認は未実施 |
| ISRとキューの補足説明 | 本人の図解依頼を受け、QA-038/039に通常の受信経路、実際の関数・変数名、ISRが空き待ちできない理由を説明。本人からの再説明を求める確認は行っていない |
| README確認 | 本人：確認し、内容に問題ないと報告（2026-10-24に受領） |

### 文字単位版の実機UART表示

本人から提供された表示（2026-10-24に受領）：

```text
Stage 6: polling byte echo
Local echo OFF; each received byte is returned unchanged.
abc123
```

起動表示中のLocal echo OFFはアプリの案内文であり、端末設定を自動検出した結果ではない。
本人の追加報告で、ローカルエコーOFF、Enter前の1文字ずつ1回の表示、Enter後の次行先頭への移動、
書き込み終了コード0も確認した（2026-10-24に受領）。端末設定は本人の報告を根拠とする。

### 割り込み版の基本実機UART表示

本人の「操作と期待結果を確認できた」との報告と併せて受領した表示（2026-10-24）：

```text
Stage 6: interrupt line echo
Local echo OFF; max 31 printable ASCII bytes; queue depth=10.
LED toggle interval=250 ms; main waits for completed lines.
Echo: abc123
Echo:
Echo: 1234567890123456789012345678901
ERROR: line too long (max 31 bytes); discarded
Echo: ok
Echo: cr
Echo: lf
```

この31 B貼り付けではポーリング版で発生したUART RX overrunは表示されず、全文を受信した。
あらゆる入力速度での無損失の保証とは区別する。

### 行単位版の31文字貼り付けで無反応（対策後のエラー通知・復帰を本人確認済み）

2026-10-24に本人から、abc123と本文なしEnterは期待どおりだが、
`1234567890123456789012345678901`（31文字）は無反応と報告を受けた。
本人の追加報告で、元の入力は貼り付け、追加Enterでも無反応、RESET後に同じ31文字を
ゆっくり手入力してEnterを押すと期待どおりだったことを確認した（2026-10-24に受領）。

ソース上は31 Bの本文を添字0〜30へ格納し、行終端で添字31へNULを置くため、31 Bは受け付ける設計。
受信した本文自体には逐次応答せず、CRまたはLFを受け取るまで返さない。
採用ドライバのpoll_inは受信フラグの確認と読出しのみで、エラー状態を検査・クリアしない。
高速入力時にはアプリの1 ms待機中などに受信データを失う可能性がある。

RESET後の手入力で31 Bの処理は確認できた。速い貼り付けによるUART受信エラーを有力な仮説とし、
採用ドライバのerr_checkがフラグを検出・解除することを照合して、アプリへ検査と復帰処理を追加した。
修正前に実際のフラグ値は取得していないため、overrun発生の確定とは扱わない。
修正後の本人の再現でUART RX overrunの表示を確認し、オーバーラン発生を確認できた。
31 Bの手入力は全文Echo、32 Bでは行長超過エラー、その後のokは正常応答だった。
31 Bの貼り付けではUARTエラーの行を捨てて通知し、RESETなしで次のokへ復帰した。
受信の無損失を達成した結果とは区別する。

本人が提供した修正後のUART表示：

```text
Stage 6: polling line echo
Local echo OFF; max 31 printable ASCII bytes; CR/LF ends a line.
RX error recovery enabled.
Echo: 1234567890123456789012345678901
ERROR: line too long (max 31 bytes); discarded
Echo: ok
ERROR: UART RX overrun; line discarded
Echo: ok
```

LinkServerの補助処理でFailed to boot probesやUnicodeDecodeErrorが出た段階5の履歴は、
段階6の発生結果として扱わない。発生した場合は後続のFlash表示・終了コード・実機動作で判断する。
プローブFW更新は行っていない。

### 文字単位版のビルド時使用量

本人のビルド表示（2026-10-24に受領）に基づく。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| 内蔵Flash | 22,488 B | 2,097,152 B | 1.07% |
| 内蔵SRAM | 5,440 B | 524,288 B | 1.04% |

表示されたRAM 5,440 BとSRAMX 0 Bを重複なく合計した。IDT_LISTも0 B。
CPU0の320 KiB RAM領域に対しては1.66%であり、SRAM全体に対する1.04%とは区別する。
計算は`使用量÷全体容量×100`。実行時最大スタック・ヒープ、CPU使用率、厳密な周期は未測定。

### 行単位版のビルド時使用量（UARTエラー復帰の追加前）

本人のビルド表示（2026-10-24に受領）に基づく。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| 内蔵Flash | 22,748 B | 2,097,152 B | 1.08% |
| 内蔵SRAM | 5,480 B | 524,288 B | 1.05% |

表示されたRAM 5,480 BとSRAMX 0 Bを重複なく合計した。IDT_LISTも0 B。
CPU0の320 KiB RAM領域に対しては1.67%。実行時最大使用量ではない。
UARTエラー復帰を追加した修正版の使用量は次節に別記する。

### 行単位ポーリング版のビルド時使用量（UARTエラー復帰の追加後）

本人が作成したビルドのzephyr.mapをCodexが読み取り確認した値。
本人のビルド末尾の使用量表示は未受領であり、Codexによるビルド実行結果とは区別する。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| 内蔵Flash | 23,052 B | 2,097,152 B | 1.10% |
| 内蔵SRAM | 5,480 B | 524,288 B | 1.05% |

_flash_used=0x5a0c、_image_ram_size=0x1568。他のRAM領域へ重複計上しない。
CPU0の320 KiB RAM領域に対しては1.67%。

### 割り込み版のビルド時使用量（本人／Codex事前確認は別に実施）

Codexは別保存先で事前ビルド済み。2026-10-24に本人のビルド表示も受領し、同じ使用量と確認した。
両者は別の実行結果として記録する。

| 対象 | 使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| 内蔵Flash | 25,136 B | 2,097,152 B | 1.20% |
| 内蔵SRAM | 8,120 B | 524,288 B | 1.55% |

RAM 8,120 B、SRAMX 0 B、IDT_LIST 0 B。CPU0の320 KiB RAM領域に対しては2.48%。
実行時の最大スタック・ヒープ使用量とCPU使用率、厳密なLED周期は未測定。

## 採用コミットの参照先

判断はローカルの採用コミットのファイルに照合した。Web取得では固定コミットURLがCache missになったため、
オンラインの本文確認済みとは扱わない。

- [UART API](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/include/zephyr/drivers/uart.h)
- [MCUX LPUARTドライバ](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/drivers/serial/uart_mcux_lpuart.c)
- [公式echo_bot](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/samples/drivers/uart/echo_bot/src/main.c)
- [CPU0ボード定義](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/boards/nxp/frdm_mcxn947/frdm_mcxn947_mcxn947_cpu0.dtsi)
