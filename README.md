# Zephyr学習プロジェクト — FRDM-MCXN947

NXP FRDM-MCXN947評価ボードを使い、小さな動作確認を積み重ねてZephyrの使い方を段階的に学ぶプロジェクトです。

GPIO、スレッド、UART、NT-Shell、メモリ／レジスタ操作、LCD、OV7670カメラ、Slint描画までを対象にします。

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

- [段階0〜15のロードマップ](docs/roadmap.md)
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
    └── 06_uart_echo/
        ├── README.md
        ├── CMakeLists.txt
        ├── prj.conf
        ├── prj_irq.conf
        ├── src/main.c
        ├── src/poll_line.c
        ├── src/irq_line.c
        └── evidence/
```

段階6のフォルダを追加しました。段階7以降のフォルダは、各イシューに着手するときに順次追加します。予定名はロードマップに記載しています。

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
