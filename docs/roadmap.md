# 学習ロードマップ

## 学習の軸（2026-11-07見直し）

このリポジトリの目的は、FRDM-MCXN947でZephyrを使えるようになること。
段階9に限らず今後のすべての段階で、次の3点を主題とする。

1. **設定からデバイス利用まで**：Devicetree・Kconfig・ビルド結果が、使用するデバイスやドライバにどうつながるか。
2. **ドライバAPIの利用**：準備状態、引数、戻り値を確認し、GPIO・UART・Display・Videoなどを用途に合わせて使う。
3. **処理の役割分担**：スレッド・割り込み・キュー・待機で、処理とデータの受け渡しを組み立てる。

各操作の前に「使うZephyr機能・目的・本人が変える点・見える動作」を短く説明する。
最小の動作を試してから必要な範囲を広げる。Cの全行解説や、数値解析・一般的な画像処理・MCU固有の詳細調査を主題にしない。
GDBやレジスタ値の手動照合は、具体的な問題の切り分けや本人の希望がある場合の補助とし、毎段階の必須条件にしない。
理解を保留して先へ進める本人の依頼を尊重し、未理解はそのまま記録する。
この方針は[AGENTS.md](../AGENTS.md)にも保存し、後続のチャットで引き継ぐ。

RAM・レジスタの実施済み記録は補助教材として保持する。LCD・カメラはZephyr APIと処理分担の学習題材を継続する。
GUIはDisplay APIの学習後に、Zephyrに統合されたLVGLを使う任意の発展課題として扱う。
Slintは今回の計画から外す。過去の選定・実習記録は当時の内容を保持する。

## 2026-11-08の見直し（本人承認済み）

本人が提案のロードマップで進めることを承認したため、正式な学習計画として採用した。
段階10の [#28](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/28) と段階11の [#29](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/29) を新規作成し、既存の #11〜#16 は段階番号・対象フォルダ・前提段階・GUI課題を更新した。
計画反映時点では段階10〜17の学習は未着手だった。同日に段階10へ着手し、予定した実機操作は本人確認済み。設定・処理担当を記録し、READMEの本人確認用6項目の一致確認も受領し、段階10の学習・記録確認は完了（PRマージ・イシュークローズは未承認）。段階11〜17は未着手。計画の承認、実機確認、記録確認とPRマージ・イシュークローズの承認を区別する。

- 段階10にConsoleと標準Shell、段階11にC/C++の単体テストを追加する。
- 旧段階10〜15は2つ後ろへ移し、段階12〜17とする。イシュー番号は維持する。
- 旧段階14・15（新段階16・17）はSlintからLVGLへ置き換える。
- 既存の段階0〜9の成果・省略・理解保留は保持する。未作成の段階フォルダは着手時に追加する。

## これまでの経過

段階0〜17をそれぞれ独立したイシューで管理します（2026-11-08に2段階追加・以降を改番）。2026-10-11時点で段階0のPC環境調査・選定・記録、段階1の本人によるビルド・書き込み・UART・GDB実習とPR確認、段階2の本人によるGPIOでのLED点灯・消灯、説明・記録・PR確認が完了しました。本人の承認後に[PR #17](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/17)と[PR #18](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/18)をマージし、#2・#3はcompletedとしてクローズ済みです。[段階1の実習結果](../stages/01_hello_world/README.md)、[段階2の実習結果](../stages/02_led_control/README.md)、[選定記録](../stages/00_versions/versions.md)を参照してください。段階3は本人の両版のビルド・書き込み・UART時刻差比較・LED点滅と速度変更・待機と周期の説明まで確認済みです。UARTとLEDの各状態の対応は500 ms版で確認し、250 ms版は速くて目視では断定できなかったと記録しています。最終版は250 ms。本人によるREADMEと[PR #20](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/20)の確認も完了し、マージとイシュー #4のクローズを承認済みです。[段階3の記録](../stages/03_led_blink/README.md)を参照してください。段階4は本人の環境確認、作業ブランチ作成、ソース・設定の表示確認とCPU0向けビルドまで完了しています。本人の接続先再確認と書き込みも成功表示・終了コード0で確認済みです。本人が3巡の低速→高速→消灯→高速再開→消灯と、途中で停止しないことを確認しました。UARTはseq 25まで進み、ERRORなし・set_retすべて0との報告です。提供UARTの時刻差も比較済みで、本人による役割分担・状態更新担当・待機・優先度・スタックの説明も確認済みです。READMEも本人が実際の学習・観察結果と一致すると確認し、PR作成を依頼しました。[PR #21](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/21)を作成し、本人によるPR内容の確認も完了しました。本人の別途の明示的な承認後、PR #21をマージし、イシュー #5をcompletedとしてクローズしました。段階4の学習は完了です。[段階4の記録](../stages/04_threads_queues/README.md)を参照してください。段階5は2026-10-12に本人の環境・Zephyrコミット・UART定義・作業ブランチ確認と、UART送信・LED・観察用待機・繰り返し、設定とCMakeの表示確認まで進みました。2026-10-13に本人のビルド抜粋と終了コード0を受領し、成功を確認しました。本人のCMakeCache・Kconfig・UART・LED・GPIOの生成定義確認も完了し、COM5とプローブHN2APUBRDCEPTも再確認済みです。本人の書き込みもFlash成功表示と終了コード0で確認済みです。本人のTera Term設定も確認済みです。RESET後の起動表示とcycle 1〜3のHello World受信を確認し、本人はLEDの点滅も報告しました。本人が送信ループ中と待機中のLED点滅継続、文字がゆっくり増える様子も確認しました。本人の出力経路・文字列終端・改行・待機・共有UARTの説明と、ログ比較の補足説明後の理解報告も受領しました。本人のREADME確認も2026-10-14に完了しました。本人の依頼によりCodexがコミット・プッシュし、[PR #22](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/22)を作成しました。本人のPR確認と別途の明示的な承認後、2026-10-14にPR #22をマージし、イシュー #6をcompletedとしてクローズしました。段階5の学習は完了です。[段階5の記録](../stages/05_uart_tx/README.md)を参照してください。段階6は2026-10-14に着手し、文字単位のポーリングエコーの初期ソースと記録をCodexが作成しました。2026-10-24に本人のZEPHYR_BASE・west 1.5.0・採用Zephyr SHAの一致を確認しました。本人の文字単位版ビルドはリンク完了・終了コード0で成功し、生成.configのUARTドライバ有効・受信競合機能の無効化も確認済みです。本人の文字単位版の書き込み終了コード0、ローカルエコーOFFでEnter前から1文字ずつ1回だけ表示、Enter後の次行先頭への移動を確認済みです。続いて行単位ポーリング版を追加しました。行単位版も本人のリンク完了・終了コード0を確認し、Codexが生成ファイルのECHO_MODE=line・poll_line.cを照合済みです。行単位版は本人がabc123・空行・RESET後の31 B手入力の期待動作を確認しました。31 Bの貼り付け後に追加Enterにも応答しなかったため、CodexがUARTエラーの検査・解除と次行への復帰を追加しました。本人が修正後の31 B正常受信、32 B超過後のok、貼り付け時のoverrun通知とRESETなしでのokへの復帰を確認しました。続いて割り込み受信・キュー・LEDスレッドの版を追加し、Codexの別保存先での事前ビルドは終了コード0で成功しました。割り込み版の本人ビルドも終了コード0で成功し、Flash 25,136 B・RAM 8,120 B、生成設定のirq選択を確認済みです。割り込み版も本人が入力待ち中のLED継続、空行、31 B貼り付け、32 B超過後の復帰、CR/LFを確認済みです。本人の3行・100行連続入力と最後のokも確認済みです。今回の100行ではキュー満杯通知は観測されず、満杯分岐の実機確認は未実施です。ISRとキューの動作を図解し、本人のREADME確認も2026-10-24に完了しました。割り込み版の書き込み終了コード0、COM5・プローブ一致、100行入力前後のLED継続も確認済みです。本人のPR確認と明示的な承認後、2026-10-24に[PR #23](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/23)をマージしました。その後の別途の明示的な承認に基づき、#7の全チェック項目を完了へ更新し、completedとしてクローズしました。段階6は完了です。ポーリング行単位版の書き込みログ・終了コードと、端末設定画面自体は未受領として区別しています。[段階6の記録](../stages/06_uart_echo/README.md)を参照してください。段階7は2026-10-30に着手し、2026-10-31に完了しました。本人のソース・設定・外部OSS照合、CPU0向けビルド・書き込み、NT-ShellとLEDコマンド、入力待ち中の点滅継続、不正入力・編集・履歴・取消し・3方式の改行、README・PRの確認を終えました。本人の明示的な承認後に[PR #25](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/25)をマージし、イシュー #8の全10チェック項目を完了へ更新してcompletedとしてクローズしました。最終状態はLED消灯、送信CR+LF・受信CR。未確認の追加ケースは[段階7の記録](../stages/07_ntshell/README.md)へ区別して残しています。段階8は2026-10-31に着手し、2026-11-01に完了しました。本人のビルド・書き込み、3幅のRAM操作、境界・不正入力、RESET初期化とGDB照合、README・PR確認を終えました。明示的な承認後に[PR #26](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/26)をマージし、#9の全8チェックを完了・completedとしてクローズしました。Flash 34,200 B・RAM 10,992 B。GDB終了後のUART無応答はRESETで復旧し、原因未確定として記録を保持。最終状態はLED消灯・RAM初期パターン・GDB終了済み。ローカルmainも同期済みです。[段階8の記録](../stages/08_memory_ram/README.md)を参照してください。段階9は2026-11-01に着手し、GPIO0 bit 10に限定するレジスタ操作を実装しました。CodexのCPU0事前ビルド・QEMUの9テスト、本人提供RM（Rev. 7, 2025-10-10）との本文照合も完了しました。本人の環境4項目も一致を確認しました。2026-11-07に本人のファイル照合PASS・終了コード0と、CPU0ビルド166/166・終了コード0を確認しました。Flash 34,436 B・RAM 8,744 B。register_commands.cの理解は本人の依頼で保留し、ビルド成功とは区別しています。今回の接続先も本人の列挙でCOM5／HN2APUBRDCEPTと確認しました。本人のFlash成功表示・0x8684 Bロード・終了コード0も確認しました。本人の起動表示・プロンプト・初期LED消灯・helpの確認報告も受領しました。reg infoと3レジスタの初期readも提供UARTで確認し、LED消灯維持の報告を受領しました。PCOR 2回／PSOR 1回のOK応答・PDOR変化と、LED点灯→点灯維持→消灯の本人報告も確認しました。PTOR 2回の反転と消灯状態での3操作先への値0による状態維持もUARTと本人の目視報告で確認しました。不正入力9件の拒否と、LED消灯維持・正常readとhelpへの応答も確認しました。本人の依頼によりGDB照合は今回の必須項目から除外しました。段階9をZephyrのGPIOドライバと設定の役割を確認する補助教材として整理し、2026-11-08に本人がREADMEの6項目と実習内容の一致を確認し、コミット・プッシュ・PR作成を依頼しました。[PR #27](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/27)を作成し、同日に本人の明示的な依頼でmainへマージしました（a198abd）。続いて本人の依頼でイシュー #10をcompletedとしてクローズしました。段階9は完了です。GDBは未実施で、省略を完了と扱いません。[段階9の仕様と手順](../stages/09_register_access/README.md)を参照してください。段階10はConsoleの1文字入出力、標準Shellのhello・LEDのon／off・点滅間隔変更・停止、ヘルプ・Tab補完・履歴、入力待ち中のLED継続を本人確認済み。最終状態は消灯・点滅停止。設定・処理担当の対応を記録し、READMEの本人確認用6項目の一致確認も受領し、段階10の学習・記録確認は完了（PRマージ・イシュークローズは未承認）。段階11〜17は未着手です。

| 段階 | 課題 | 追加するフォルダ | 前提段階 | イシュー | Zephyr学習の重点 |
|---|---|---|---|---|---|
| 0 | 使用バージョンと開発環境の方針を決定する | `stages/00_versions/` | なし | [#1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1) | west・SDK・ボード指定と再現可能なビルド環境 |
| 1 | 環境構築とHello Worldのビルド・実行・デバッグ | `stages/01_hello_world/` | [段階0](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1) | [#2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/2) | Zephyrアプリのビルド・書込み・実行。既実施のGDB記録は保持 |
| 2 | GPIO APIでLEDを点灯・消灯する | `stages/02_led_control/` | [段階1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/2) | [#3](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3) | DevicetreeのLED指定とGPIO API |
| 3 | k_msleep()でLEDを周期的に点滅させる | `stages/03_led_blink/` | [段階2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3) | [#4](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/4) | k_msleepによる待機と周期動作 |
| 4 | スレッドとメッセージキューでLEDを制御する | `stages/04_threads_queues/` | [段階3](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/4) | [#5](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/5) | k_thread・k_msgqによる処理とデータの分担 |
| 5 | UART APIでHello Worldを送信する | `stages/05_uart_tx/` | [段階4](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/5) | [#6](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/6) | UARTデバイス取得・準備確認・送信API |
| 6 | UARTのポーリング受信と割り込み受信でエコーを実装する | `stages/06_uart_echo/` | [段階5](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/6) | [#7](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/7) | UART受信API・割り込み・キュー・待機 |
| 7 | NT-Shellを組み込みLED制御コマンドを追加する | `stages/07_ntshell/` | [段階6](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/7) | [#8](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/8) | Zephyrアプリへの外部ライブラリ接続とUART担当 |
| 8 | NT-ShellにRAMのリード・ライト・ダンプを追加する | `stages/08_memory_ram/` | [段階7](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/8) | [#9](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/9) | 実施済み補助：Zephyrアプリ上のRAM。C解析の深掘りは任意 |
| 9 | ZephyrのGPIOドライバと周辺レジスタの役割を確認する | `stages/09_register_access/` | [段階8](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/9) | [#10](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/10) | 補助：Devicetree・GPIO API・Kconfigとドライバの担当。GDB照合は省略 |
| 10 | Consoleの文字入出力と標準ShellでLEDを操作する | `stages/10_zephyr_shell/` | 段階4・6・7 | [#28](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/28) | ConsoleとShellの役割、UARTバックエンド、コマンド登録・引数・ヘルプ・補完 |
| 11 | C/C++の単体テスト環境を構築し、小さな処理を検証する | `stages/11_unit_testing/` | [段階10](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/28) | [#29](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/29) | Ztest・Twister・CMake／Kconfig、依存の切り分け、テスト結果の読み方 |
| 12 | LCD_PAR_S035に基本図形とカラーパターンを描画する | `stages/12_lcd_basic/` | [段階2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3) | [#11](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/11) | shield・Devicetree・Kconfig・デバイス準備確認・Display API |
| 13 | LCDに静止画像を表示し描画時間とRAM使用量を確認する | `stages/13_lcd_image/` | [段階12](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/11) | [#12](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/12) | Display APIのバッファ指定・部分更新・Zephyr時刻API |
| 14 | DVP 20-pin OV7670から画像を取得する | `stages/14_camera_capture/` | [段階4](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/5) | [#13](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/13) | Video APIの形式設定・バッファ受け渡し・開始停止・タイムアウト |
| 15 | OV7670のカメラ画像をLCD_PAR_S035に表示する | `stages/15_camera_display/` | [段階13](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/12), [段階14](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/13) | [#14](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/14) | Video／Displayの接続、必要なスレッド・キューとバッファの所有者 |
| 16 | （任意・発展）ZephyrのLVGL環境を構築し最小表示を行う | `stages/16_lvgl_setup/` | [段階13](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/12) | [#15](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/15) | westモジュール・Kconfig・chosen display・DisplayドライバとLVGLの接続 |
| 17 | （任意・発展）LVGLでLCD_PAR_S035のUIを周期更新する | `stages/17_lvgl_display/` | [段階16](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/15) | [#16](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/16) | LVGL実行コンテキスト、時刻・待機、キューによるUIへのデータ受け渡し |

## 追加・置換する段階の範囲

### 段階10：Consoleから標準Shellへ

本人の依頼で2026-11-08に実装コミット`fc4a5d5`をプッシュし、[PR #31](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/31)を作成した。PRはOPEN・未マージ、イシュー #28もOPEN。マージ・クローズは本人の別途の確認・承認後に行う。

2026-11-08：Consoleの1文字入出力と標準Shellのhello、LEDのon／off・点滅間隔変更・停止、ヘルプ・Tab補完・履歴、入力待ち中のLED継続を本人確認済み。最終状態は消灯・点滅停止。[段階10の記録](../stages/10_zephyr_shell/README.md)へ設定・API・処理担当の対応と実施結果を保存し、READMEの本人確認用6項目の一致確認も受領し、段階10の学習・記録確認は完了（PRマージ・イシュークローズは未承認）。

- **使うZephyr機能**：Consoleの入出力API、標準Shell、UARTバックエンド、GPIO API、既習のスレッド・キュー。
- **使う目的**：文字入出力とコマンド処理の役割を分け、Zephyr標準の操作用CLIを作る。
- **本人が変える点**：Console／Shellの設定、コマンド登録・ヘルプ文、LED制御の引数と点滅間隔。
- **見える動作**：Consoleで入力文字が返り、Shellではhelp・Tab補完・LED操作が動く。入力待ち中もLEDの周期処理が継続する。

まずShell無効のConsole版で1文字の入力・出力を試す。その後、同じ段階の別構成として標準Shell版を作り、最小コマンド→LEDのon／off→点滅間隔の順に増やす。
`CONFIG_SHELL`とUARTバックエンド、`zephyr,console`／`zephyr,shell-uart`の指定と生成設定を対応させる。
同じUARTの受信担当は一つにし、Console入力・NT-Shell・独自UART受信とShellを同時に受信させない。
長い処理はShellハンドラで待ち続けず、必要な要求をLED担当へ渡す。
NT-Shell版の既存成果は保持し、[QA-045の比較](qa.md#qa-045-supplement)を実際の操作に結び付ける。RAM・レジスタコマンドの全面移植はこの段階の条件にしない。

### 段階11：C/C++の単体テスト

- **使うZephyr機能**：Ztest（テスト定義・アサーション）、Twister（ビルド・実行・結果集計）、CMake／Kconfig。必要な箇所だけFFF等のfakeを使う。
- **使う目的**：実機なしで検証できる処理を切り出し、変更後に期待する結果が保たれることを自動確認する。
- **本人が変える点**：段階10の引数から制御要求を作る処理、入力と期待値、C/C++のテスト・ビルド設定。
- **見える動作**：本人が追加したテストがPASSし、意図的に処理を誤らせるとFAIL、修正するとPASSに戻る。失敗したケースと理由を結果から読める。

このプロジェクトでは、Zephyr標準のZtest＋Twisterを最初の構成として推奨する。C++もZtestで小さな関数・クラスを検証し、C++用設定を含む最小構成を記録する。既存Cアプリ全体のC++化は不要。
現在のWindows／PowerShellと段階9のCodexによるQEMU実行実績を生かし、初期の実行先は`qemu_cortex_m3`を候補とする。Twister経由とC++版の実行は着手時に検証する。
QEMU上でZephyrを動かしつつ対象関数の依存を置き換えて単体を検証する方式と、カーネルを含まない`unit_testing`方式を区別する。
`native_sim`／`unit_testing`はLinux環境を前提とするため、Windows上で必要なら別途Linux環境を用意する発展項目とし、最初から環境の追加を必須にしない。

1. 採用Zephyrの固定コミットに合わせ、必要なPython依存、QEMU、Twisterの起動方法、C/C++コンパイラを確認して記録する。
2. Cの対象関数一つとテスト一つから始め、本人がPASS→意図したFAIL→修正後のPASSを確認する。
3. 正常入力と代表的な不正入力を追加し、必要ならGPIOへ渡す要求や戻り値をfakeで確認する。
4. 同じ手順でC++の最小対象とテストを実行し、再実行コマンドと結果を残す。

段階9のCodexによる9テスト成功は参考実績であり、本人の単体テスト学習の完了には数えない。
実機でのUART受信、配線、MCXN947のGPIO、表示タイミングの保証は別の実機確認として扱う。
GoogleTestは既存のC++テスト資産を利用する場合の選択肢とし、FFF・カバレッジ・CIの全面導入や網羅率目標は初回の必須条件にしない。

### 段階16・17：LVGLによるGUI応用

- **使うZephyr機能**：westで固定されたLVGLモジュール、Kconfig、Devicetreeの`zephyr,display`、Displayドライバ、時刻・待機・必要なキュー。
- **使う目的**：Display APIでの描画を土台に、Zephyrの既存統合を使ってGUIを動かす。
- **本人が変える点**：段階16では表示設定・ラベル、段階17ではカウンタの更新間隔・表示内容・データ受け渡し。
- **見える動作**：段階16でLCDに最小のラベルが表示され、段階17でカウンタ等が継続更新される。

段階16は採用コミットの`samples/subsys/display/lvgl`とLCD_PAR_S035のshield設定を基に、実機の最小表示を目指す。
Zephyrが固定するLVGLの版と設定を使用し、別の最新版へ置き換える作業やPCシミュレータ構築は必須にしない。
公式の統合が存在することと、このボード・LCD・設定の組合せで動作確認済みであることは区別する。今回の実機表示は未確認。
段階17ではUIを更新する実行コンテキストを決め、採用版のLVGL処理方式と排他規則に沿って更新する。
別スレッドのデータを表示する場合は、既習のキュー等でUI担当へ渡す。全画面バッファを前提にせず、描画バッファ・ヒープ・スタックとFlash／RAMを確認する。
カメラ合成とタッチ入力は後続の発展項目とし、最小のラベル・周期更新から進める。

### 選定に参照した一次資料

公式Web資料と採用ローカルZephyrのShell・Ztest・Twister資料、C++のZtest例（`tests/lib/cpp/cxx`）、LVGLサンプルを照合した。今回の作業は計画の検討と資料確認であり、追加環境の構築・ビルド・実機確認は未実施。

- [Console API](https://docs.zephyrproject.org/latest/doxygen/html/group__console__api.html)
- [Shell](https://docs.zephyrproject.org/latest/services/shell/index.html)
- [Ztest](https://docs.zephyrproject.org/latest/develop/test/ztest.html)
- [Twister（採用コミット）](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/doc/develop/test/twister.rst)
- [C++のZtest例（採用コミット）](https://github.com/zephyrproject-rtos/zephyr/tree/dccb09599635bdff17633fa7e9dab014b91dce90/tests/lib/cpp/cxx)
- [native_sim](https://docs.zephyrproject.org/latest/boards/native/native_sim/doc/index.html)
- [LVGL basic sample](https://docs.zephyrproject.org/latest/samples/subsys/display/lvgl/README.html)
- [LCD_PAR_S035](https://docs.zephyrproject.org/latest/boards/shields/lcd_par_s035/doc/index.html)

## 進め方

- 基本は段階番号順に進めます。前提段階は技術的な依存関係です。
- LCD系は段階2、カメラ取得は段階4を終えれば着手できます。
- LVGL系はDisplay APIの学習後に選ぶ任意の発展課題です。技術的前提は段階13で、カメラ系の完了は不要です。
- 各段階を始める際に対応フォルダを追加し、動く成果物と実機確認の記録を残します。
- ユーザー本人がビルド・実機確認を行い、観察した結果を記録します。Codexは何のZephyr機能を何のために使うかを先に短く説明します。デバッグは必要な問題の切り分けに使います。
- Codexによる準備・事前検証と、ユーザー本人の実習結果は実施者を明記して区別します。Codexの検証だけでは学習の完了条件にチェックしません。
- ユーザーが質問し回答した内容は[QA表](qa.md)へ追記します。質問した段階、質問日時（日本時間）、カテゴリ、質問内容、回答を記録し、日時が不明または代替時刻の場合は明記します。
- ユーザー本人が完了条件を満たしたことを確認し、クローズを依頼または許可した場合のみ、イシューをクローズします。
- カメラ映像とLVGL UIの統合、タッチ入力、デュアルコアは段階17の後の発展課題です。

## 各段階のREADMEに残す情報

1. 目的、前提段階、使用する機材と配線。
2. 使用バージョンと、前段階から変更した設定。
3. ビルド・書き込み・実行のコマンドと実行ディレクトリ。デバッグは実施した場合に記録する。
4. 学ぶAPIとKconfig／Devicetreeの変更点。
5. 期待する動作、実際の確認結果、未確認事項、トラブル対応。
6. Flash／RAM使用量をバイト数とマイコン全体の容量に対する使用率（%）で記録する。2026-10-11の本人の指定により、次回の段階から適用する。画像・GUI段階ではバッファ構成と更新時間も記録する。
7. 学習に使用したAIツール、AIモデルの識別子・バージョン、推論設定、利用期間と確認根拠。[記録方法と後続段階の欄](ai-models.md)に従い、モデル変更時は行を追加する。

### Flash／RAM使用率の分母と確認範囲

[NXPのMCXN947仕様](https://www.nxp.com/part/MCXN947VKLT)を基に、内蔵Flashは2 MiB（2,097,152 B）、内蔵SRAMは512 KiB（524,288 B）を分母とする。使用率は「使用量（B）÷全体容量（B）×100」で計算し、小数点以下2桁を基本とする。

ビルドログのRAM領域は今回320 KiBなので、ログの1.36%（4,448 B÷327,680 B）と、マイコン全体に対する0.85%（4,448 B÷524,288 B）は分母が異なる。全体容量に対する割合を主に表示し、ビルドで割り当てられた領域の容量と使用率も区別して残す。SRAMXなど複数のRAM領域を使う場合は、mapファイルで領域の重複を確認し、重複しない使用量を合計する。

これらは対象アプリのビルド時に確定する使用量であり、実行時のスタック・ヒープの最大使用量や、別コアのプログラムを含めた実測値とは区別する。512 KiB全体をCPU0アプリが自由に使えることを意味しない。外付けメモリや、ECCなどによる利用可能容量の変更は別途明記する。

公開済みの公式資料の `latest` は変化します。実装時には採用したZephyrと、そのwestマニフェストで固定されたLVGLの資料・ソースに照合します。
