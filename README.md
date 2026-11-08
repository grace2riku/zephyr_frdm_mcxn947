# Zephyr学習プロジェクト — FRDM-MCXN947

NXP FRDM-MCXN947評価ボードを使い、小さな動作確認を積み重ねてZephyrの使い方を段階的に学ぶプロジェクトです。

学習の軸は、**Zephyrの設定からデバイスを使う流れ、ドライバAPI、スレッド・キューによる役割分担**です。
GPIO・UART・LCD・OV7670カメラを題材に、Devicetree／KconfigとAPIの動作を対応させます。
NT-Shellの接続、RAM・レジスタ操作は補助教材として扱います。
2026-11-08に本人が承認したロードマップでは、標準Console／ShellとC/C++の単体テストを追加し、GUIの任意発展課題をLVGLへ変更しています。

2026-11-07に本人が、段階9に限らず今後もこの目的から離れない方針を明示しました。
各操作の前に「使うZephyr機能・目的・本人が変える点・見える動作」を短く示します。
GDBの照合やC・MCU固有の細部の調査は必要時の補助に位置付けます。
継続方針は[AGENTS.md](AGENTS.md)と[ロードマップ](docs/roadmap.md)を参照してください。

## 現在の状態

2026-10-11：段階0の選定・記録、段階1の本人によるビルド・書き込み・UART・GDB実習とPR確認、段階2の本人によるLED点灯・消灯の実習・説明・記録・PR確認が完了しました。本人の承認後に[PR #17](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/17)と[PR #18](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/18)をマージし、イシュー #2・#3はcompletedとしてクローズ済みです。[段階1の記録](stages/01_hello_world/README.md)と[段階2の記録](stages/02_led_control/README.md)を参照してください。段階2終了時のソース・実機は消灯版です。DTCは段階1の実物確認に基づき、段階0の計画を1.6.1の別導入へ訂正しました。

学習の主体はユーザー本人です。Codexは各操作の理由を日本語で説明し、手順の準備とトラブル対応を支援します。Codexの動作確認はユーザー本人の実習完了として扱いません。ユーザー本人が完了条件を満たしたことを確認し、クローズを依頼または許可した場合のみ、学習イシューをクローズします。

段階3（#4）は本人の両版のビルド・書き込み、UART時刻差比較、赤色LED点滅・速度変更、待機と周期の説明まで確認済みです。UARTとLEDの各状態の対応は500 ms版で確認し、250 ms版は速くて目視では断定できなかったと記録しています。最終ソースとボードは250 ms版。Codexの両版ビルド確認は別に記録し、本人によるREADMEと[PR #20](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/20)の確認も完了し、マージとイシュー #4のクローズを承認済みです。[段階3の手順と確認状況](stages/03_led_blink/README.md)を参照してください。

## 学習計画

段階4（#5）に着手しました。本人の環境・Zephyrコミット確認、作業ブランチ作成、ソース・設定の表示確認とCPU0向けビルドまで完了。
本人の書き込みも成功表示と終了コード0で確認済みです。Codexのビルド確認は別に記録しています。本人が最初の1巡のUARTと赤色LEDを確認し、低速→高速→消灯→高速再開→消灯の動作を報告しました。提供UARTの時刻差も比較済みです。続いて本人が3巡とも同じ動作を続け、UARTがseq 25まで進み、ERRORなし・set_retすべて0と報告しました。本人による役割分担・状態更新担当・待機・優先度・スタックの説明も確認済みです。READMEも本人が実際の学習・観察結果と一致すると確認し、PR作成を依頼しました。[PR #21](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/21)を作成し、本人によるPR内容の確認も完了しました。本人の別途の明示的な承認後、PR #21をマージし、イシュー #5をcompletedとしてクローズしました。段階4の学習は完了です。
[段階4の手順と確認状況](stages/04_threads_queues/README.md)を参照してください。

2026-10-12：段階5（#6）に着手しました。本人の環境・Zephyrコミット・UART定義・作業ブランチ確認と、UART取得、文字列送信、LEDスレッド、観察用待機・繰り返し、設定とCMakeの表示確認まで進みました。2026-10-13に本人のビルド抜粋と終了コード0を受領し、ビルド成功を確認しました。Codexの事前ビルド・生成設定確認は別に記録しています。本人のCMakeCache・Kconfig・UART・LED・GPIOの生成定義確認も完了し、COM5とプローブHN2APUBRDCEPTも再確認済みです。本人の書き込みもFlash成功表示と終了コード0で確認済みです。本人のTera Term設定も確認済みです。RESET後の起動表示とcycle 1〜3のHello World受信を確認し、本人はLEDの点滅も報告しました。本人が送信ループ中と待機中のLED点滅継続、文字がゆっくり増える様子も確認しました。本人の出力経路・文字列終端・改行・待機・共有UARTの説明と、ログ比較の補足説明後の理解報告も受領しました。本人のREADME確認も2026-10-14に完了しました。本人の依頼によりCodexがコミット・プッシュし、[PR #22](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/22)を作成しました。本人のPR確認と別途の明示的な承認後、2026-10-14にPR #22をマージし、イシュー #6をcompletedとしてクローズしました。段階5の学習は完了です。[段階5の手順と確認状況](stages/05_uart_tx/README.md)を参照してください。

段階6（#7）は2026-10-14に着手し、2026-10-24に本人の文字単位・行単位エコー、割り込み受信中のLED点滅継続、31 B貼り付け、32 B超過後の復帰、CR/LFを確認しました。ポーリング版で発生した貼り付け時のUARTオーバーランは検査・通知・復帰処理を追加し、本人の実機で確認済みです。割り込み版の本人ビルドは終了コード0。Codexの別保存先での事前ビルドとは区別して記録しています。本人の3行・100行連続入力と最後のokも確認済みです。今回の100行ではキュー満杯通知は観測されず、満杯分岐の実機確認は未実施です。ISRとキューの動作を図解し、本人のREADME確認も2026-10-24に完了しました。割り込み版の書き込み終了コード0、COM5・プローブ一致、100行入力前後のLED継続も確認済みです。本人のPR確認と明示的なマージ承認後、2026-10-24に[PR #23](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/23)をマージしました。その後の別途の明示的な承認に基づき、イシュー #7の全チェック項目を完了に更新し、completedとしてクローズしました。段階6は完了です。[段階6の手順と確認状況](stages/06_uart_echo/README.md)を参照してください。

2026-10-31：段階7（#8）は完了しました。本人がビルド・書き込み、NT-Shellの起動・help・LED操作と入力待ち中の点滅継続、不正入力・編集・履歴・取消し・CR／LF／CR+LF、READMEとPRを確認しました。本人の明示的な承認後に[PR #25](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/25)をマージし、イシュー #8の全10チェック項目を完了へ更新してcompletedとしてクローズしました。ローカルmainも同期済みです。最終状態はLED消灯、送信CR+LF・受信CR。未実施の追加ケースは区別して[段階7の記録](stages/07_ntshell/README.md)に残しています。

2026-11-01：段階8（#9）は完了しました。本人が8／16／32 bitのRAM読み書き・ダンプ、境界・不正入力の拒否、RESET初期化とGDB照合、README・PRを確認しました。明示的な承認後に[PR #26](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/26)をマージし、イシュー #9の全8チェック項目を完了・completedとしてクローズしました。Flash 34,200 B・RAM 10,992 B。GDB終了直後のUART無応答はRESETで復旧し、原因未確定として記録を保持しています。最終状態はLED消灯・RAM初期パターン・GDB終了済み。ローカルmainも同期済みです。[段階8の手順と確認結果](stages/08_memory_ram/README.md)を参照してください。

2026-11-01：段階9（#10）に着手しました。`codex/stage9-register-access`にGPIO0 bit 10に限定するレジスタ操作を実装し、CPU0向け事前ビルド・QEMUの9テストを確認しました。本人提供のRM（Rev. 7, 2025-10-10）との本文照合もCodexが完了し、コード変更は不要でした。本人の環境4項目も一致を確認しました。2026-11-07に本人のファイル照合PASS・終了コード0と、CPU0ビルド166/166・終了コード0を確認しました。Flash 34,436 B・RAM 8,744 B。register_commands.cの理解は本人の依頼で保留し、ビルド成功とは区別しています。今回の接続先も本人の列挙でCOM5／HN2APUBRDCEPTと確認しました。本人のFlash成功表示・0x8684 Bロード・終了コード0も確認しました。本人の起動表示・プロンプト・初期LED消灯・helpの確認報告も受領しました。reg infoと3レジスタの初期readも提供UARTで確認し、LED消灯維持の報告を受領しました。PCOR 2回／PSOR 1回のOK応答・PDOR変化と、LED点灯→点灯維持→消灯の本人報告も確認しました。PTOR 2回の反転と消灯状態での3操作先への値0による状態維持もUARTと本人の目視報告で確認しました。不正入力9件の拒否と、LED消灯維持・正常readとhelpへの応答も確認しました。本人の依頼によりGDB照合は今回の必須項目から除外しました。ZephyrのGPIOドライバと設定の役割を確認する補助教材として整理し、2026-11-08に本人がREADMEの6項目と実習内容の一致を確認し、コミット・プッシュ・PR作成を依頼しました。[PR #27](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/27)を作成し、同日に本人の明示的な依頼でmainへマージしました（a198abd）。続いて本人の依頼でイシュー #10をcompletedとしてクローズしました。段階9は完了です。GDBは未実施の省略です。[段階9の仕様と手順](stages/09_register_access/README.md)を参照してください。

2026-11-08：本人が提案のロードマップで進めることを承認しました。段階10のConsole／標準Shellを [#28](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/28)、段階11のC/C++単体テスト（Ztest＋Twister、初期実行先はQEMU候補）を [#29](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/29) として作成しました。旧段階10〜15は新段階12〜17へ移し、旧Slint課題は新段階16・17のLVGL課題へ変更しました。既存イシュー #11〜#16 と継続方針も更新済みです。同日に段階10へ着手し、本人がConsoleの1文字入出力、標準Shellのhello、LEDのon／off・点滅間隔変更・停止、ヘルプ・Tab補完・履歴、入力待ち中の点滅継続を確認しました。最終状態は赤色LED消灯・点滅停止です。設定・処理担当の対応を記録し、READMEの本人確認用6項目の一致確認も受領し、段階10の学習・記録確認は完了（PR #31マージ済み・イシュー #28はcompletedでクローズ済み）です。本人の依頼でコミット・プッシュし、[PR #31](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/31)を作成しました。2026-11-08に本人がPR内容を確認し、mainへのマージを依頼しました。実施結果はイシュー #28へ記録します。段階11〜17は未着手です。[段階10の記録](stages/10_zephyr_shell/README.md)を参照してください。

- [段階0〜17の学習ロードマップ](docs/roadmap.md)
- [段階別の質問と回答 QA表](docs/qa.md)
- [学習に使用したAIモデルの記録方法](docs/ai-models.md)
- [学習イシュー](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues)
- [段階0：バージョンと開発環境の方針](stages/00_versions/README.md)
- [段階1：環境構築とHello World](stages/01_hello_world/README.md)
- [段階2：GPIO APIでLEDを点灯・消灯する](stages/02_led_control/README.md)
- [段階3：k_msleep()でLEDを周期的に点滅させる](stages/03_led_blink/README.md)
- [段階4：スレッドとメッセージキューでLEDを制御する](stages/04_threads_queues/README.md)
- [段階5：UART APIでHello Worldを送信する](stages/05_uart_tx/README.md)
- [段階6：UARTのポーリング受信と割り込み受信でエコーを実装する](stages/06_uart_echo/README.md)
- [段階7：NT-ShellでLEDをコマンド操作する](stages/07_ntshell/README.md)
- [段階8：NT-Shellで専用RAMを読み書き・ダンプする](stages/08_memory_ram/README.md)
- [段階9：ZephyrのGPIOドライバと周辺レジスタの役割を確認する](stages/09_register_access/README.md)
- [段階10：Consoleの文字入出力と標準Shell](stages/10_zephyr_shell/README.md)

## フォルダ構成

```text
zephyr_frdm_mcxn947/
├── README.md
├── .gitignore
├── docs/
│   ├── roadmap.md
│   ├── qa.md
│   └── ai-models.md
└── stages/
    ├── 00_versions/
    │   ├── README.md
    │   ├── versions.md
    │   ├── west-frozen.yml
    │   ├── west-projects.txt
    │   └── python-requirements-lock.txt
    ├── 01_hello_world/
    │   ├── README.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── debug.conf
    │   ├── src/main.c
    │   ├── scripts/
    │   └── evidence/
    ├── 02_led_control/
    │   ├── README.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/main.c
    │   └── evidence/
    ├── 03_led_blink/
    │   ├── README.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/main.c
    │   ├── evidence/
    │   └── user-evidence/
    ├── 04_threads_queues/
    │   ├── README.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/main.c
    │   ├── evidence/
    │   └── user-evidence/
    ├── 05_uart_tx/
    │   ├── README.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/main.c
    │   ├── evidence/
    │   └── user-evidence/
    ├── 06_uart_echo/
    │   ├── README.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── prj_irq.conf
    │   ├── src/main.c
    │   ├── src/poll_line.c
    │   ├── src/irq_line.c
    │   └── evidence/
    ├── 07_ntshell/
    │   ├── README.md
    │   ├── UPSTREAM.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/
    │   ├── third_party/ntshell/
    │   ├── scripts/
    │   ├── tests/commands/
    │   └── evidence/
    ├── 08_memory_ram/
    │   ├── README.md
    │   ├── UPSTREAM.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/
    │   ├── third_party/ntshell/
    │   ├── scripts/
    │   ├── tests/commands/
    │   ├── evidence/
    │   └── user-evidence/
    ├── 09_register_access/
    │   ├── README.md
    │   ├── UPSTREAM.md
    │   ├── CMakeLists.txt
    │   ├── prj.conf
    │   ├── src/
    │   ├── third_party/ntshell/
    │   ├── scripts/
    │   ├── tests/commands/
    │   ├── evidence/
    │   └── user-evidence/
    └── 10_zephyr_shell/
        ├── README.md
        ├── console/
        │   ├── CMakeLists.txt
        │   ├── prj.conf
        │   └── src/main.c
        └── shell/
            ├── CMakeLists.txt
            ├── prj.conf
            └── src/
                ├── main.c
                ├── led_control.c
                └── led_control.h
```

段階10はConsole最小版と標準ShellのLED点滅版を追加しました。段階11以降のフォルダは、各イシューに着手するときに順次追加します。予定名はロードマップに記載しています。

実装段階では各フォルダにREADMEと必要なアプリのソース・設定を置き、段階ごとにビルド・検証できる状態を残します。Zephyr本体、SDK、仮想環境、ビルド出力はこのリポジトリへコミットしません。段階1で取得した有効67モジュールのSHAとPython依存の固定情報を段階0フォルダへ保存しました。

## 使用機材と基本方針

- 評価ボード：NXP FRDM-MCXN947。
- LCD：NXP LCD_PAR_S035 TFT LCD Module。
- カメラ：DVP 20-pin OV7670 Camera Module。
- 初期ターゲット：`frdm_mcxn947/mcxn947/cpu0`。
- 開発ホスト：Windows 11ネイティブ、PowerShell、VS Code（詳細は段階0の記録を参照）。
- 各段階でビルド手順、期待する動作、実機確認結果を記録します。
- 依存OSSを組み込む際は取得元とバージョンを記録し、ライセンス表示を保持します。

## 公式資料

- [FRDM-MCXN947](https://docs.zephyrproject.org/latest/boards/nxp/frdm_mcxn947/doc/index.html)
- [Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/)
- [LCD_PAR_S035](https://docs.zephyrproject.org/latest/boards/shields/lcd_par_s035/doc/index.html)
- [DVP 20-pin OV7670](https://docs.zephyrproject.org/latest/boards/shields/dvp_20pin_ov7670/doc/index.html)
- [Video capture（採用版v4.4.2のソース資料）](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/samples/drivers/video/capture/README.rst)
- [Slint MCU環境構築](https://docs.slint.dev/latest/docs/cpp/mcu/generic/)
- [Slint Zephyrデモ](https://github.com/slint-ui/slint/blob/master/demos/printerdemo/zephyr/README.md)
