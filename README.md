# Zephyr学習プロジェクト — FRDM-MCXN947

NXP FRDM-MCXN947評価ボードを使い、小さな動作確認を積み重ねてZephyrの使い方を段階的に学ぶプロジェクトです。

GPIO、スレッド、UART、NT-Shell、メモリ／レジスタ操作、LCD、OV7670カメラ、Slint描画までを対象にします。

## 現在の状態

2026-10-11：段階0の選定・記録は完了しました。段階1は本人のビルド・書き込み・UART・GDB実習とPR確認まで完了し、本人の承認後に[PR #17](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/17)をマージ、[イシュー #2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/2)をcompletedとしてクローズしました。[段階1の日本語手順・事前検証・本人の実習結果](stages/01_hello_world/README.md)を参照してください。段階2は[LED制御アプリと手順](stages/02_led_control/README.md)を準備し、Codexの点灯版ビルド・生成設定確認が済んだ状態です。2026-10-11に本人のセッション設定・入力ファイル・ボード定義の読み出しと、点灯版のビルド成功（終了コード0）を確認しました。生成設定、点灯版の書き込み成功（終了コード0）、UARTのAPI成功結果と赤色LEDの点灯を本人の報告で確認済み。本人による消灯版へのソース変更、ビルド・書き込み成功（各終了コード0）、UARTのAPI成功結果と赤色LEDの消灯を確認済み。本人のGPIO・極性の説明と記録の相違なしの確認を受領済み。PRの確認待ちで、[#3](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3)はopenです。DTCは段階1の実物確認に基づき、段階0の計画を1.6.1の別導入へ訂正しました。

学習の主体はユーザー本人です。Codexは各操作の理由を日本語で説明し、手順の準備とトラブル対応を支援します。Codexの動作確認はユーザー本人の実習完了として扱いません。ユーザー本人が完了条件を満たしたことを確認し、クローズを依頼または許可した場合のみ、学習イシューをクローズします。

## 学習計画

- [段階0〜15のロードマップ](docs/roadmap.md)
- [段階別の質問と回答 QA表](docs/qa.md)
- [学習イシュー](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues)
- [段階0：バージョンと開発環境の方針](stages/00_versions/README.md)
- [段階1：環境構築とHello World](stages/01_hello_world/README.md)
- [段階2：GPIO APIでLEDを点灯・消灯する](stages/02_led_control/README.md)

## フォルダ構成

```text
zephyr_frdm_mcxn947/
├── README.md
├── .gitignore
├── docs/
│   ├── roadmap.md
│   └── qa.md
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
    └── 02_led_control/
        ├── README.md
        ├── CMakeLists.txt
        ├── prj.conf
        ├── src/main.c
        └── evidence/
```

段階3以降のフォルダは、各イシューに着手するときに順次追加します。予定名はロードマップに記載しています。

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
